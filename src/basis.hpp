#ifndef ARTA_BASIS_HPP_
#define ARTA_BASIS_HPP_

#include "mesh.hpp"

namespace arta {
namespace basis {
  double local(const mesh::Mesh* mesh, const double& x, const double& y,
               const unsigned& tri, const unsigned& vert);
  double dlocal_dx(const mesh::Mesh* mesh, const double& x, const double& y,
                   const unsigned& tri, const unsigned& vert);
  double dlocal_dy(const mesh::Mesh* mesh, const double& x, const double& y,
                   const unsigned& tri, const unsigned& vert);
  double global(const mesh::Mesh* mesh, const double& x, const double& y,
                const unsigned& vert, const unsigned& tri);
}  // namespace basis
}  // namespace arta

#endif  // ARTA_BASIS_HPP_
