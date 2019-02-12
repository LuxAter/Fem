#ifndef FEM_MESH_MESH_CLASS_HPP_
#define FEM_MESH_MESH_CLASS_HPP_

#include <vector>

#include "../util/util.hpp"

namespace fem {
struct Mesh {
  std::vector<Pt> pts;
  std::vector<Tri> tri;
  std::vector<Tri> adj;
};
}  // namespace fem

#endif  // FEM_MESH_MESH_CLASS_HPP_
