#ifndef FEM_UTIL_TO_STRING_HPP_
#define FEM_UTIL_TO_STRING_HPP_

#include <string>
#include <cstdio>

namespace fem {
  std::string to_string(const float& val, unsigned perc = 8);
  std::string to_string(const double& val, unsigned perc = 16);
  std::string to_string(const long double& val, unsigned perc = 20);
  std::string to_hex(const unsigned& val);
  std::string to_hex(const unsigned long& val);
} // namespace fem

#endif  // FEM_UTIL_TO_STRING_HPP_
