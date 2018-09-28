#ifndef FEM_MESH_DELAUNAY_HPP_
#define FEM_MESH_DELAUNAY_HPP_

#include <array>
#include <vector>

#include "../shape.hpp"

namespace fem {
namespace mesh {
  // std::vector<fem::shape::Triangle> Delaunay(
  //     const std::vector<fem::shape::Point>& points);
  bool RotationalLess(const std::size_t& center, const std::size_t& a,
                      const std::size_t& b,
                      const std::vector<std::array<double, 2>>& points);
  bool InCircumCircle(const std::size_t& pt,
                      const std::array<std::size_t, 3>& tri,
                      const std::vector<std::array<double, 2>>& points);
  std::vector<std::array<std::size_t, 3>> Delaunay(
      std::vector<std::array<double, 2>>& points);
}  // namespace mesh
}  // namespace fem

#endif  // FEM_MESH_DELAUNAY_HPP_
