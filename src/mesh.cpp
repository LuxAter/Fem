#include "mesh.hpp"

#include <unistd.h>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>

#include <iostream>

#include "geometry.hpp"
#include "script.hpp"
#include "logger.hpp"

fem::mesh::Mesh::Mesh(const std::string& file_path) {
  bounds = {{INFINITY, INFINITY, -INFINITY, -INFINITY}};
  FILE* input = fopen((file_path + ".node").c_str(), "r");
  if(!input){
    log::warning("Failed to open \"%s.node", file_path.c_str());
    return;
  }
  int n, a, b, c;
  fscanf(input, "%d %d %d %d", &n, &a, &b, &c);
  for (int i = 0; i < n; ++i) {
    double x, y;
    fscanf(input, "%d %lf %lf %d", &a, &x, &y, &c);
    pts.push_back({x, y});
    bounds[0] = std::min(bounds[0], x);
    bounds[1] = std::min(bounds[1], y);
    bounds[2] = std::max(bounds[2], x);
    bounds[3] = std::max(bounds[3], y);
    boundary_index.push_back(c);
  }
  fclose(input);
  input = fopen((file_path + ".ele").c_str(), "r");
  if(!input){
    log::warning("Failed to open \"%s.ele", file_path.c_str());
    return;
  }
  fscanf(input, "%d %d %d", &n, &a, &b);
  for (int i = 0; i < n; ++i) {
    int x, y, z;
    fscanf(input, "%d %d %d %d", &a, &x, &y, &z);
    tri.push_back({x, y, z});
  }
  fclose(input);
  input = fopen((file_path + ".neigh").c_str(), "r");
  if(!input){
    log::warning("Failed to open \"%s.neigh", file_path.c_str());
    return;
  }
  fscanf(input, "%d %d", &n, &a);
  for (int i = 0; i < n; ++i) {
    int x, y, z;
    fscanf(input, "%d %d %d %d", &a, &y, &z, &x);
    adj.push_back({x, y, z});
  }
  fclose(input);
  input = fopen((file_path + ".poly").c_str(), "r");
  if(!input){
    log::warning("Failed to open \"%s.poly", file_path.c_str());
    return;
  }
  fscanf(input, "%d %d %d %d", &n, &a, &b, &c);
  for (int i = 0; i < n; ++i) {
    float x, y;
    fscanf(input, "%d %f %f %d", &a, &x, &y, &b);
  }
  fscanf(input, "%d %d", &n, &a);
  for (int i = 0; i < n; ++i) {
    int d;
    fscanf(input, "%d %d %d %d", &a, &b, &c, &d);
  }
  fscanf(input, "%d", &n);
  if (n != 0)
    has_holes = true;
  else
    has_holes = false;
  fclose(input);
  if (access((file_path + ".bc").c_str(), F_OK) != -1) {
    input = fopen((file_path + ".bc").c_str(), "r");
    fscanf(input, "%d", &n);
    bc = std::vector<std::string>(n + 1, "0.0");
    char buff[256];
    for (int i = 0; i < n; ++i) {
      int c;
      if ((c = fgetc(input)) == '#') {
        fscanf(input, "%d %255s", &a, buff);
        continue;
      }
      ungetc(c, input);
      fscanf(input, "%d %255s", &a, buff);
      bc[a] = buff;
    }
    fclose(input);
  }else{
    log::info("No boundary condition file specified.");
  }
}

int fem::mesh::Mesh::locate(const Pair<double>& pt) const {
  if (has_holes == false) {
    int t = tri.size() - 1;
    bool searching = true;
    while (searching == true) {
      bool within = true;
      for (unsigned long i = 0; i < 3 && within == true; ++i) {
        unsigned long v1 = tri[t][i];
        unsigned long v2 = tri[t][(i + 1) % 3];
        if ((pts[v1].y - pt.y) * (pts[v2].x - pt.x) >
            (pts[v1].x - pt.x) * (pts[v2].y - pt.y)) {
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
        if ((pts[v1].y - pt.y) * (pts[v2].x - pt.x) >
            (pts[v1].x - pt.x) * (pts[v2].y - pt.y)) {
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

double fem::mesh::Mesh::boundary(const unsigned long& point_index) const {
  if (bc.size() == 0) {
    return 0.0;
  }
  if (!bc[boundary_index[point_index]].compare(0, 7, "script.")) {
    return script::call(pts[point_index].x, pts[point_index].y,
                        bc[boundary_index[point_index]].substr(7));
  } else {
    return std::atof(bc[boundary_index[point_index]].c_str());
  }
}
