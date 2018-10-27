#ifndef FEM_MATH_FUNCTION_HPP_
#define FEM_MATH_FUNCTION_HPP_

#include <functional>
#include <vector>

namespace fem {
namespace math {
  std::function<double(double)> GenPiecewise(
      std::function<double(double)> a, std::array<double, 2> a_range,
      std::function<double(double)> other = [](double x) { return 0; });
  std::function<double(double)> GenPiecewise(
      std::function<double(double)> a, std::array<double, 2> a_range,
      std::function<double(double)> b, std::array<double, 2> b_range,
      std::function<double(double)> other = [](double x) { return 0; });
  std::function<double(double)> GenPiecewise(
      std::function<double(double)> a, std::array<double, 2> a_range,
      std::function<double(double)> b, std::array<double, 2> b_range,
      std::function<double(double)> c, std::array<double, 2> c_range,
      std::function<double(double)> other = [](double x) { return 0; });
}  // namespace math
}  // namespace fem

#endif  // FEM_MATH_FUNCTION_HPP_
