#include "file.hpp"

#include <sys/stat.h>
#include <unistd.h>
#include <string>

bool fem::file_exists(const std::string& file_path) {
  return (access(file_path.c_str(), F_OK) != -1);
}
