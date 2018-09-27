#include "image/image2d.hpp"
#include "image/image.hpp"

fem::image::Image2D::Image2D() : Image() {}
fem::image::Image2D::Image2D(std::size_t width, std::size_t height)
    : Image(width, height) {}

void fem::image::Image2D::Line(uint32_t x0, uint32_t y0, uint32_t x1,
    uint32_t y1, uint32_t color) {

}
