#include "mesh/pslg.hpp"

#include <array>
#include <cmath>
#include <string>
#include <vector>

#include "util/rand.hpp"
#include "util/range.hpp"

#include <iostream>

std::array<double, 4> fem::mesh::Pslg::Bounds() const {
  std::array<double, 4> bounds = {{INFINITY, INFINITY, -INFINITY, -INFINITY}};
  for (auto& it : points) {
    bounds[0] = std::min(bounds[0], it[0]);
    bounds[1] = std::min(bounds[1], it[1]);
    bounds[2] = std::max(bounds[2], it[0]);
    bounds[3] = std::max(bounds[3], it[1]);
  }
  return bounds;
}

void fem::mesh::Pslg::Scale() {
  std::array<double, 4> bounds = Bounds();
  double d_max = std::max(bounds[2] - bounds[0], bounds[3] - bounds[1]);
  for (auto& it : points) {
    it[0] = (it[0] - bounds[0]) / d_max;
    it[1] = (it[1] - bounds[1]) / d_max;
  }
  for (auto& it : holes) {
    it[0] = (it[0] - bounds[0]) / d_max;
    it[1] = (it[1] - bounds[1]) / d_max;
  }
}

fem::mesh::Pslg fem::mesh::operator*(const Pslg& lhs, const double& rhs) {
  Pslg out;
  for (auto& it : lhs.points) {
    out.points.push_back({{it[0] * rhs, it[1] * rhs}});
  }
  for (auto& it : lhs.holes) {
    out.holes.push_back({{it[0] * rhs, it[1] * rhs}});
  }
  out.edges = lhs.edges;
  return out;
}

fem::mesh::Pslg fem::mesh::operator+(const Pslg& lhs, const double& rhs) {
  Pslg out;
  for (auto& it : lhs.points) {
    out.points.push_back({{it[0] + rhs, it[1] + rhs}});
  }
  for (auto& it : lhs.holes) {
    out.holes.push_back({{it[0] + rhs, it[1] + rhs}});
  }
  out.edges = lhs.edges;
  return out;
}

fem::mesh::Pslg fem::mesh::Random(const std::size_t& points,
                                  const std::size_t& edges) {
  Pslg pslg;
  std::random_device rand_dev;
  std::mt19937 gen(rand_dev());
  std::uniform_real_distribution<double> point_dist(0.0, 1.0);
  std::uniform_int_distribution<std::size_t> edge_dist(0, points);
  for (std::size_t i = 0; i < points; ++i) {
    pslg.points.push_back({{point_dist(gen), point_dist(gen)}});
  }
  for (std::size_t i = 0; i < edges; ++i) {
    pslg.edges.push_back({{edge_dist(gen), edge_dist(gen)}});
  }
  return pslg;
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
  pslg.Scale();
  return pslg;
}

