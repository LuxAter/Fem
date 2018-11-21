#ifndef FEM_IMAGE_SVG_HPP_
#define FEM_IMAGE_SVG_HPP_

#include <cstdint>
#include <future>
#include <map>
#include <string>
#include <vector>

#include "../mesh/mesh.hpp"
#include "../mesh/pslg.hpp"

namespace fem {
namespace image {
  class Svg {
   public:
    Svg();
    Svg(std::size_t width, std::size_t height);
    Svg(const Svg& copy);

    void Fill(std::string color = "#ffffff");

    void Point(uint32_t x, uint32_t y, std::string color = "#000000");

    void Line(uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1,
              std::string color = "#000000", uint32_t stroke = 1,
              uint32_t dash = 0);

    void Circle(uint32_t x, uint32_t y, uint32_t r,
                std::string color = "#000000", std::string fill = "none",
                uint32_t stroke = 1);

    void Triangle(uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1,
                  uint32_t x2, uint32_t y2, std::string color = "#000000",
                  std::string fill = "none", uint32_t stroke = 1);
    void Triangle(uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1,
                  uint32_t x2, uint32_t y2, uint32_t stroke,
                  std::string color = "#000000");
    void Triangle(const std::array<std::size_t, 3>& tri,
                  const std::vector<std::array<double, 2>>& points,
                  std::string color = "#000000", std::string fill = "none",
                  uint32_t stroke = 1);
    void Triangle(const std::array<std::size_t, 3>& tri,
                  const std::vector<std::array<double, 2>>& points,
                  uint32_t stroke, std::string color = "#000000");

    void Rectangle(uint32_t x, uint32_t y, uint32_t w, uint32_t h,
                   std::string color = "#000000", std::string fill = "none",
                   uint32_t stroke = 1);
    void PolyLine(std::vector<std::array<double, 2>> points,
                  std::string color = "#000000", uint32_t stroke = 1,
                  uint32_t dash = 0);

    void Text(std::string msg, uint32_t x, uint32_t y,
              std::string color = "#000000", std::string anchor = "middle");

    void Pslg(mesh::Pslg pslg, std::string edge = "#000000",
              std::string point = "#000000", std::string hole = "#ff0000",
              uint32_t stroke = 1);
    void Mesh(mesh::Mesh mesh, std::string edge = "#000000",
              std::string vertex = "#000000", uint32_t stroke = 1,
              uint32_t dash = 0, bool label = false);
    void Triangles(std::vector<std::array<uint64_t, 3>>& triangles,
                   std::vector<std::array<double, 2>>& points,
                   std::string edge = "#000000", std::string vertex = "#000000",
                   uint32_t stroke = 1, uint32_t dash = 0, double scale = 1.0,
                   double shift = 0.0);

#ifdef _REENTRANT
    std::future<bool> WriteSvg(const std::string& file_path);
#else
    bool WriteSvg(const std::string& file_path);
#endif
    bool WriteSvgWait(const std::string& file_path);

    inline std::map<std::string, std::string>& operator[](int64_t i) {
      if (i >= 0) {
        return elements_[i];
      } else {
        return elements_[elements_.size() + i];
      }
    }

   protected:
    std::size_t width_, height_;
    std::vector<std::map<std::string, std::string>> elements_;

   private:
  };
}  // namespace image
}  // namespace fem

#endif  // FEM_IMAGE_SVG_HPP_
