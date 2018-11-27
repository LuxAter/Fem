#ifndef FEM_MESH_QUAD_TREE_HPP_
#define FEM_MESH_QUAD_TREE_HPP_

#include <array>
#include <memory>
#include <vector>
#include <cmath>

namespace fem {
namespace mesh {
  class QuadTree {
   public:
    QuadTree();
    QuadTree(const QuadTree& copy);
    QuadTree(const std::vector<std::array<double, 2>>& pts,
             const std::vector<std::array<std::size_t, 3>>& tris,
             std::size_t cnt = 1);
    QuadTree(const std::vector<std::array<double, 2>>& pts,
             const std::vector<std::array<std::size_t, 3>>& tris,
             const std::array<double, 2>& x_range,
             const std::array<double, 2>& y_range, std::size_t cnt = 1);

    std::size_t FindTri(const std::array<double, 2>& pt);

   private:
    void ConstructTree(const std::vector<std::array<double, 2>>& points,
                       const std::vector<std::array<std::size_t, 3>>& triangles,
                       const std::array<double, 2>& x_range,
                       const std::array<double, 2>& y_range, std::size_t cnt);

    int8_t FindQuad(const std::array<double, 2>& pt) const;
    bool InQuad(uint8_t quad, const std::array<double, 2>& pt) const;

    std::array<double, 2> divider_;
    std::array<std::shared_ptr<QuadTree>, 4> children_;

    std::vector<std::array<double, 2>> points_;
    std::vector<std::array<std::size_t, 3>> triangles_;
  };
}  // namespace mesh
}  // namespace fem

#endif  // FEM_MESH_QUAD_TREE_HPP_
