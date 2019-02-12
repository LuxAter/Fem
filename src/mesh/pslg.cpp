#include "mesh/pslg.hpp"

#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <vector>

#include "util/util.hpp"

fem::PSLG fem::Normalize(const PSLG& pslg, double min_v, double max_v) {
  double x_min = INFINITY, x_max = -INFINITY, y_min = INFINITY,
         y_max = INFINITY;
  PSLG res;
  for (auto& pt : pslg.points) {
    x_min = std::min(x_min, pt.x);
    x_max = std::max(x_max, pt.x);
    y_min = std::min(y_min, pt.y);
    y_max = std::max(y_max, pt.y);
  }
  for (auto& pt : pslg.holes) {
    x_min = std::min(x_min, pt.x);
    x_max = std::max(x_max, pt.x);
    y_min = std::min(y_min, pt.y);
    y_max = std::max(y_max, pt.y);
  }
  double dmax = std::max(x_max - x_min, y_max - y_min) / (max_v - min_v);
  for (auto& pt : pslg.points) {
    res.points.push_back(
        {(pt.x - x_min + min_v) / dmax, (pt.y - y_min + min_v) / dmax});
  }
  for (auto& pt : pslg.holes) {
    res.holes.push_back(
        {(pt.x - x_min + min_v) / dmax, (pt.y - y_min + min_v) / dmax});
  }
  res.edges = pslg.edges;
  res._x_min = x_min;
  res._y_min = y_min;
  res._x_max = x_max;
  res._y_max = y_max;
  res._dmax = dmax;
  return res;
}

fem::PSLG fem::UnNormalize(const PSLG& pslg, double min_v, double max_v) {
  PSLG res;
  for (auto& pt : pslg.points) {
    res.points.push_back({pt.x * pslg._dmax + pslg._x_min - min_v,
                          pt.y * pslg._dmax + pslg._y_min - min_v});
  }
  for (auto& pt : pslg.holes) {
    res.holes.push_back({pt.x * pslg._dmax + pslg._x_min - min_v,
                         pt.y * pslg._dmax + pslg._y_min - min_v});
  }
  res.edges = pslg.edges;
  return res;
}

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
