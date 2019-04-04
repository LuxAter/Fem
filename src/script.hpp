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
  class Script {
   public:
    Script();
    explicit Script(const std::string& file);

    inline bool valid() const { return valid_check_.use_count() != 0; }

    template <typename _T>
      _T get(const std::string& var);

    double initial(const Point& pt);
    double forcing(const Point& pt, );


   private:
    std::shared_ptr<int> valid_check_;
    lua_State* state_;

    std::string file_name_;
    std::function<void(std::string, std::string)> _error_;
  };
}  // namespace script
}  // namespace arta

#endif  // ARTA_SCRIPT_HPP_
