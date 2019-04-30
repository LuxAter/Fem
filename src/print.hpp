#ifndef ARTA_PRINT_HPP_
#define ARTA_PRINT_HPP_

#include <cstdio>
#include <string>

namespace arta {
inline std::string fmt_val(double val) {
  char buff[255];
  snprintf(buff, 255, "%+015.10lf", val);
  return std::string(buff);
}
inline std::string fmt_val(unsigned val) {
  char buff[255];
  snprintf(buff, 255, "%04u", val);
  return std::string(buff);
}
}  // namespace arta

#endif  // ARTA_PRINT_HPP_
