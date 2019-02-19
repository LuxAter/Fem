#ifndef FEM_IMAGE_PNG_HPP_
#define FEM_IMAGE_PNG_HPP_

#include <string>

#include <png.h>

#include "../mesh/mesh.hpp"
#include "../mesh/pslg.hpp"

namespace fem {
namespace image {
  void PngInitalizeData(const unsigned& w, const unsigned& h);
  void PngFreeData();
  bool PngWriteData(const std::string& file_name);
  void PngDrawPoint(int x, int y, uint32_t c, uint8_t w = 1);
  void PngDrawLineLow(int x0, int y0, int x1, int y1, uint32_t c,
                      uint8_t w = 1);
  void PngDrawLineHigh(int x0, int y0, int x1, int y1, uint32_t c,
                       uint8_t w = 1);
  void PngDrawLine(int x0, int y0, int x1, int y1, uint32_t c, uint8_t w = 1);
  void PngDrawCircle(int cx, int cy, unsigned r, uint32_t c, int w = -1);
  bool WritePng(const std::string& file_name, const unsigned& w,
                const unsigned& h, const PSLG& pslg);
  bool WritePng(const std::string& file_name, const unsigned& w,
                const unsigned& h, const Mesh& mesh);
  bool WritePng(const std::string& file_name, const unsigned& w,
                const unsigned& h, const Mesh& mesh,
                double (*func)(const Mesh& mesh, const Pt&));
}  // namespace image
}  // namespace fem

#endif  // FEM_IMAGE_PNG_HPP_
