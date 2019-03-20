#ifndef FEM_MESH_HPP_
#define FEM_MESH_HPP_

#include <array>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>

#include "geometry.hpp"

namespace fem {
namespace mesh {
  class Mesh {
   public:
    Mesh() {}
    explicit Mesh(const std::string& file_path);

    std::vector<Pair<double>> pts;
    std::vector<unsigned long> boundary_index;
    std::vector<Triple<long>> tri, adj;
    std::vector<std::string> bc;
    std::array<double, 4> bounds;
    bool has_holes;

    int locate(const Pair<double>& pt) const;
    double boundary(const unsigned long& point_index) const;
    inline bool is_boundary(const unsigned long& point_index) const {
      return boundary_index[point_index] != 0;
    }

   protected:
   private:
  };
}  // namespace mesh
}  // namespace fem

#endif  // FEM_MESH_HPP_
