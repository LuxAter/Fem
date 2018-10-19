#ifndef FEM_IMAGE_IMAGE2D_HPP_
#define FEM_IMAGE_IMAGE2D_HPP_

#include <array>
#include <vector>

#include "../shape.hpp"
#include "image.hpp"

namespace fem {
namespace image {
  class Image2D : public Image {
   public:
    Image2D();
    Image2D(std::size_t width, std::size_t height);

    void Point(int32_t x, int32_t y, uint32_t color = 0xffffff);
    void Point(const std::array<double, 2>& pt, uint32_t color = 0xffffff);
    void Point(const shape::Point& pt, uint32_t color = 0xffffff);
    void Line(int32_t x0, int32_t y0, int32_t x1, int32_t y1,
              uint32_t color = 0xffffff);
    void Triangle(int32_t x0, int32_t y0, int32_t x1, int32_t y1, int32_t x2,
                  int32_t y2, uint32_t color = 0xffffff);
    void Triangle(const std::array<std::size_t, 3>& tri,
                  const std::vector<std::array<double, 2>>& points,
                  uint32_t color = 0xfffff);
    void Triangle(const shape::Triangle& tri, uint32_t color = 0xffffff);
    void Rectangle(int32_t x, int32_t y, int32_t w, int32_t h,
                   uint32_t color = 0xffffff);
    void Circle(int32_t x, int32_t y, int32_t r, uint32_t color = 0xffffff);
    void Circle(const std::array<double, 2>& pt, int32_t r,
                uint32_t color = 0xffffff);
    void Circle(const shape::Circle& cir, uint32_t color = 0xffffff);
    void Polygon(std::vector<int32_t> x, std::vector<int32_t> y,
                 uint32_t color = 0xffffff);
    void Polygon(std::vector<std::array<int32_t, 2>> points,
                 uint32_t color = 0xffffff);

   protected:
   private:
    void LineLow(int32_t x0, int32_t y0, int32_t x1, int32_t y1,
                 uint32_t color);
    void LineHeigh(int32_t x0, int32_t y0, int32_t x1, int32_t y1,
                   uint32_t color);
  };
}  // namespace image
}  // namespace fem

#endif  // FEM_IMAGE_IMAGE2D_HPP_
