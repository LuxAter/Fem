#include "script.hpp"

#include <array>
#include <functional>
#include <memory>
#include <string>
#include <vector>

extern "C" {
#include <lua5.3/lauxlib.h>
#include <lua5.3/lua.h>
#include <lua5.3/lualib.h>
}

#include "linalg.hpp"
#include "logger.hpp"

static lua_State* state_;
static std::string file_name_;

void arta::script::load_script(const std::string& file) {
  state_ = luaL_newstate();
  luaL_openlibs(state_);
  if (luaL_loadfile(state_, file.c_str()) || lua_pcall(state_, 0, 0, 0)) {
    state_ = NULL;
    log::error("Failed to load script file \"%s\"", file.c_str());
  } else {
    file_name_ = file;
    log::success("Loaded script file \"%s\"", file.c_str());
  }
}

void arta::script::close_script() {
  if (state_ != NULL) {
    lua_close(state_);
    state_ = NULL;
    log::success("Closed script file \"%s\"", file_name_.c_str());
  } else {
    log::warning("Failed to close script file \"%s\"", file_name_.c_str());
  }
}

bool arta::script::valid() { return state_ != NULL; }

bool arta::script::has(const std::string& var) {
  lua_getglobal(state_, var.c_str());
  bool res = !lua_isnil(state_, -1);
  lua_pop(state_, -1);
  return res;
}

double arta::script::A(const double& x, const double& y, const unsigned& i,
                       const unsigned& j) {
  lua_getglobal(state_, "A");
  double val = 0.0;
  if (!lua_isnil(state_, -1) && lua_istable(state_, -1)) {
    lua_pushnumber(state_, i);
    lua_gettable(state_, -2);
    if (lua_istable(state_, -1)) {
      lua_pushnumber(state_, j);
      lua_gettable(state_, -2);
      if (lua_isfunction(state_, -1)) {
        lua_pushnumber(state_, x);
        lua_pushnumber(state_, y);
        lua_call(state_, 2, 1);
        val = lua_tonumber(state_, -1);
      } else if (lua_isnumber(state_, -1)) {
        val = lua_tonumber(state_, -1);
      }
    }
  }
  int n = lua_gettop(state_);
  lua_pop(state_, n);
  return val;
}
double arta::script::B(const double& x, const double& y, const unsigned& i) {
  lua_getglobal(state_, "B");
  double val = 0.0;
  if (!lua_isnil(state_, -1) && lua_istable(state_, -1)) {
    lua_pushnumber(state_, i);
    lua_gettable(state_, -2);
    if (lua_isfunction(state_, -1)) {
      lua_pushnumber(state_, x);
      lua_pushnumber(state_, y);
      lua_call(state_, 2, 1);
      val = lua_tonumber(state_, -1);
    } else {
      val = lua_tonumber(state_, -1);
    }
  }
  int n = lua_gettop(state_);
  lua_pop(state_, n);
  return val;
}
double arta::script::C(const double& x, const double& y) {
  lua_getglobal(state_, "C");
  double val = 0.0;
  if (!lua_isnil(state_, -1)) {
    if (lua_isfunction(state_, -1)) {
      lua_pushnumber(state_, x);
      lua_pushnumber(state_, y);
      lua_call(state_, 2, 1);
      val = lua_tonumber(state_, -1);
    } else if (lua_isnumber(state_, -1)) {
      val = lua_tonumber(state_, -1);
    }
  }
  lua_pop(state_, -1);
  return val;
}
double arta::script::forcing(const double& x, const double& y,
                             const double& t) {
  const char* alias[] = {"f", "force", "forcing"};
  for (unsigned i = 0; i < 3; ++i) {
    lua_getglobal(state_, alias[i]);
    if (!lua_isnil(state_, -1)) break;
  }
  if (lua_isnil(state_, -1)) return 0.0;
  lua_pushnumber(state_, x);
  lua_pushnumber(state_, y);
  lua_pushnumber(state_, t);
  lua_call(state_, 3, 1);
  double val = lua_tonumber(state_, -1);
  lua_pop(state_, -1);
  return val;
}

double arta::script::boundary(const unsigned& i, const double& x,
                              const double& y, const double& t) {
  double val = 0.0;
  const char* alias[] = {"bdry", "bndry", "boundary"};
  for (unsigned i = 0; i < 3; ++i) {
    lua_getglobal(state_, alias[i]);
    if (!lua_isnil(state_, -1)) break;
  }
  if (lua_isnil(state_, -1) || !lua_istable(state_, -1)) return 0.0;
  lua_pushnumber(state_, i);
  lua_gettable(state_, -2);
  if (lua_isnil(state_, -1)) {
    lua_pop(state_, 1);
    lua_pushnumber(state_, 0);
    lua_gettable(state_, -2);
    if (lua_isnil(state_, -1)) {
      val = 0.0;
    } else if (lua_isnumber(state_, -1)) {
      val = lua_tonumber(state_, -1);
    } else if (lua_isfunction(state_, -1)) {
      lua_pushnumber(state_, x);
      lua_pushnumber(state_, y);
      lua_pushnumber(state_, t);
      lua_call(state_, 3, 1);
      val = lua_tonumber(state_, -1);
    }
    lua_pop(state_, -1);
  } else if (lua_isnumber(state_, -1)) {
    val = lua_tonumber(state_, -1);
  } else if (lua_isfunction(state_, -1)) {
    lua_pushnumber(state_, x);
    lua_pushnumber(state_, y);
    lua_pushnumber(state_, t);
    lua_call(state_, 3, 1);
    val = lua_tonumber(state_, -1);
  }
  int n = lua_gettop(state_);
  lua_pop(state_, n);
  return val;
}

std::string arta::script::gets(const std::string& var) {
  std::string res = std::string();
  lua_getglobal(state_, var.c_str());
  if (lua_isstring(state_, -1)) {
    res = std::string(lua_tostring(state_, -1));
  }
  lua_pop(state_, 1);
  return res;
}

double arta::script::getd(const std::string& var) {
  double res = double();
  lua_getglobal(state_, var.c_str());
  if (lua_isnumber(state_, -1)) {
    res = static_cast<double>(lua_tonumber(state_, -1));
  }
  lua_pop(state_, 1);
  return res;
}

std::string arta::script::get_file_name() { return file_name_; }