fem::mesh::Pslg fem::mesh::LoadSvgToPslg(const std::string& file) {
  Pslg pslg;
  std::vector<std::array<double, 4>> ellipse;
  std::vector<std::array<double, 4>> rects;
  std::vector<std::vector<std::array<double, 2>>> polygons;
  std::vector<std::array<double, 2>> holes;
  FILE* load = fopen(file.c_str(), "r");
  uint64_t width_, height_;
  if (!load) {
    return pslg;
  }
  fscanf(load, "<svg width=\"%lu\" height=\"%lu\" >", &width_, &height_);
  std::string line = "";
  uint8_t state = 0;
  while (line != "</svg>") {
    char buff[256];
    fscanf(load, "%255s", buff);
    std::string line(buff);
    if (line.find_first_not_of(" \t") == std::string::npos) {
      continue;
    }
    line = line.substr(line.find_first_not_of(" \t"),
                       line.size() - line.find_first_not_of(" \t"));
    if (line == "</svg>")
      break;
    else if (line == "/>") {
      if(state == 6){
        polygons.back().push_back(polygons.back()[0]);
      }
      state = 0;
      continue;
    }
    switch (state) {
      case 0:
        if (line == "<!--") {
          state = 1;
        } else if (line == "<circle") {
          ellipse.push_back(std::array<double, 4>());
          state = 2;
        } else if (line == "<rect") {
          rects.push_back(std::array<double, 4>());
          state = 3;
        } else if (line == "<ellipse") {
          ellipse.push_back(std::array<double, 4>());
          state = 4;
        } else if (line == "<point") {
          holes.push_back(std::array<double, 2>());
          state = 5;
        } else if (line == "<polygon") {
          polygons.push_back({});
          state = 6;
        } else if (line == "<polyline") {
          polygons.push_back({});
          state = 7;
        }
        break;
      case 1:
        if (line == "-->") {
          state = 0;
        }
        break;
      case 2:
      case 4:
        if (!line.compare(0, 3, "cx=")) {
          sscanf(line.c_str(), "cx=\"%lf\"", &ellipse.back()[0]);
        } else if (!line.compare(0, 3, "cy=")) {
          sscanf(line.c_str(), "cy=\"%lf\"", &ellipse.back()[1]);
        } else if (!line.compare(0, 2, "r=")) {
          sscanf(line.c_str(), "r=\"%lf\"", &ellipse.back()[2]);
          ellipse.back()[3] = ellipse.back()[2];
        } else if (!line.compare(0, 3, "rx=")) {
          sscanf(line.c_str(), "rx=\"%lf\"", &ellipse.back()[2]);
        } else if (!line.compare(0, 3, "ry=")) {
          sscanf(line.c_str(), "ry=\"%lf\"", &ellipse.back()[3]);
        }
        break;
      case 3:
        if (!line.compare(0, 2, "x=")) {
          sscanf(line.c_str(), "x=\"%lf\"", &rects.back()[0]);
        } else if (!line.compare(0, 2, "y=")) {
          sscanf(line.c_str(), "y=\"%lf\"", &rects.back()[1]);
        } else if (!line.compare(0, 6, "width=")) {
          sscanf(line.c_str(), "width=\"%lf\"", &rects.back()[2]);
        } else if (!line.compare(0, 7, "height=")) {
          sscanf(line.c_str(), "height=\"%lf\"", &rects.back()[3]);
        }
        break;
      case 5:
        if (!line.compare(0, 2, "x=")) {
          sscanf(line.c_str(), "x=\"%lf\"", &holes.back()[0]);
        } else if (!line.compare(0, 2, "y=")) {
          sscanf(line.c_str(), "y=\"%lf\"", &holes.back()[1]);
        }
        break;
      case 6:
      case 7:
        if (!line.compare(0, 7, "points=")) {
          polygons.back().push_back(std::array<double, 2>());
          sscanf(line.c_str(), "points=\"%lf,%lf", &polygons.back().back()[0],
                 &polygons.back().back()[1]);
          state += 2;
        }
        break;
      case 8:
      case 9:
        polygons.back().push_back(std::array<double, 2>());
        if (line.back() == '"') {
          sscanf(line.c_str(), "%lf,%lf\"", &polygons.back().back()[0],
                 &polygons.back().back()[1]);
          state -= 2;
        } else {
          sscanf(line.c_str(), "%lf,%lf", &polygons.back().back()[0],
                 &polygons.back().back()[1]);
        }
        break;
    }
  }
  pslg.holes = holes;
  for (auto& it : rects) {
    pslg.points.push_back({{it[0], it[1]}});
    pslg.points.push_back({{it[0] + it[2], it[1]}});
    pslg.points.push_back({{it[0], it[1] + it[3]}});
    pslg.points.push_back({{it[0] + it[2], it[1] + it[3]}});
    pslg.edges.push_back({{pslg.points.size() - 4, pslg.points.size() - 3}});
    pslg.edges.push_back({{pslg.points.size() - 4, pslg.points.size() - 2}});
    pslg.edges.push_back({{pslg.points.size() - 3, pslg.points.size() - 1}});
    pslg.edges.push_back({{pslg.points.size() - 2, pslg.points.size() - 1}});
  }
  for (auto& it : polygons) {
    for (std::size_t i = 0; i < it.size() - 1; ++i) {
      std::cout << it[i][0] << ',' << it[i][1] << ' ';
      pslg.points.push_back(it[i]);
      pslg.edges.push_back({{pslg.points.size() - 1, pslg.points.size()}});
    }
    pslg.points.push_back(it.back());
  }
  std::cout << ellipse.size() << rects.size() << holes.size() << polygons.size()
            << "\n";
  fclose(load);
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
