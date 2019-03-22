#ifndef FEM_PLOT_HPP_
#define FEM_PLOT_HPP_

#include <array>
#include <limits>
#include <string>
#include <vector>

#include "mesh.hpp"

namespace fem {
namespace plot {
  void line_low(uint32_t** buffer, unsigned x0, unsigned y0, unsigned x1,
                unsigned y1, const unsigned& w, const unsigned & h);
  void line_high(uint32_t** buffer, unsigned x0, unsigned y0, unsigned x1,
                 unsigned y1, const unsigned& w, const unsigned& h);
  void line(uint32_t** buffer, unsigned x0, unsigned y0, unsigned x1,
            unsigned y1, const unsigned& w, const unsigned& h);
  void overlay(uint32_t** buffer, const mesh::Mesh& mesh, const unsigned& w,
               const unsigned& h);
  void imsave(std::string file, std::vector<std::vector<double>> vals,
              std::string cmap = "jet",
              double v_min = std::numeric_limits<double>::infinity(),
              double v_max = -std::numeric_limits<double>::infinity(),
              const mesh::Mesh* mesh = NULL);
  void write_bmp(const std::string file, uint32_t** buffer, unsigned w,
                 unsigned h);
  double clamp(double val, double in_min, double in_max, double out_min,
               double out_max);
}  // namespace plot
}  // namespace fem

#endif  // FEM_PLOT_HPP_
