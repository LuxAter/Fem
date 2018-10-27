#ifndef FEM_MATH_INTEGRATE_HPP_
#define FEM_MATH_INTEGRATE_HPP_

#include <functional>

#include <iostream>

namespace fem {
namespace math {
  template <typename _T = double>
  _T Integrate(const std::function<_T(_T)>& func, _T a, _T b,
               std::size_t steps = 100) {
    _T h = _T((b - a) / static_cast<_T>(steps));
    _T sum_a = _T();
    _T sum_b = _T();
    for (std::size_t i = 1; i < steps; i += 2) {
      sum_a += func(a + (static_cast<_T>(i) * h));
    }
    for (std::size_t i = 2; i < steps - 1; i += 2) {
      sum_b += func(a + (static_cast<_T>(i) * h));
    }
    return (h / 3.0) * (func(a) + 4.0 * sum_a + 2.0 * sum_b + func(b));
  }
}  // namespace math
}  // namespace fem

#endif  // FEM_MATH_INTEGRATE_HPP_
