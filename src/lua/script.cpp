#include "lua/script.hpp"

extern "C" {
#include <lauxlib.h>
#include <lua.h>
#include <lualib.h>
}

static lua_State* lua_state_ = nullptr;

void fem::lua::OpenScript(const std::string& file_name) {
  lua_state_ = luaL_newstate();
  luaL_openlibs(lua_state_);
  if (luaL_loadfile(lua_state_, file_name.c_str()) || lua_pcall(lua_state_, 0, 0, 0)) {
    lua_state_ = nullptr;
  }
}

double fem::lua::Call(const double& x, const double& y, double t){
  lua_getglobal(lua_state_, "func");
  lua_pushnumber(lua_state_, x);
  lua_pushnumber(lua_state_, y);
  lua_pushnumber(lua_state_, t);
  lua_pcall(lua_state_, 3, 1, 0);
  double res = lua_tonumber(lua_state_, -1);
  lua_pop(lua_state_, 1);
  return res;
}

void fem::lua::CloseScript(){
  if (lua_state_ != nullptr){
    lua_close(lua_state_);
    lua_state_ = nullptr;
  }
}
