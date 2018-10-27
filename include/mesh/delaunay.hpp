#ifndef FEM_MESH_DELAUNAY_HPP_
#define FEM_MESH_DELAUNAY_HPP_

#include "pslg.hpp"
#include "mesh.hpp"
#include "shape.hpp"

namespace fem {
  namespace mesh {
    void InsertVertex(Mesh& mesh, const Vertex& vert);
    void RestoreDelaunay(Mesh& mesh);
    std::vector<std::array<double, 2>> SortPoints(const Pslg& pslg);
    Mesh Delaunay(const Pslg& pslg);
  } // namespace mesh
} // namespace fem

#endif  // FEM_MESH_DELAUNAY_HPP_
