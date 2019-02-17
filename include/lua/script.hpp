#ifndef FEM_LUA_SCRIPT_HPP_
#define FEM_LUA_SCRIPT_HPP_

extern "C"{
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}

#include <string>

namespace fem {
  namespace lua {
    void OpenScript(const std::string& file_name);
    double Call(const double& x, const double& y, double t = 0.0);
    void CloseScript();
  } // namespace lua
} // namespace fem

#endif  // FEM_LUA_SCRIPT_HPP_
