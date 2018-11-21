#ifndef FEM_MESH_DELAUNAY_HPP_
#define FEM_MESH_DELAUNAY_HPP_

#include "mesh.hpp"
// #include "pslg.hpp"
// #include "shape.hpp"

namespace fem {
namespace del {
  fem::mesh::Mesh DelTri(std::vector<std::array<double, 2>> points);
  fem::mesh::Mesh ConDelTri(std::vector<std::array<double, 2>> points,
                            std::vector<std::array<uint64_t, 2>> edges);

  std::vector<std::array<double, 2>> BSort(
      const std::vector<std::array<double, 2>>& points);
  std::vector<std::array<double, 2>> QSorti(
      const std::vector<std::vector<uint32_t>>& bins,
      const std::vector<std::array<double, 2>>& points);
  std::pair<std::vector<std::array<uint64_t, 3>>,
            std::vector<std::array<uint64_t, 3>>>
  Delaun(std::vector<std::array<double, 2>> points);
  uint64_t Edg(const uint64_t& tri, const uint64_t& adj,
               const std::vector<std::array<uint64_t, 3>>& edg);
  uint64_t TriLoc(const std::array<double, 2>& p,
                  const std::vector<std::array<double, 2>>& points,
                  const std::vector<std::array<uint64_t, 3>>& vertices,
                  const std::vector<std::array<uint64_t, 3>>& adjacency);
  bool Swap(const std::array<double, 2>& p1, const std::array<double, 2>& p2,
            const std::array<double, 2>& p3, const std::array<double, 2>& p);

  std::vector<std::vector<uint64_t>> GenPointIndex(
      const std::vector<std::array<uint64_t, 3>>& triangles,
      const uint64_t& size);
  std::pair<std::vector<std::array<double, 2>>,
            std::vector<std::array<uint64_t, 2>>>
  BSort(const std::vector<std::array<double, 2>>& points,
        const std::vector<std::array<uint64_t, 2>>& edges);
  std::pair<std::vector<std::array<uint64_t, 3>>,
            std::vector<std::array<uint64_t, 3>>>
  ConDelaun(std::vector<std::array<double, 2>> points,
            std::vector<std::array<uint64_t, 2>> edges);
  bool Intersects(const std::array<uint64_t, 2>& ab,
                  const std::array<uint64_t, 2>& cd,
                  const std::vector<std::array<double, 2>>& points);
  template <typename _T>
  std::array<_T, 2> operator-(const std::array<_T, 2>& lhs,
                              const std::array<_T, 2>& rhs) {
    return {{lhs[0] - rhs[0], lhs[1] - rhs[1]}};
  }
}  // namespace del
}  // namespace fem

#endif  // FEM_MESH_DELAUNAY_HPP_
