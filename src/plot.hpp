#ifndef ARTA_PLOT_HPP_
#define ARTA_PLOT_HPP_

#include <string>

#include "linalg.hpp"
#include "mesh.hpp"
#include "pde.hpp"

namespace arta {
void plot_grid(const std::string& dest, const unsigned& w, const unsigned& h,
               const std::vector<std::vector<double>>& vals, std::string cmap);

void plot(const std::string& dest, const PDE* pde, const unsigned& w,
          const unsigned& h, std::string cmap);

void write_bmp(const std::string file, uint32_t** buffer, unsigned w,
               unsigned h);

double clamp(double val, double in_min, double in_max, double out_min,
             double out_max);
}  // namespace arta

#endif  // ARTA_PLOT_HPP_
