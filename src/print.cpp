#include "print.hpp"

#include <string>

std::string fem::fmt_val(double val){
  char buff[255];
  snprintf(buff, 255, "%+015.10lf", val);
  return std::string(buff);
}
