#include "util/to_string.hpp"

#include <string>
#include <cstdio>

std::string fem::to_string(const float& val, unsigned perc){
  char buff[256];
  snprintf(buff, 255, "%.*f", perc, val);
  return std::string(buff);
}
std::string fem::to_string(const double& val, unsigned perc){
  char buff[256];
  snprintf(buff, 255, "%.*lf", perc, val);
  return std::string(buff);
}
std::string fem::to_string(const long double& val, unsigned perc){
  char buff[256];
  snprintf(buff, 255, "%.*Lf", perc, val);
  return std::string(buff);
}
std::string fem::to_hex(const unsigned& val){
  char buff[256];
  snprintf(buff, 255, "#%06x", val);
  return std::string(buff);
}
std::string fem::to_hex(const unsigned long& val){
  char buff[256];
  snprintf(buff, 255, "#%06lx", val);
  return std::string(buff);
}
