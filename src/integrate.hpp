#ifndef FEM_INTEGATE_HPP_
#define FEM_INTEGATE_HPP_

#include <functional>

#include "mesh.hpp"

namespace fem {
namespace math {
  double integrate(std::function<double(double, double)> func, unsigned tri,
                   const mesh::Mesh& mesh);
  double integrate(const std::string& func, unsigned tri,
                   const mesh::Mesh& mesh);
}  // namespace math
}  // namespace fem

#endif  // FEM_INTEGATE_HPP_
