#ifndef FEM_BASIS_HPP_
#define FEM_BASIS_HPP_

#include "mesh/mesh.hpp"
#include "util/util.hpp"

namespace fem {
double GlobalBasis(const Mesh& mesh, const unsigned long& i, const Pt& pt,
                   const unsigned long& tri);
}  // namespace fem

#endif  // FEM_BASIS_HPP_
