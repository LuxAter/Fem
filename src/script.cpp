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
  }else{
    file_name_ = file;
    log::success("Loaded script file \"%s\"", file.c_str());
  }
}

void arta::script::close_script() {
  if (state_ != NULL) {
    lua_close(state_);
    state_ = NULL;
    log::success("Closed script file \"%s\"", file_name_.c_str());
  }else{
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
