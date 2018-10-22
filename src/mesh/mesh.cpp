#include "mesh/mesh.hpp"

#include <array>
#include <string>
#include <vector>

#include <iostream>

bool fem::mesh::Contains(const std::array<std::size_t, 2>& edge,
                         const std::size_t& vertex) {
  return edge[0] == vertex || edge[1] == vertex;
}
bool fem::mesh::Contains(const std::array<std::size_t, 3>& tri,
                         const std::size_t& vertex) {
  return tri[0] == vertex || tri[1] == vertex || tri[2] == vertex;
}
bool fem::mesh::Contains(const std::array<std::size_t, 3>& tri,
                         const std::array<std::size_t, 2>& edge) {
  return (tri[0] == edge[0] && tri[1] == edge[1]) ||
         (tri[1] == edge[0] && tri[2] == edge[1]) ||
         (tri[2] == edge[0] && tri[0] == edge[1]) ||
         (tri[0] == edge[1] && tri[1] == edge[0]) ||
         (tri[1] == edge[1] && tri[2] == edge[0]) ||
         (tri[2] == edge[1] && tri[0] == edge[0]);
}

fem::mesh::Mesh::Mesh() {}
fem::mesh::Mesh::Mesh(const std::vector<std::array<double, 2>>& points,
                      const std::vector<std::array<std::size_t, 3>>& triangles)
    : points(points), triangles(triangles) {
  DeterminEdges();
}

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
bool fem::mesh::Mesh::CircumCircle(const std::size_t& tri,
                                   const std::array<double, 2>& pt) const {
  double x13 = points[triangles[tri][0]][0] - points[triangles[tri][2]][0];
  double y13 = points[triangles[tri][0]][1] - points[triangles[tri][2]][1];
  double x23 = points[triangles[tri][1]][0] - points[triangles[tri][2]][0];
  double y23 = points[triangles[tri][1]][1] - points[triangles[tri][2]][1];
  double x1p = points[triangles[tri][0]][0] - pt[0];
  double y1p = points[triangles[tri][0]][1] - pt[1];
  double x2p = points[triangles[tri][1]][0] - pt[0];
  double y2p = points[triangles[tri][1]][1] - pt[1];
  return (x13 * x23 + y13 * y23) * (x2p * y1p - x1p * y2p) <
         (x23 * y13 - x13 * y23) * (x2p * x1p + y1p * y2p);
}

bool fem::mesh::Mesh::CircumCircle(const std::size_t& tri,
                                   const std::size_t& pt) const {
  return CircumCircle(tri, points[pt]);
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
  mesh.DeterminEdges();
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
