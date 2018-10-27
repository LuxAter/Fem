#include "math/function.hpp"

#include <functional>
#include <vector>

std::function<double(double)> fem::math::GenPiecewise(
    std::function<double(double)> a, std::array<double, 2> a_range,
    std::function<double(double)> other) {
  return [a, a_range, other](double x) {
    if (x >= a_range[0] && x <= a_range[1])
      return a(x);
    else
      return other(x);
  };
}
std::function<double(double)> fem::math::GenPiecewise(
    std::function<double(double)> a, std::array<double, 2> a_range,
    std::function<double(double)> b, std::array<double, 2> b_range,
    std::function<double(double)> other) {
  return [a, a_range, b, b_range, other](double x) {
    if (x >= a_range[0] && x < a_range[1])
      return a(x);
    else if (x >= b_range[0] && x <= b_range[1])
      return b(x);
    else
      return other(x);
  };
}
std::function<double(double)> fem::math::GenPiecewise(
    std::function<double(double)> a, std::array<double, 2> a_range,
    std::function<double(double)> b, std::array<double, 2> b_range,
    std::function<double(double)> c, std::array<double, 2> c_range,
    std::function<double(double)> other) {
  return [a, a_range, b, b_range, c, c_range, other](double x) {
    if (x >= a_range[0] && x < a_range[1])
      return a(x);
    else if (x >= b_range[0] && x < b_range[1])
      return b(x);
    else if (x >= c_range[0] && x <= c_range[1])
      return c(x);
    else
      return other(x);
  };
}
