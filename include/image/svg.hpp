#ifndef FEM_IMAGE_SVG_HPP_
#define FEM_IMAGE_SVG_HPP_

#include <string>

#include "../mesh/pslg.hpp"
#include "../mesh/mesh.hpp"

namespace fem {
namespace image {
  void SvgInitalizeData(const unsigned& w, const unsigned& h);
  void SvgFreeData();
  bool SvgWriteData(const std::string& file_name);
  void SvgDrawPoint(int x, int y, uint32_t c, uint8_t w = 1);
  void SvgDrawLine(int x0, int y0, int x1, int y1, uint32_t c, uint8_t w = 1);
  void SvgDrawCircle(int cx, int cy, unsigned r, uint32_t c, int w = -1);
  bool WriteSvg(const std::string& file_name, const unsigned& w,
                const unsigned& h, const PSLG& pslg);
  bool WriteSvg(const std::string& file_name, const unsigned& w,
                const unsigned& h, const Mesh& pslg);
}  // namespace image
}  // namespace fem

#endif  // FEM_IMAGE_SVG_HPP_
