#include "script.hpp"

extern "C" {
#include <lua5.3/lauxlib.h>
#include <lua5.3/lua.h>
#include <lua5.3/lualib.h>
}

#include <string>
#include <vector>

namespace fem {
namespace script {
  static lua_State* lua_state_ = nullptr;
}  // namespace script
}  // namespace fem

bool fem::script::open_script(const std::string& file_name) {
  lua_state_ = luaL_newstate();
  luaL_openlibs(lua_state_);
  if (luaL_loadfile(lua_state_, file_name.c_str()) ||
      lua_pcall(lua_state_, 0, 0, 0)) {
    lua_state_ = nullptr;
    return false;
  }
  return true;
}

double fem::script::call(const double& x, const double& y, std::string func) {
  lua_getglobal(lua_state_, func.c_str());
  lua_pushnumber(lua_state_, x);
  lua_pushnumber(lua_state_, y);
  lua_pcall(lua_state_, 2, 1, 0);
  double res = lua_tonumber(lua_state_, -1);
  lua_pop(lua_state_, 1);
  return res;
}

double fem::script::call(const double& x, const double& y, double t,
                         std::string func) {
  lua_getglobal(lua_state_, func.c_str());
  lua_pushnumber(lua_state_, x);
  lua_pushnumber(lua_state_, y);
  lua_pushnumber(lua_state_, t);
  lua_pcall(lua_state_, 3, 1, 0);
  double res = lua_tonumber(lua_state_, -1);
  lua_pop(lua_state_, 1);
  return res;
}

std::vector<double> fem::script::call(const std::vector<double>& x,
                                      const std::vector<double>& y, double t,
                                      std::string func) {
  std::vector<double> res;
  lua_getglobal(lua_state_, func.c_str());
  for (std::size_t i = 0; i < x.size() && i < y.size(); ++i) {
    lua_pushnumber(lua_state_, x[i]);
    lua_pushnumber(lua_state_, y[i]);
    lua_pushnumber(lua_state_, t);
    lua_call(lua_state_, 3, 1);
    res.push_back(lua_tonumber(lua_state_, -1));
  }
  lua_pop(lua_state_, 1);
  return res;
}

bool fem::script::close_script() {
  if (lua_state_ == nullptr) return false;
  lua_close(lua_state_);
  lua_state_ = nullptr;
  return true;
}
