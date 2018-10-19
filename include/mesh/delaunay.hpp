#ifndef FEM_MESH_DELAUNAY_HPP_
#define FEM_MESH_DELAUNAY_HPP_

#include <array>
#include <vector>

#include "../mesh/mesh.hpp"
#include "../mesh/pslg.hpp"
#include "../shape.hpp"

namespace fem {
namespace mesh {
  // std::vector<fem::shape::Triangle> Delaunay(
  //     const std::vector<fem::shape::Point>& points);
  bool Intersect(const std::array<double, 2>& a, const std::array<double, 2>& b,
                 const std::array<double, 2>& c,
                 const std::array<double, 2>& d);
  bool SharedEdge(const std::size_t& a, const std::size_t& b,
                  const std::array<std::size_t, 3>& tri);
  bool RotationalLess(const std::size_t& center, const std::size_t& a,
                      const std::size_t& b,
                      const std::vector<std::array<double, 2>>& points);
  bool InCircumCircle(const std::size_t& pt,
                      const std::array<std::size_t, 3>& tri,
                      const std::vector<std::array<double, 2>>& points);
  bool BadInCircumCircle(const std::size_t& pt,
                         const std::array<std::size_t, 3>& tri,
                         const std::vector<std::array<double, 2>>& points);
  // std::vector<std::array<std::size_t, 3>> Delaunay(
  //     std::vector<std::array<double, 2>>& points);
  void InsertPoint(Mesh& mesh, const std::array<double, 2>& vertex);
  bool EdgeCheck(const std::vector<std::array<std::size_t, 2>>& bad_edges,
                 const std::vector<std::array<std::size_t, 2>>& good_edges,
                 const std::array<std::size_t, 2>& edge);
  void ApplyEdge(Mesh& mesh, const std::array<std::size_t, 2>& edge);
  // Mesh Delaunay(std::vector<std::array<double, 2>>& points);
  Mesh Delaunay(const Pslg& plsg);
}  // namespace mesh
}  // namespace fem

#endif  // FEM_MESH_DELAUNAY_HPP_
