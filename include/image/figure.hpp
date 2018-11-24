#ifndef FEM_IMAGE_FIGURE_HPP_
#define FEM_IMAGE_FIGURE_HPP_

#include <array>
#include <cmath>
#include <functional>
#include <future>
#include <vector>

#include "../mesh/mesh.hpp"

namespace fem {
namespace image {
  class Figure {
   public:
    Figure(const uint64_t& w, const uint64_t& h);
    Figure(const Figure& copy);

    void SetCmap(const std::string& cmap);
    void SetSpacing(const double& spacing);
    void SetBackground(const uint32_t& rgb);

    void Rectangle(const std::function<double(double, double)>& func,
                   const double& x, const double& y, const double& w,
                   const double& h, double vmin = -INFINITY,
                   double vmax = INFINITY);
    void Circle(const std::function<double(double, double)>& func,
                const double& cx, const double& cy, const double& r,
                double vmin = -INFINITY, double vmax = INFINITY);
    void Mesh(const std::function<double(double, double)>& func,
              const fem::mesh::Mesh& mesh, double vmin = -INFINITY,
              double vmax = INFINITY);

    void OverlayCircle(int cx, int cy, int r, uint32_t color = 0x000000);
    void OverlayLine(int x0, int y0, int x1, int y1, uint8_t size = 1, uint32_t color = 0x000000);
    void OverlayLineHigh(int x0, int y0, int x1, int y1, uint8_t size = 1, uint32_t color = 0x000000);
    void OverlayLineLow(int x0, int y0, int x1, int y1, uint8_t size = 1, uint32_t color = 0x000000);
    void OverlayGrid(uint32_t x_steps, uint32_t y_steps, uint8_t size = 1,
                     uint32_t color = 0x000000);
    void OverlayMesh(fem::mesh::Mesh& mesh, uint8_t size = 1,
                     uint32_t color = 0x000000);

#ifdef _REENTRANT
    std::future<bool> WritePng(const std::string& file_path);
#else
    bool WritePng(const std::string& file_path);
#endif
    bool WritePngWait(const std::string& file_path);

   private:
    uint64_t width_, height_;
    std::string cmap_ = "viridis";
    double spacing_ = 0.05;
    uint32_t background_ = 0xffffff;
    std::array<double, 2> bounds_ = {{INFINITY, -INFINITY}};
    std::vector<std::vector<double>> data_;
    std::vector<std::vector<int64_t>> overlay_;

    inline uint64_t LimitPixel(int64_t v, int64_t min_v, int64_t max_v) const {
      return std::min(std::max(v, min_v), max_v - 1);
    }
    inline uint32_t ConvertRgb(const uint8_t& r, const uint8_t& g,
                               const uint8_t& b) const {
      return (((static_cast<uint32_t>(r) << 8) + static_cast<uint32_t>(g))
              << 8) +
             static_cast<uint32_t>(b);
    }
    inline uint32_t ConvertRgb(const double& r, const double& g,
                               const double& b) const {
      return ConvertRgb(static_cast<uint8_t>(r * 255),
                        static_cast<uint8_t>(g * 255),
                        static_cast<uint8_t>(b * 255));
    }
    inline uint32_t ConvertRgb(const std::array<double, 3>& rgb) const {
      return ConvertRgb(rgb[0], rgb[1], rgb[2]);
    }
    std::array<double, 3> CmapLookUp(
        const double& z, const std::vector<std::array<double, 3>>& cmap) const;
    std::vector<std::array<double, 3>> LoadCmap(const std::string& cmap) const;
    std::vector<std::vector<uint32_t>> GenFrameBuffer() const;
  };
}  // namespace image
}  // namespace fem

#endif  // FEM_IMAGE_FIGURE_HPP_
