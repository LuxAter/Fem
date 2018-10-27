#ifndef FEM_MESH_SHAPE_HPP_
#define FEM_MESH_SHAPE_HPP_

#include <array>

namespace fem {
namespace mesh{
  typedef typename std::array<double, 2> Vertex;
  typedef typename std::array<std::size_t, 2> Edge;
  typedef typename std::array<std::size_t, 3> Triangle;
  bool operator==(const Vertex& lhs, const Vertex& rhs) {
    return lhs[0] == rhs[0] && lhs[1] == rhs[1];
  }
  bool operator<(const std::size_t& lhs, const Edge& rhs) {
    return lhs == rhs[0] || lhs == rhs[1];
  }
  bool operator==(const Edge& lhs, const Edge& rhs) {
    return (lhs[0] == rhs[0] && lhs[1] == rhs[1]) ||
           (lhs[0] == rhs[1] && lhs[0] == rhs[0]);
  }
  bool operator==(const Triangle& lhs, const Triangle& rhs) {
    return (lhs[0] == rhs[0] && lhs[1] == rhs[1] && lhs[2] == rhs[2]) ||
           (lhs[0] == rhs[1] && lhs[1] == rhs[2] && lhs[2] == rhs[0]) ||
           (lhs[0] == rhs[2] && lhs[1] == rhs[0] && lhs[2] == rhs[1]);
  }
  bool operator<(const std::size_t& lhs, const Triangle& rhs) {
    return lhs == rhs[0] || lhs == rhs[1] || lhs == rhs[2];
  }
  bool operator<(const Edge& lhs, const Triangle& rhs) {
    return (lhs[0] == rhs[0] && lhs[1] == rhs[1]) ||
           (lhs[1] == rhs[0] && lhs[0] == rhs[1]) ||
           (lhs[0] == rhs[1] && lhs[1] == rhs[2]) ||
           (lhs[1] == rhs[1] && lhs[0] == rhs[2]) ||
           (lhs[0] == rhs[2] && lhs[1] == rhs[0]) ||
           (lhs[1] == rhs[2] && lhs[0] == rhs[0]);
  }
  std::array<std::array<std::size_t, 2>, 3> GetEdges(const Triangle& tri) {
    return {{{{tri[0], tri[1]}}, {{tri[1], tri[2]}}, {{tri[2], tri[0]}}}};
  }
}  // namespace mesh 
}  // namespace fem

#endif  // FEM_MESH_SHAPE_HPP_
