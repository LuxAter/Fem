#ifndef ARTA_CALC_INTEGRATE_HPP_
#define ARTA_CALC_INTEGRATE_HPP_

#include <functional>
#include "../mesh.hpp"

namespace arta {
namespace calc {
  double integrate(const std::function<double(double, double)>& func,
                   const unsigned& tri, const mesh::Mesh* mesh);
  double integrate(const std::function<double(double, double, double)>& func,
                   const double& t, const unsigned& tri,
                   const mesh::Mesh* mesh);
}  // namespace calc
}  // namespace arta

#endif  // ARTA_CALC_INTEGRATE_HPP_
