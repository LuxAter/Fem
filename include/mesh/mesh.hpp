#ifndef FEM_MESH_MESH_HPP_
#define FEM_MESH_MESH_HPP_

#include <array>
#include <string>
#include <vector>

namespace fem {
namespace mesh {
  class Mesh {
   public:
    Mesh();
    Mesh(const Mesh& copy);
    Mesh(const std::vector<std::array<double, 2>>& points,
         const std::vector<std::array<std::size_t, 3>>& triangles);
    Mesh(const std::vector<std::array<double, 2>>& points,
         const std::vector<std::array<std::size_t, 3>>& triangles,
         const std::vector<std::array<std::size_t, 3>>& adj);

    void DeterminEdges();
    bool InMesh(const std::array<double, 2>& pt) const;

    std::vector<std::array<double, 2>> points;
    std::vector<std::array<std::size_t, 2>> edges;
    std::vector<std::array<std::size_t, 3>> triangles;
    std::vector<std::array<std::size_t, 3>> adjacency;
   private:
  };
  Mesh operator*(const Mesh& lhs, const double& rhs);
  Mesh operator+(const Mesh& lhs, const double& rhs);

  Mesh LoadMesh(const std::string& file);
  void SaveMesh(const std::string& file, Mesh mesh);
  Mesh LoadTriangleMesh(const std::string& name);

  Mesh Flip(Mesh mesh);
}  // namespace mesh
}  // namespace fem

#endif  // FEM_MESH_MESH_HPP_
