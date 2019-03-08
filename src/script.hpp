#ifndef FEM_SCRIPT_HPP_
#define FEM_SCRIPT_HPP_

extern "C" {
#include <lua5.3/lauxlib.h>
#include <lua5.3/lua.h>
#include <lua5.3/lualib.h>
}

#include <string>
#include <vector>

namespace fem {
namespace script {
  bool open_script(const std::string& file_name);
  double call(const double& x, const double& y, std::string func = "func");
  double call(const double& x, const double& y, double t,
              std::string func = "func");
  std::vector<double> call(const std::vector<double>& x,
                           const std::vector<double>& y, double t = 0,
                           std::string func = "func");
  bool close_script();
}  // namespace script
}  // namespace fem

#endif  // FEM_SCRIPT_HPP_
