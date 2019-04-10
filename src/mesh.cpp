#include "mesh.hpp"

#include <cstdlib>
#include <string>

#include <sys/stat.h>
#include <sys/types.h>

#include "logger.hpp"
#include "script.hpp"

void arta::mesh::construct_mesh(const std::string& source,
                                const std::string& dest, const double& area,
                                const double& angle) {
  std::string command = "../triangle -pzn -a" + std::to_string(area) + " -q" +
                        std::to_string(angle) + " " + source + " >/dev/null";
  log::info("Executing \"%s\"", command.c_str());
  if (std::system(command.c_str()) != 0) {
    log::warning("Failed to execute \"%s\"", command.c_str());
  }
  struct stat st = {0};
  if (stat(dest.c_str(), &st) == -1) {
    log::info("Creating directory \"%s\"", dest.c_str());
    mkdir(dest.c_str(), 0700);
  }
  std::string file_name(source);
  std::string base_name(source);
  file_name.erase(file_name.rfind('.'));
  base_name.erase(0, base_name.find_last_of("\\/") + 1);
  base_name.erase(base_name.rfind('.'));
  std::rename((file_name + ".1.ele").c_str(),
              (dest + base_name + ".ele").c_str());
  std::rename((file_name + ".1.neigh").c_str(),
              (dest + base_name + ".neigh").c_str());
  std::rename((file_name + ".1.node").c_str(),
              (dest + base_name + ".node").c_str());
  std::rename((file_name + ".1.poly").c_str(),
              (dest + base_name + ".poly").c_str());
}
