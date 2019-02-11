#include "image/svg.hpp"

#include <cmath>
#include <fstream>
#include <string>
#include <vector>

#include "mesh/pslg.hpp"
#include "util/to_string.hpp"

static std::vector<std::string> instructions_;
static unsigned width_ = 0, height_ = 0;

void fem::image::SvgInitalizeData(const unsigned& w, const unsigned& h) {
  instructions_.push_back("<svg width=\"" + std::to_string(w) + "\" height=\"" +
                          std::to_string(h) + "\">");
  instructions_.push_back("<rect x=\"0\" y=\"0\" width=\"" + std::to_string(w) +
                          "\" height=\"" + std::to_string(h) +
                          "\" fill=\"#ffffff\" />");
  width_ = w;
  height_ = h;
}
void fem::image::SvgFreeData() {
  instructions_.clear();
  width_ = 0;
  height_ = 0;
}
bool fem::image::SvgWriteData(const std::string& file_name) {
  std::ofstream out(file_name.c_str());
  if (out.is_open()) {
    for (auto& ins : instructions_) {
      out << ins << '\n';
    }
    out << "</svg>\n";
    out.close();
  }
  return true;
}
void fem::image::SvgDrawPoint(int x, int y, uint32_t c, uint8_t w) {
  instructions_.push_back("<circle cx=\"" + std::to_string(x) + "\" cy=\"" +
                          std::to_string(y) + "\" r=\"" + std::to_string(w) +
                          "\" fill=\"" + fem::to_hex(c) + "\" stroke=\"" +
                          fem::to_hex(c) + "\" />");
}
void fem::image::SvgDrawLine(int x0, int y0, int x1, int y1, uint32_t c,
                             uint8_t w) {
  instructions_.push_back("<line x1=\"" + std::to_string(x0) + "\" y1=\"" +
                          std::to_string(y0) + "\" x2=\"" + std::to_string(x1) +
                          "\" y2=\"" + std::to_string(y1) + "\" stroke=\"" +
                          fem::to_hex(c) + "\" stroke-width=\"" +
                          std::to_string(w) + "\" />");
}
void fem::image::SvgDrawCircle(int cx, int cy, unsigned r, uint32_t c, int w) {
  if (w > 0) {
    instructions_.push_back("<circle cx=\"" + std::to_string(cx) + "\" cy=\"" +
                            std::to_string(cy) + "\" r=\"" + std::to_string(r) +
                            "\" stroke-width=\"" + std::to_string(w) +
                            "\" stroke=\"" + fem::to_hex(c) + "\" />");
  } else {
    instructions_.push_back("<circle cx=\"" + std::to_string(cx) + "\" cy=\"" +
                            std::to_string(cy) + "\" r=\"" + std::to_string(r) +
                            "\" fill=\"" + fem::to_hex(c) + "\" stroke=\"" +
                            fem::to_hex(c) + "\" />");
  }
}
bool fem::image::WriteSvg(const std::string& file_name, const unsigned& w,
    const unsigned& h, const PSLG& pslg) {
  SvgInitalizeData(w, h);
  std::vector<Pt> scaled_points = pslg.points;
  std::vector<Pt> scaled_holes = pslg.holes;
  double x = w / 2.0;
  double y = h / 2.0;
  for (auto& pt : scaled_points) {
    pt.x = (pt.x * (x * 0.98)) + x;
    pt.y = (pt.y * (y * 0.98)) + y;
    SvgDrawCircle(pt.x, pt.y, 2, 0);
  }
  for(auto& edg : pslg.edges){
    SvgDrawLine(scaled_points[edg.x].x, scaled_points[edg.x].y, scaled_points[edg.y].x, scaled_points[edg.y].y, 0);
  }
  for (auto& pt : scaled_holes) {
    pt.x = (pt.x * x) + x;
    pt.y = (pt.y * y) + y;
    SvgDrawCircle(pt.x, pt.y, 2, 0xff0000);
  }
  bool res = SvgWriteData(file_name);
  SvgFreeData();
  return res;
}
