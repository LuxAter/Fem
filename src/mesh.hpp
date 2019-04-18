#ifndef ARTA_MESH_HPP_
#define ARTA_MESH_HPP_

#include <string>
#include <vector>

#include "linalg.hpp"

namespace arta {
namespace mesh {
  class Mesh {
   public:
    Mesh();
    explicit Mesh(const std::string& base_name);
    Mesh(const Mesh& copy);


    std::vector<linalg::Pair<double>> pts;
    std::vector<unsigned long> bdry_index;
    std::vector<linalg::Triple<long>> tri, adj;
    std::array<double, 4> bounds;
    bool has_holes_;

   private:
  };

  void construct_mesh(const std::string& source, const std::string& dest,
                      const double& area, const double& angle);
}  // namespace mesh
}  // namespace arta

#endif  // ARTA_MESH_HPP_
