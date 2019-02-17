#include "mesh/mesh_class.hpp"
#include "util/util.hpp"

#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <map>
#include <string>
#include <vector>

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
  }

  std::fclose(src);
  return mesh;
}
