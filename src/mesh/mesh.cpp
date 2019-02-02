#include "mesh/mesh.hpp"

#include <array>
#include <string>
#include <vector>
#include <cmath>

#include <iostream>

fem::mesh::Mesh::Mesh() {}
fem::mesh::Mesh::Mesh(const Mesh& copy)
    : points(copy.points), triangles(copy.triangles) {}
fem::mesh::Mesh::Mesh(const std::vector<std::array<double, 2>>& points,
                      const std::vector<std::array<std::size_t, 3>>& triangles)
    : points(points), triangles(triangles) {}
fem::mesh::Mesh::Mesh(const std::vector<std::array<double, 2>>& points,
                      const std::vector<std::array<std::size_t, 3>>& triangles,
                      const std::vector<std::array<std::size_t, 3>>& adj)
    : points(points), triangles(triangles), adjacency(adj) {}

void fem::mesh::Mesh::DeterminEdges() {
  for (auto& tri : triangles) {
    std::array<std::size_t, 2> a = {{tri[0], tri[1]}}, b = {{tri[1], tri[2]}},
                               c = {{tri[2], tri[0]}}, ap = {{tri[1], tri[0]}},
                               bp = {{tri[2], tri[1]}}, cp = {{tri[0], tri[2]}};
    bool a_in = false, b_in = false, c_in = false;
    for (std::size_t i = 0; i < edges.size() && (!a_in || !b_in || !c_in);
         i++) {
      if (edges[i] == a || edges[i] == ap) {
        a_in = true;
      } else if (edges[i] == b || edges[i] == bp) {
        b_in = true;
      } else if (edges[i] == c || edges[i] == cp) {
        c_in = true;
      }
    }
    if (!a_in) edges.push_back(a);
    if (!b_in) edges.push_back(b);
    if (!c_in) edges.push_back(c);
  }
}

bool fem::mesh::Mesh::InMesh(const std::array<double, 2>& pt) const {
  return true;
  for (auto& tri : triangles) {
    double Area =
        0.5 * (-points[tri[1]][1] * points[tri[2]][0] +
               points[tri[0]][1] * (-points[tri[1]][0] + points[tri[2]][0]) +
               points[tri[0]][0] * (points[tri[1]][1] - points[tri[2]][1]) +
               points[tri[1]][0] * points[tri[2]][1]);
    double s = 1 / (2 * Area) *
               (points[tri[0]][1] * points[tri[2]][0] -
                points[tri[0]][0] * points[tri[2]][1] +
                (points[tri[2]][1] - points[tri[0]][1]) * pt[0] +
                (points[tri[0]][0] - points[tri[2]][0]) * pt[1]);
    double t = 1 / (2 * Area) *
               (points[tri[0]][0] * points[tri[1]][1] -
                points[tri[0]][1] * points[tri[1]][0] +
                (points[tri[0]][1] - points[tri[1]][1]) * pt[0] +
                (points[tri[1]][0] - points[tri[0]][0]) * pt[1]);
    if (s > 0 && t > 0 && (1 - s - t) > 0) {
      return true;
    }
  }
  return false;
  // TODO Use this mesh traversal search, it is much better!
  //
  //
  // uint64_t tri = triangles.size() - 1;
  // bool searching = true;
  // while (searching == true) {
  //   bool within = true;
  //   for (uint64_t i = 0; i < 3; ++i) {
  //     uint64_t v1 = triangles[tri][i];
  //     uint64_t v2 = triangles[tri][(i + 1) % 3];
  //     if ((points[v1][1] - pt[1]) * (points[v2][0] - pt[0]) >
  //         (points[v1][0] - pt[0]) * (points[v2][1] - pt[1])) {
  //       tri = adjacency[tri][i];
  //       within = false;
  //     }
  //   }
  //   if (within == true || tri == 0) {
  //     searching = false;
  //   }
  // }
  // return (tri == 0) ? false : true;
}

fem::mesh::Mesh fem::mesh::operator*(const Mesh& lhs, const double& rhs) {
  Mesh mesh = lhs;
  for (auto& it : mesh.points) {
    it[0] *= rhs;
    it[1] *= rhs;
  }
  return mesh;
}
fem::mesh::Mesh fem::mesh::operator+(const Mesh& lhs, const double& rhs) {
  Mesh mesh = lhs;
  for (auto& it : mesh.points) {
    it[0] += rhs;
    it[1] += rhs;
  }
  return mesh;
}

fem::mesh::Mesh fem::mesh::LoadMesh(const std::string& file) {
  Mesh mesh;
  FILE* load = fopen(file.c_str(), "r");
  if (!load) {
    return mesh;
  }
  std::size_t points, triangles;
  fscanf(load, "%lu", &points);
  for (std::size_t i = 0; i < points; ++i) {
    double x, y;
    fscanf(load, "%lf %lf", &x, &y);
    mesh.points.push_back(std::array<double, 2>{{x, y}});
  }
  fscanf(load, "%lu", &triangles);
  for (std::size_t i = 0; i < triangles; ++i) {
    std::size_t a, b, c;
    fscanf(load, "%lu %lu %lu", &a, &b, &c);
    mesh.triangles.push_back(std::array<std::size_t, 3>{{a, b, c}});
  }
  fclose(load);
  return mesh;
}

void fem::mesh::SaveMesh(const std::string& file, Mesh mesh) {
  FILE* out = fopen(file.c_str(), "w");
  if (!out) {
    return;
  }
  fprintf(out, "%lu\n", mesh.points.size());
  for (auto& it : mesh.points) {
    fprintf(out, "%lf %lf\n", it[0], it[1]);
  }
  fprintf(out, "%lu\n", mesh.triangles.size());
  for (auto& it : mesh.triangles) {
    fprintf(out, "%lu %lu %lu\n", it[0], it[1], it[2]);
  }
  fclose(out);
}

fem::mesh::Mesh fem::mesh::LoadTriangleMesh(const std::string& name) {
  Mesh mesh;
  FILE* node = fopen((name + ".node").c_str(), "r");
  if (!node) {
    fprintf(stderr, "Failed to load \"%s\"\n", (name + ".node").c_str());
    return mesh;
  }
  uint64_t n, d, a, b, i_bit;
  fscanf(node, "%lu %lu %lu %lu", &n, &d, &a, &b);
  for (; n > 0; --n) {
    double x, y, d_bit;
    fscanf(node, "%lu %lf %lf %lf %lu", &i_bit, &x, &y, &d_bit, &b);
    mesh.points.push_back({{x, y}});
  }
  fclose(node);
  FILE* ele = fopen((name + ".ele").c_str(), "r");
  if (!ele) {
    fprintf(stderr, "Failed to load \"%s\"\n", (name + ".ele").c_str());
    return mesh;
  }
  fscanf(ele, "%lu %lu %lu", &n, &d, &a);
  for (; n > 0; --n) {
    uint64_t a, b, c;
    fscanf(ele, "%lu %lu %lu %lu", &i_bit, &a, &b, &c);
    mesh.triangles.push_back({{a - 1, b - 1, c - 1}});
  }
  fclose(ele);
  return mesh;
}


fem::mesh::Mesh fem::mesh::Flip(Mesh mesh){
  std::array<double, 2>range_y = {{INFINITY, -INFINITY}};
  for(auto& pt : mesh.points){
    range_y[0] = std::min(range_y[0], pt[1]);
    range_y[1] = std::max(range_y[1], pt[1]);
  }
  for(auto& pt : mesh.points){
    pt[1] = range_y[1] - (pt[1] - range_y[0]);
  }
  return mesh;
}
