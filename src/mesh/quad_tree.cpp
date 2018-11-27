#include "mesh/quad_tree.hpp"

#include <array>
#include <bitset>
#include <cmath>
#include <memory>
#include <vector>

fem::mesh::QuadTree::QuadTree() {}
fem::mesh::QuadTree::QuadTree(const QuadTree& copy)
    : divider_(copy.divider_),
      children_(copy.children_),
      points_(copy.points_),
      triangles_(copy.triangles_) {}

fem::mesh::QuadTree::QuadTree(
    const std::vector<std::array<double, 2>>& pts,
    const std::vector<std::array<std::size_t, 3>>& tris, std::size_t cnt) {
  std::array<double, 2> x_range = {{INFINITY, -INFINITY}};
  std::array<double, 2> y_range = {{INFINITY, -INFINITY}};
  for (auto& pt : pts) {
    x_range[0] = std::min(x_range[0], pt[0]);
    x_range[1] = std::max(x_range[1], pt[0]);
    y_range[0] = std::min(y_range[0], pt[1]);
    y_range[1] = std::max(y_range[1], pt[1]);
  }
  ConstructTree(pts, tris, x_range, y_range, cnt);
}
fem::mesh::QuadTree::QuadTree(
    const std::vector<std::array<double, 2>>& pts,
    const std::vector<std::array<std::size_t, 3>>& tris,
    const std::array<double, 2>& x_range, const std::array<double, 2>& y_range,
    std::size_t cnt) {
  ConstructTree(pts, tris, x_range, y_range, cnt);
}

void fem::mesh::QuadTree::ConstructTree(
    const std::vector<std::array<double, 2>>& points,
    const std::vector<std::array<std::size_t, 3>>& triangles,
    const std::array<double, 2>& x_range, const std::array<double, 2>& y_range,
    std::size_t cnt) {
  if (cnt == 0) {
    return;
    points_ = points;
    triangles_ = triangles;
  }
  divider_[0] = (x_range[1] + x_range[0]) / 2.0;
  divider_[1] = (y_range[1] + y_range[0]) / 2.0;
  std::array<std::vector<std::size_t>, 4> quad_saved_points;
  std::array<std::vector<std::array<double, 2>>, 4> quad_points;
  std::array<std::vector<std::array<std::size_t, 3>>, 4> quad_triangles;
  for (auto& tri : triangles) {
    std::bitset<4> quad;
    for (auto& pt : tri) {
      for (uint8_t i = 0; i < 4; ++i) {
        quad[i] = InQuad(i, points[pt]);
      }
    }
    // Add triangle and point to every quad with atleast one of its verticies
  }
}

int8_t fem::mesh::QuadTree::FindQuad(const std::array<double, 2>& pt) const {
  if (triangles_.size() == 0) {
    if (pt[0] <= divider_[0]) {
      if (pt[1] <= divider_[1]) {
        return 1;
      } else {
        return 2;
      }
    } else {
      if (pt[1] <= divider_[1]) {
        return 3;
      } else {
        return 4;
      }
    }
  } else {
    return -1;
  }
}

bool fem::mesh::QuadTree::InQuad(uint8_t quad,
                                 const std::array<double, 2>& pt) const {
  if (quad == 0) {
    if (pt[0] <= divider_[0] && pt[1] <= divider_[1]) return true;
    return false;
  } else if (quad == 1) {
    if (pt[0] <= divider_[0] && !(pt[1] <= divider_[1])) return true;
    return false;
  } else if (quad == 2) {
    if (!(pt[0] <= divider_[0]) && pt[1] <= divider_[1]) return true;
    return false;
  } else if (quad == 3) {
    if (!(pt[0] <= divider_[0]) && !(pt[1] <= divider_[1])) return true;
    return false;
  }
}
