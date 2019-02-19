#include "mesh/mesh_class.hpp"
#include "util/util.hpp"

#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <map>
#include <string>
#include <vector>

unsigned long fem::TriLoc(const Pt& p, const Mesh& mesh) {
  unsigned long t = mesh.tri.size() - 1;
  bool searching = true;
  while (searching == true) {
    bool within = true;
    for (unsigned long i = 0; i < 3; ++i) {
      unsigned long v1 = mesh.tri[t][i];
      unsigned long v2 = mesh.tri[t][(i + 1) % 3];
      if ((mesh.pts[v1].y - p.y) * (mesh.pts[v2].x - p.x) >
          (mesh.pts[v1].x - p.x) * (mesh.pts[v2].y - p.y)) {
        t = mesh.adj[t][i];
        within = false;
      }
    }
    if (within == true || t >= mesh.tri.size()) {
      searching = false;
    }
  }
  return t;
}

fem::Mesh fem::LoadMesh(const std::string& file_name) {
  FILE* src = std::fopen(file_name.c_str(), "r");
  unsigned num;
  unsigned n_attrs;
  Mesh mesh;
  std::fscanf(src, "%u %u", &num, &n_attrs);
  for (unsigned i = 0; i < num; ++i) {
    double x, y;
    std::fscanf(src, "%lf %lf", &x, &y);
    std::vector<double> attrs;
    for (unsigned j = 0; j < n_attrs; ++j) {
      double attr;
      std::fscanf(src, "%lf", &attr);
      attrs.push_back(attr);
    }
    mesh.pts.push_back({x, y});
    if (!std::all_of(attrs.begin(), attrs.end(),
                     [](double x) { return x == 0.0; })) {
      mesh.bc[mesh.pts.size() - 1] = attrs;
    }
  }
  std::fscanf(src, "%u", &num);
  for (unsigned i = 0; i < num; ++i) {
    unsigned a, b, c;
    std::fscanf(src, "%u %u %u", &a, &b, &c);
    mesh.tri.push_back({a, b, c});
    mesh.adj.push_back({num, num, num});
    for (unsigned j = 0; j < 3; ++j) {
      unsigned tmp;
      if (std::fscanf(src, "%u", &tmp) != 0) {
        mesh.adj.back()[j] = tmp;
      } else {
        std::fscanf(src, "_");
      }
    }
  }

  std::fclose(src);
  return mesh;
}
