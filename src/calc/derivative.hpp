#ifndef ARTA_CALC_DERIVATIVE_HPP_
#define ARTA_CALC_DERIVATIVE_HPP_

#include <functional>

namespace arta {
namespace calc {
  double derivative(const std::function<double(double, double)>& func,
                    const double& x, const double& y, const double& hx,
                    const double& hy);
}  // namespace calc
}  // namespace arta

#endif  // ARTA_CALC_DERIVATIVE_HPP_
