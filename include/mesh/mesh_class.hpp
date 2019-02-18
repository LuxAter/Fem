#ifndef FEM_MESH_MESH_CLASS_HPP_
#define FEM_MESH_MESH_CLASS_HPP_

#include <vector>
#include <string>
#include <map>

#include "../util/util.hpp"

namespace fem {
struct Mesh {
  std::vector<Pt> pts;
  std::vector<Tri> tri;
  std::vector<Tri> adj;
  std::map<std::size_t, std::vector<double>> bc;
};

unsigned long TriLoc(const Pt& pt, const Mesh& mesh);

Mesh LoadMesh(const std::string& file_name);
}  // namespace fem

#endif  // FEM_MESH_MESH_CLASS_HPP_
