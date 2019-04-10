#ifndef ARTA_MESH_HPP_
#define ARTA_MESH_HPP_

#include <string>

namespace arta {
namespace mesh {
  void construct_mesh(const std::string& source, const std::string& dest,
                      const double& area, const double& angle);
}  // namespace mesh
}  // namespace arta

#endif  // ARTA_MESH_HPP_
