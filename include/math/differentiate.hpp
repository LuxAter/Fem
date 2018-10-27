#ifndef FEM_MATH_DIFFERENTIATE_HPP_
#define FEM_MATH_DIFFERENTIATE_HPP_

#include <functional>

namespace fem {
namespace math {
  template <typename _T = double>
  _T Differentiate(const std::function<_T(_T)>& func, _T x, _T h = 0.01) {
    return (1.0 / 280.0 * func(x - 4 * h) - 4.0 / 105 * func(x - 3 * h) +
            1.0 / 5.0 * func(x - 2 * h) - 4.0 / 5.0 * func(x - h) +
            4.0 / 5.0 * func(x + h) - 1.0 / 5.0 * func(x + 2 * h) +
            4.0 / 105.0 * func(x + 3 * h) - 1.0 / 280.0 * func(x + 4 * h)) /
           h;
  }
  template <typename _T = double>
  inline std::function<_T(_T)> Derivative(std::function<_T(_T)> func,
                                          _T h = 0.01) {
    return [func, h](double x) {
      return (1.0 / 280.0 * func(x - 4 * h) - 4.0 / 105 * func(x - 3 * h) +
              1.0 / 5.0 * func(x - 2 * h) - 4.0 / 5.0 * func(x - h) +
              4.0 / 5.0 * func(x + h) - 1.0 / 5.0 * func(x + 2 * h) +
              4.0 / 105.0 * func(x + 3 * h) - 1.0 / 280.0 * func(x + 4 * h)) /
             h;
    };
  }
}  // namespace math
}  // namespace fem

#endif  // FEM_MATH_DIFFERENTIATE_HPP_
