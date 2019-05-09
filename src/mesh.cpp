#include "mesh.hpp"

#include <cmath>
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
    fscanf(input, "%ld %ld %ld %ld", &a, &y, &z, &x);
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

double arta::mesh::Mesh::grain_size(const unsigned& e) const {
  double x1 = pts[tri[e][0]].x, y1 = pts[tri[e][0]].y;
  double x2 = pts[tri[e][1]].x, y2 = pts[tri[e][1]].y;
  double x3 = pts[tri[e][2]].x, y3 = pts[tri[e][2]].y;
  return fabs((x1 * (y2 - y3) + x2 * (y3 - y1) + x3 * (y1 - y2)) / 2.0) / 8;
}

int arta::mesh::Mesh::locate(const double& x, const double& y) const {
  if (has_holes_ == false) {
    int t = tri.size() - 1;
    bool searching = true;
    while (searching == true) {
      bool within = true;
      for (unsigned long i = 0; i < 3 && within == true; ++i) {
        unsigned long v1 = tri[t][i];
        unsigned long v2 = tri[t][(i + 1) % 3];
        if ((pts[v1].y - y) * (pts[v2].x - x) >
            (pts[v1].x - x) * (pts[v2].y - y)) {
          t = adj[t][i];
          within = false;
        }
      }
      if (within == true || t == -1) {
        searching = false;
      }
    }
    return t;
  } else {
    bool searching = true;
    for (unsigned long t = 0; t < tri.size() && searching == true; ++t) {
      bool within = true;
      for (unsigned long i = 0; i < 3 && within == true; ++i) {
        unsigned long v1 = tri[t][i];
        unsigned long v2 = tri[t][(i + 1) % 3];
        if ((pts[v1].y - y) * (pts[v2].x - x) >
            (pts[v1].x - x) * (pts[v2].y - y)) {
          within = false;
        }
      }
      if (within == true) {
        return t;
      }
    }
    return -1;
  }
}

bool arta::mesh::Mesh::is_boundary(const unsigned& e) const {
  return bdry_index[e] != 0;
}

void arta::mesh::construct_mesh(const std::string& source,
                                const std::string& dest, const double& area,
                                const double& angle) {
  std::string command = "../thirdparty/triangle/triangle -pzn -a" +
                        std::to_string(area) + " -q" + std::to_string(angle) +
                        " " + source + " >/dev/null";
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
