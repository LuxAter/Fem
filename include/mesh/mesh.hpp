#ifndef FEM_MESH_MESH_HPP_
#define FEM_MESH_MESH_HPP_

#include <array>
#include <string>
#include <vector>

namespace fem {
namespace mesh {
  
  bool Contains(const std::array<std::size_t, 2>& edge, const std::size_t& vertex);
  bool Contains(const std::array<std::size_t, 3>& tri, const std::size_t& vertex);
  bool Contains(const std::array<std::size_t, 3>& tri, const std::array<std::size_t, 2>& edge);

  class Mesh {
   public:
    Mesh();
    Mesh(const std::vector<std::array<double, 2>>& points,
         const std::vector<std::array<std::size_t, 3>>& triangles);

    void DeterminEdges();
    bool CircumCircle(const std::size_t& tri, const std::array<double, 2>& pt) const;
    bool CircumCircle(const std::size_t& tri, const std::size_t& pt) const;

    std::vector<std::array<double, 2>> points;
    std::vector<std::array<std::size_t, 3>> triangles;
    std::vector<std::array<std::size_t, 2>> edges;
   private:
  };

  Mesh LoadMesh(const std::string& file);
  void SaveMesh(const std::string& file, Mesh mesh);
}  // namespace mesh
}  // namespace fem

#endif  // FEM_MESH_MESH_HPP_
