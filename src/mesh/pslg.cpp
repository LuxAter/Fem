#include "mesh/pslg.hpp"

#include <array>
#include <cmath>
#include <string>
#include <vector>

#include "util/rand.hpp"
#include "util/range.hpp"

#include <iostream>

void fem::mesh::Pslg::Center() {
  std::array<double, 2> range_x = {{INFINITY, -INFINITY}};
  std::array<double, 2> range_y = {{INFINITY, -INFINITY}};
  for (auto& it : points) {
    range_x[0] = std::min(range_x[0], it[0]);
    range_x[1] = std::max(range_x[1], it[0]);
    range_y[0] = std::min(range_y[0], it[1]);
    range_y[1] = std::max(range_y[1], it[1]);
  }
  std::array<double, 2> center = {
      {range_x[0] + (range_x[1] - range_x[0]) / 2.0,
       range_y[0] + (range_y[1] - range_y[0]) / 2.0}};
  for (auto& it : points) {
    it[0] -= center[0];
    it[1] -= center[1];
  }
  for (auto& it : holes) {
    it[0] -= center[0];
    it[1] -= center[1];
  }
}

void fem::mesh::Pslg::Scale() {
  std::array<double, 2> range_x = {{INFINITY, -INFINITY}};
  std::array<double, 2> range_y = {{INFINITY, -INFINITY}};
  for (auto& it : points) {
    range_x[0] = std::min(range_x[0], it[0]);
    range_x[1] = std::max(range_x[1], it[0]);
    range_y[0] = std::min(range_y[0], it[1]);
    range_y[1] = std::max(range_y[1], it[1]);
  }
  double scale_x, scale_y;
  scale_x = 2.0 / (range_x[1] - range_x[0]);
  scale_y = 2.0 / (range_y[1] - range_y[0]);
  double scale = std::min(scale_x, scale_y);
  for (auto& it : points) {
    it[0] *= scale;
    it[1] *= scale;
  }
  for (auto& it : holes) {
    it[0] *= scale;
    it[1] *= scale;
  }
}

void fem::mesh::Pslg::Center(double x, double y) {
  for (auto& it : points) {
    it[0] += x;
    it[1] += y;
  }
  for (auto& it : holes) {
    it[0] += x;
    it[1] += y;
  }
}

void fem::mesh::Pslg::Scale(double alpha) {
  for (auto& it : points) {
    it[0] *= alpha;
    it[1] *= alpha;
  }
  for (auto& it : holes) {
    it[0] *= alpha;
    it[1] *= alpha;
  }
}

bool fem::mesh::Pslg::Contains(std::array<double, 2> pt) {
  for (auto& it : points) {
    if (it == pt) return true;
  }
  return false;
}
bool fem::mesh::Pslg::Contains(std::array<std::size_t, 2> eg) {
  for (auto& it : edges) {
    if ((it[0] == eg[0] && it[1] == eg[1]) ||
        (it[1] == eg[0] && it[0] == eg[1]))
      return true;
  }
  return false;
}

std::vector<std::array<double, 2>> fem::mesh::GenPoints(const Pslg& pslg,
                                                        std::size_t scale) {
  std::vector<std::array<double, 2>> points = pslg.points;
  std::random_device rand_dev;
  std::mt19937 gen(rand_dev());
  std::array<double, 2> range_x = {{INFINITY, -INFINITY}};
  std::array<double, 2> range_y = {{INFINITY, -INFINITY}};
  for (auto& it : pslg.points) {
    range_x[0] = std::min(range_x[0], it[0]);
    range_x[1] = std::max(range_x[1], it[0]);
    range_y[0] = std::min(range_y[0], it[1]);
    range_y[1] = std::max(range_y[1], it[1]);
  }
  double ep = std::min((range_x[1] - range_x[0]) / scale,
                       (range_y[1] - range_y[0]) / scale);
  std::uniform_real_distribution<double> dist(0.0, ep);
  for (auto& y : util::StepRange(range_y[0] - ep, range_y[1] + ep, scale)) {
    for (auto& x : util::StepRange(range_x[0] - ep, range_x[1] + ep, scale)) {
      points.push_back({{x + dist(gen), y + dist(gen)}});
    }
  }
  return points;
}

fem::mesh::Pslg fem::mesh::LoadPslg(const std::string& file) {
  Pslg pslg;
  FILE* load = fopen(file.c_str(), "r");
  if (!load) {
    return pslg;
  }
  std::size_t points, edges, holes;
  fscanf(load, "%lu", &points);
  for (std::size_t i = 0; i < points; ++i) {
    double x, y;
    fscanf(load, "%lf %lf", &x, &y);
    pslg.points.push_back(std::array<double, 2>{{x, y}});
  }
  fscanf(load, "%lu", &edges);
  for (std::size_t i = 0; i < edges; ++i) {
    std::size_t a, b;
    fscanf(load, "%lu %lu", &a, &b);
    pslg.edges.push_back(std::array<std::size_t, 2>{{a - 1, b - 1}});
  }
  fscanf(load, "%lu", &holes);
  for (std::size_t i = 0; i < holes; ++i) {
    double x, y;
    fscanf(load, "%lf %lf", &x, &y);
    pslg.holes.push_back(std::array<double, 2>{{x, y}});
  }
  fclose(load);
  pslg.Center();
  pslg.Scale();
  return pslg;
}

void fem::mesh::SavePslg(const std::string& file, Pslg pslg) {
  FILE* out = fopen(file.c_str(), "w");
  if (!out) {
    return;
  }
  fprintf(out, "%lu\n", pslg.points.size());
  for (auto& it : pslg.points) {
    fprintf(out, "%lf %lf\n", it[0], it[1]);
  }
  fprintf(out, "%lu\n", pslg.edges.size());
  for (auto& it : pslg.edges) {
    fprintf(out, "%lu %lu\n", it[0], it[1]);
  }
  fprintf(out, "%lu\n", pslg.holes.size());
  for (auto& it : pslg.holes) {
    fprintf(out, "%lf %lf\n", it[0], it[1]);
  }
  fclose(out);
}
