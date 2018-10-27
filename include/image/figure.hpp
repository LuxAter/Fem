#ifndef FEM_IMAGE_FIGURE_HPP_
#define FEM_IMAGE_FIGURE_HPP_

#include <cmath>
#include <cstdint>

#include <array>
#include <functional>
#include <vector>

#include "svg.hpp"

#include <iostream>

namespace fem {
namespace image {
  class Figure {
   public:
    Figure();
    Figure(const uint64_t& w, const uint64_t& h);
    Figure(const Figure& copy);

    template <typename _T, std::size_t _N>
    void Line(std::array<_T, _N> y);
    template <typename _T, std::size_t _N>
    void Line(std::array<_T, _N> x, std::array<_T, _N> y,
              std::string color = "", std::string vertex_color = "",
              std::string vertex_fill = "", uint32_t stroke = 1, uint32_t dash=0,
              uint32_t vertex_r = 0) {
      std::tuple<std::vector<std::array<double, 2>>, std::string, std::string,
                 std::string, double, double, double>
          local_plot;
      if (color == "" && vertex_color == "" && vertex_fill == "") {
        color = colors_[color_];
        vertex_color = colors_[color_];
        vertex_fill = colors_[color_];
        color_ = (color_ + 1) % 18;
      }
      for (std::size_t i = 0; i < _N; ++i) {
        double dx = static_cast<double>(x[i]);
        double dy = static_cast<double>(y[i]);
        // std::cout << x[i] << "->" << dx << "<<<\n";
        bounds_[0] = std::min(bounds_[0], dx);
        bounds_[1] = std::min(bounds_[1], dy);
        bounds_[2] = std::max(bounds_[2], dx);
        bounds_[3] = std::max(bounds_[3], dy);
        std::get<0>(local_plot).push_back({{dx, dy}});
      }
      std::get<1>(local_plot) = color;
      std::get<2>(local_plot) = vertex_color;
      std::get<3>(local_plot) = vertex_fill;
      std::get<4>(local_plot) = stroke;
      std::get<5>(local_plot) = dash;
      std::get<6>(local_plot) = vertex_r;
      line_.push_back(local_plot);
    }

    template <typename _T, std::size_t _N>
    void Scatter(std::array<_T, _N> y);
    template <typename _T, std::size_t _N>
    void Scatter(std::array<_T, _N> x, std::array<_T, _N> y,
                 std::string color = "", std::string fill = "",
                 uint32_t stroke = 1, uint32_t r = 1) {
      std::tuple<std::vector<std::array<double, 2>>, std::string, std::string,
                 double, double>
          local_plot;
      if (color == "" && fill == "") {
        color = colors_[color_];
        fill = colors_[color_];
        color_ = (color_ + 1) % 18;
      }
      for (std::size_t i = 0; i < _N; ++i) {
        double dx = static_cast<double>(x[i]);
        double dy = static_cast<double>(y[i]);
        bounds_[0] = std::min(bounds_[0], dx);
        bounds_[1] = std::min(bounds_[1], dy);
        bounds_[2] = std::max(bounds_[2], dx);
        bounds_[3] = std::max(bounds_[3], dy);
        std::get<0>(local_plot).push_back({{dx, dy}});
      }
      std::get<1>(local_plot) = color;
      std::get<2>(local_plot) = fill;
      std::get<3>(local_plot) = stroke;
      std::get<4>(local_plot) = r;
      scatter_.push_back(local_plot);
    }

    void SaveSvg(const std::string& file);
    void SavePgf(const std::string& file);

   private:
    void PlotScatter(
        Svg& svg,
        const std::tuple<std::vector<std::array<double, 2>>, std::string,
                         std::string, double, double>& scatter);
    void PlotLine(
        Svg& svg,
        const std::tuple<std::vector<std::array<double, 2>>, std::string,
                         std::string, std::string, double, double, double>& line);

    uint64_t width_, height_;
    uint8_t color_;

    std::array<std::string, 18> colors_ = {
        {"#2196F3", "#4CAF50", "#FFC107", "#F44336", "#3F51B5", "#57C7B8",
         "#FFEB3B", "#795548", "#673AB7", "#00BCD4", "#CDDC39", "#FF5722",
         "#9C27B0", "#03A9F4", "#8BC34A", "#FF9800", "#E91E63"}};

    std::array<double, 4> bounds_;

    std::vector<std::tuple<std::vector<std::array<double, 2>>, std::string,
                           std::string, double, double>>
        scatter_;
    std::vector<std::tuple<std::vector<std::array<double, 2>>, std::string,
                           std::string, std::string, double, double, double>>
        line_;
  };
}  // namespace image
}  // namespace fem

#endif  // FEM_IMAGE_FIGURE_HPP_
