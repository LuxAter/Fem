#ifndef FEM_IMAGE_IMAGE2D_HPP_
#define FEM_IMAGE_IMAGE2D_HPP_

#include "image.hpp"

namespace fem {
namespace image {
  class Image2D : public Image{
    public:
      Image2D();
      Image2D(std::size_t width, std::size_t height);

      void Line(uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1, uint32_t color=0xffffff);
    protected:
    private:
  };
}  // namespace image
}  // namespace fem

#endif  // FEM_IMAGE_IMAGE2D_HPP_
