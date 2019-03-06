#ifndef FEM_PLOT_HPP_
#define FEM_PLOT_HPP_

#include <array>
#include <limits>
#include <string>
#include <vector>

namespace fem {
namespace plot {
  void imsave(std::string file, std::vector<std::vector<double>> vals,
              std::string cmap = "jet",
              double v_min = std::numeric_limits<double>::infinity(),
              double v_max = -std::numeric_limits<double>::infinity());
  void write_bmp(const std::string file, uint32_t** buffer, unsigned w,
                 unsigned h);
  double clamp(double val, double in_min, double in_max, double out_min,
               double out_max);
}  // namespace plot
}  // namespace fem

#endif  // FEM_PLOT_HPP_
