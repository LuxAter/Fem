#ifndef FEM_INTEGATE_HPP_
#define FEM_INTEGATE_HPP_

#include "mesh.hpp"

namespace fem {
namespace math {
  double integrate(double (*func)(double, double), unsigned tri,
                   const mesh::Mesh& mesh);
}  // namespace math
}  // namespace fem

#endif  // FEM_INTEGATE_HPP_
