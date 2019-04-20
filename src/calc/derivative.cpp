#include "derivative.hpp"

#include <functional>

static double weights_[9] = {
    +0.003571428571428571, -0.038095238095238100, +0.200000000000000000,
    -0.800000000000000000, +0.000000000000000000, +0.800000000000000000,
    -0.200000000000000000, +0.038095238095238100, -0.003571428571428571};

double arta::calc::derivative(const std::function<double(double, double)>& func,
                              const double& x, const double& y,
                              const double& hx, const double& hy) {
  double sum = 0.0;
  for (unsigned i = 0; i < 9; ++i) {
    sum += (weights_[i] * func(x + ((static_cast<int>(i) - 4) * hx),
                               y + ((static_cast<int>(i) - 4) * hy)));
  }
  return sum / (hx + hy);
}
