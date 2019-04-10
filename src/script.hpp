#ifndef ARTA_SCRIPT_HPP_
#define ARTA_SCRIPT_HPP_

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

namespace arta {
namespace script {
  void load_script(const std::string& file);
  void close_script();
  bool valid();
  bool has(const std::string& var);
  std::string get_file_name();
}  // namespace script
}  // namespace arta

#endif  // ARTA_SCRIPT_HPP_
