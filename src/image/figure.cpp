#include "image/figure.hpp"

#include <cmath>
#include <cstdint>

#include <array>
#include <functional>
#include <vector>

#include "image/svg.hpp"

fem::image::Figure::Figure()
    : width_(0),
      height_(0),
      color_(0),
      bounds_({{INFINITY, INFINITY, -INFINITY, -INFINITY}}) {}
fem::image::Figure::Figure(const uint64_t& w, const uint64_t& h)
    : width_(w),
      height_(h),
      color_(0),
      bounds_({{INFINITY, INFINITY, -INFINITY, -INFINITY}}) {}

fem::image::Figure::Figure(const Figure& copy)
    : width_(copy.width_),
      height_(copy.height_),
      color_(copy.color_),
      bounds_(copy.bounds_),
      scatter_(copy.scatter_),
      line_(copy.line_) {}

void fem::image::Figure::SaveSvg(const std::string& file) {
  Svg svg(width_, height_);
  svg.Rectangle(0, 0, width_, height_, "white", "white");
  double d_max = std::max(bounds_[2] - bounds_[0], bounds_[3] - bounds_[1]);
  double sx = (width_ - (width_ * 0.1)) / (bounds_[2] - bounds_[0]);
  double sy = (height_ - (height_ * 0.1)) / (bounds_[3] - bounds_[1]);
  double offset_x = (width_ * 0.05);
  double offset_y = (height_ * 0.05);
  // double sx = (width_ - (width_ * 0.1)) / d_max;
  // double sy = (height_ - (height_ * 0.1)) / d_max;
  for (auto& it : scatter_) {
    for (auto& pt : std::get<0>(it)) {
      pt[0] = (pt[0] - bounds_[0]) * sx + offset_x;
      pt[1] = height_ - ((pt[1] - bounds_[1]) * sy) - offset_y;
    }
    PlotScatter(svg, it);
  }
  for (auto& it : line_) {
    for (auto& pt : std::get<0>(it)) {
      // std::cout << pt[0] << ',' << pt[1] << "->";
      pt[0] = (pt[0] - bounds_[0]) * sx + offset_x;
      pt[1] = height_ - ((pt[1] - bounds_[1]) * sy) - offset_y;
      // std::cout << pt[0] << ',' << pt[1] << "\n";
    }
    PlotLine(svg, it);
  }
  svg.WriteSvgWait(file);
}

void fem::image::Figure::SavePgf(const std::string& file) {
  std::string pre = "\\begin{tikzpicture}\n";
  char buff[256];
  snprintf(buff, 255, "\\begin{axis}[xmin=%lf, xmax=%lf, ymin=%lf, ymax=%lf]\n",
           bounds_[0], bounds_[2], bounds_[1], bounds_[3]);
  std::string axis(buff);
  std::string plots = "";
  std::string post="\\end{axis}\n\\end{tikzpicture}";
  uint32_t color_index = 0;
  for (auto& it : line_) {
    uint32_t r = 0, g = 0, b = 0;
    sscanf(std::get<1>(it).c_str(), "#%02x%02x%02x", &r, &g, &b);
    // std::cout << std::get<1>(it) << "->" << r << ',' << g << ',' << b << '\n';
    snprintf(buff, 255, "\\definecolor{color%u}{RGB}{%u,%u,%u}\n", color_index, r, g, b);
    pre += std::string(buff);
    std::string opt = std::get<5>(it) != 0 ? "dashed, " : "";
    snprintf(buff, 255, "\\addplot[color%u, %s forget plot]\ntable{%%\n", color_index, opt.c_str());
    std::string plot(buff);
    for (auto& pt : std::get<0>(it)) {
      plot += std::to_string(pt[0]) + ' ' + std::to_string(pt[1]) + '\n';
    }
    plot += "};\n";
    plots += plot;
    color_index++;
  }
  FILE* out = fopen(file.c_str(), "w");
  if (!out) return;
  fprintf(out, "%s", pre.c_str());
  fprintf(out, "%s", axis.c_str());
  fprintf(out, "%s", plots.c_str());
  fprintf(out, "%s", post.c_str());
  fclose(out);
}

void fem::image::Figure::PlotScatter(
    Svg& svg, const std::tuple<std::vector<std::array<double, 2>>, std::string,
                               std::string, double, double>& scatter) {
  std::string color = std::get<1>(scatter);
  std::string fill = std::get<2>(scatter);
  double stroke = std::get<3>(scatter);
  double r = std::get<4>(scatter);
  for (auto& pt : std::get<0>(scatter)) {
    svg.Circle(pt[0], pt[1], r, color, fill, stroke);
  }
}
void fem::image::Figure::PlotLine(
    Svg& svg,
    const std::tuple<std::vector<std::array<double, 2>>, std::string,
                     std::string, std::string, double, double, double>& line) {
  std::string color = std::get<1>(line);
  std::string vertex_color = std::get<2>(line);
  std::string vertex_fill = std::get<3>(line);
  double stroke = std::get<4>(line);
  double dash = std::get<5>(line);
  double r = std::get<6>(line);
  svg.PolyLine(std::get<0>(line), color, stroke, dash);
  for (auto& pt : std::get<0>(line)) {
    svg.Circle(pt[0], pt[1], r, vertex_color, vertex_fill, stroke);
  }
}
