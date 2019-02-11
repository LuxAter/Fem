#include "mesh/pslg.hpp"

#include <cstdio>
#include <cstdlib>
#include <vector>

#include "util/util.hpp"

void fem::SaveToFile(const std::string& file_name, const PSLG& pslg) {
  FILE* out = std::fopen(file_name.c_str(), "w");
  fprintf(out, "%lu\n", pslg.points.size());
  for (auto& it : pslg.points) {
    fprintf(out, "%lf %lf", it.x, it.y);
  }
  fprintf(out, "\n%lu\n", pslg.edges.size());
  for (auto& it : pslg.edges) {
    fprintf(out, "%lu %lu", it.x, it.y);
  }
  fprintf(out, "\n%lu\n", pslg.holes.size());
  for (auto& it : pslg.holes) {
    fprintf(out, "%lf %lf", it.x, it.y);
  }
  std::fclose(out);
}

fem::PSLG fem::LoadPslgFromFile(const std::string& file_name) {
  PSLG res;
  FILE* src = std::fopen(file_name.c_str(), "r");
  unsigned long n = 0;
  fscanf(src, "%lu", &n);
  for (unsigned long i = 0; i < n; ++i) {
    double px, py;
    fscanf(src, "%lf %lf", &px, &py);
    res.points.push_back({px, py});
  }
  fscanf(src, "%lu", &n);
  for (unsigned long i = 0; i < n; ++i) {
    unsigned long ex, ey;
    fscanf(src, "%lu %lu", &ex, &ey);
    res.edges.push_back({ex, ey});
  }
  fscanf(src, "%lu", &n);
  for (unsigned long i = 0; i < n; ++i) {
    double px, py;
    fscanf(src, "%lf %lf", &px, &py);
    res.holes.push_back({px, py});
  }
  std::fclose(src);
  return res;
}
