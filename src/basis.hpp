#ifndef FEM_BASIS_HPP_
#define FEM_BASIS_HPP_

#include "geometry.hpp"
#include "mesh.hpp"

namespace fem {
namespace basis {
  double local_basis(const mesh::Mesh& mesh, const Pair<double>& pt,
                     const unsigned& tri, const unsigned& vert);
  double local_basis_deriv(const mesh::Mesh& mesh, const Pair<double>& pt,
                           const unsigned& tri, const unsigned& vert);
  double global_basis(const mesh::Mesh& mesh, const Pair<double>& pt,
                      const unsigned& tri, const unsigned& vert);
}  // namespace basis
}  // namespace fem

#endif  // FEM_BASIS_HPP_
