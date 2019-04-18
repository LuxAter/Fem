#include "mesh.hpp"

#include <cstdlib>
#include <string>

#include <sys/stat.h>
#include <sys/types.h>

#include "logger.hpp"
#include "script.hpp"

arta::mesh::Mesh::Mesh() {}
arta::mesh::Mesh::Mesh(const std::string& base_name) {
  FILE* input = fopen((base_name + ".node").c_str(), "r");
  if (!input) {
    log::error("Failed to open \"%s.node\"", base_name.c_str());
    return;
  }
  long n, a, b, c;
  fscanf(input, "%ld %ld %ld %ld", &n, &a, &b, &c);
  for (long i = 0; i < n; ++i) {
    double x, y;
    fscanf(input, "%ld %lf %lf %ld", &a, &x, &y, &c);
    pts.push_back({x, y});
    bounds[0] = std::min(bounds[0], x);
    bounds[1] = std::min(bounds[1], y);
    bounds[2] = std::max(bounds[2], x);
    bounds[3] = std::max(bounds[3], y);
    bdry_index.push_back(c);
  }
  fclose(input);

  input = fopen((base_name + ".ele").c_str(), "r");
  if (!input) {
    log::error("Failed to open \"%s.ele\"", base_name.c_str());
    return;
  }
  fscanf(input, "%ld %ld %ld", &n, &a, &b);
  for (long i = 0; i < n; ++i) {
    long x, y, z;
    fscanf(input, "%ld %ld %ld %ld", &a, &x, &y, &z);
    tri.push_back({x, y, z});
  }
  fclose(input);

  input = fopen((base_name + ".neigh").c_str(), "r");
  if (!input) {
    log::error("Failed to open \"%s.neigh\"", base_name.c_str());
    return;
  }
  fscanf(input, "%ld %ld", &n, &a);
  for (long i = 0; i < n; ++i) {
    long x, y, z;
    fscanf(input, "%ld %ld %ld %ld", &a, &x, &y, &z);
    adj.push_back({x, y, z});
  }
  fclose(input);

  input = fopen((base_name + ".poly").c_str(), "r");
  if (!input) {
    log::error("Failed to open \"%s.poly\"", base_name.c_str());
    return;
  }
  fscanf(input, "%ld %ld %ld %ld", &n, &a, &b, &c);
  for (long i = 0; i < n; ++i) {
    double x, y;
    fscanf(input, "%ld %lf %lf %ld", &a, &x, &y, &b);
  }
  fscanf(input, "%ld %ld", &n, &a);
  for (long i = 0; i < n; ++i) {
    long d;
    fscanf(input, "%ld %ld %ld %ld", &a, &b, &c, &d);
  }
  fscanf(input, "%ld", &n);
  if (n != 0) {
    has_holes_ = true;
  } else {
    has_holes_ = false;
  }
  fclose(input);
}

arta::mesh::Mesh::Mesh(const Mesh& copy)
    : pts(copy.pts),
      bdry_index(copy.bdry_index),
      tri(copy.tri),
      adj(copy.adj),
      bounds(copy.bounds),
      has_holes_(copy.has_holes_) {}

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
