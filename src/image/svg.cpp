#include "image/svg.hpp"

#include <cmath>
#include <cstdint>
#include <future>
#include <map>
#include <string>
#include <vector>

#include "mesh/mesh.hpp"
#include "mesh/pslg.hpp"

#include <iostream>

bool WriteSvgFull(std::string file_path, std::size_t width_,
                  std::size_t height_,
                  std::vector<std::map<std::string, std::string>> elements_) {
  FILE* out = fopen(file_path.c_str(), "w");
  if (!out) {
    return false;
  }
  fprintf(out, "<svg width=\"%lu\" height=\"%lu\">\n", width_, height_);
  for (auto& it : elements_) {
    std::string element = "  <" + it["type"] + " ";
    if(it["type"] != "text"){
      for (auto& attr : it) {
        if (attr.first == "type") continue;
        element += attr.first + "=\"" + attr.second + "\" ";
      }
      fprintf(out, "%s/>\n", element.c_str());
    }else{
      for (auto& attr : it) {
        if (attr.first == "type" || attr.first == "string") continue;
        element += attr.first + "=\"" + attr.second + "\" ";
      }
      fprintf(out, "%s>%s</text>", element.c_str(), it["string"].c_str());
    }
  }
  fprintf(out, "</svg>");
  fclose(out);
  return false;
}

fem::image::Svg::Svg() : width_(0), height_(0) {}
fem::image::Svg::Svg(std::size_t width, std::size_t height)
    : width_(width), height_(height) {}
fem::image::Svg::Svg(const Svg& copy)
    : width_(copy.width_), height_(copy.height_), elements_(copy.elements_) {}

void fem::image::Svg::Fill(std::string color) {
  elements_.clear();
  elements_.push_back({{"type", "rect"},
                       {"x", "0"},
                       {"y", "0"},
                       {"width", std::to_string(width_)},
                       {"height", std::to_string(height_)},
                       {"fill", color}});
}

void fem::image::Svg::Point(uint32_t x, uint32_t y, std::string color) {
  elements_.push_back({{"type", "circle"},
                       {"cx", std::to_string(x)},
                       {"cy", std::to_string(y)},
                       {"r", "1"},
                       {"stroke", color},
                       {"fill", color}});
}
void fem::image::Svg::Line(uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1,
                           std::string color, uint32_t stroke, uint32_t dash) {
  elements_.push_back({{"type", "line"},
                       {"x1", std::to_string(x0)},
                       {"y1", std::to_string(y0)},
                       {"x2", std::to_string(x1)},
                       {"y2", std::to_string(y1)},
                       {"stroke", color},
                       {"stroke-width", std::to_string(stroke)},
                       {"stroke-dasharray", std::to_string(dash)}});
}
void fem::image::Svg::Circle(uint32_t x, uint32_t y, uint32_t r,
                             std::string color, std::string fill,
                             uint32_t stroke) {
  elements_.push_back({{"type", "circle"},
                       {"cx", std::to_string(x)},
                       {"cy", std::to_string(y)},
                       {"r", std::to_string(r)},
                       {"stroke", color},
                       {"fill", fill},
                       {"stoke-width", std::to_string(stroke)}});
}
void fem::image::Svg::Triangle(uint32_t x0, uint32_t y0, uint32_t x1,
                               uint32_t y1, uint32_t x2, uint32_t y2,
                               std::string color, std::string fill,
                               uint32_t stroke) {
  elements_.push_back(
      {{"type", "polygon"},
       {"points", std::to_string(x0) + ',' + std::to_string(y0) + ' ' +
                      std::to_string(x1) + ',' + std::to_string(y1) + ' ' +
                      std::to_string(x2) + ',' + std::to_string(y2)},
       {"stroke", color},
       {"fill", fill},
       {"stroke-width", std::to_string(stroke)}});
}
void fem::image::Svg::Triangle(uint32_t x0, uint32_t y0, uint32_t x1,
                               uint32_t y1, uint32_t x2, uint32_t y2,
                               uint32_t stroke, std::string color) {
  Triangle(x0, y0, x1, y1, x2, y2, color, "none", stroke);
}
void fem::image::Svg::Triangle(const std::array<std::size_t, 3>& tri,
                               const std::vector<std::array<double, 2>>& points,
                               std::string color, std::string fill,
                               uint32_t stroke) {
  Triangle(points[tri[0]][0], points[tri[0]][1], points[tri[1]][0],
           points[tri[1]][1], points[tri[2]][0], points[tri[2]][1], color, fill,
           stroke);
}
void fem::image::Svg::Triangle(const std::array<std::size_t, 3>& tri,
                               const std::vector<std::array<double, 2>>& points,
                               uint32_t stroke, std::string color) {
  Triangle(points[tri[0]][0], points[tri[0]][1], points[tri[1]][0],
           points[tri[1]][1], points[tri[2]][0], points[tri[2]][1], color,
           "none", stroke);
}
void fem::image::Svg::Rectangle(uint32_t x, uint32_t y, uint32_t w, uint32_t h,
                                std::string color, std::string fill,
                                uint32_t stroke) {
  elements_.push_back({{"type", "rect"},
                       {"x", std::to_string(x)},
                       {"y", std::to_string(y)},
                       {"width", std::to_string(w)},
                       {"height", std::to_string(h)},
                       {"stroke", color},
                       {"fill", fill},
                       {"stroke-width", std::to_string(stroke)}});
}

void fem::image::Svg::PolyLine(std::vector<std::array<double, 2>> points,
                               std::string color, uint32_t stroke,
                               uint32_t dash) {
  std::string points_string;
  for (auto& it : points) {
    points_string += std::to_string(static_cast<uint32_t>(it[0])) + ',' +
                     std::to_string(static_cast<uint32_t>(it[1])) + ' ';
  }
  elements_.push_back({{"type", "polyline"},
                       {"points", points_string},
                       {"stroke", color},
                       {"fill", "none"},
                       {"stroke-width", std::to_string(stroke)},
                       {"stroke-dasharray", std::to_string(dash)}});
}

void fem::image::Svg::Text(std::string msg, uint32_t x, uint32_t y,
                           std::string color, std::string anchor) {
  elements_.push_back({{"type", "text"},
                       {"x", std::to_string(x)},
                       {"y", std::to_string(y)},
                       {"fill", color},
                       {"text-anchor", anchor},
                       {"string", msg}});
}

void fem::image::Svg::Pslg(mesh::Pslg pslg, std::string edge, std::string point,
                           std::string hole, uint32_t stroke) {
  for (auto& eg : pslg.edges) {
    Line(pslg.points[eg[0]][0], pslg.points[eg[0]][1], pslg.points[eg[1]][0],
         pslg.points[eg[1]][1], edge, stroke);
  }
  if (stroke == 1) {
    stroke = 2;
  }
  for (auto& pt : pslg.points) {
    Circle(pt[0], pt[1], stroke / 2, point, point);
  }
  for (auto& pt : pslg.holes) {
    Circle(pt[0], pt[1], stroke / 2, hole, hole);
  }
}

void fem::image::Svg::Mesh(mesh::Mesh mesh, std::string edge,
                           std::string vertex, uint32_t stroke, uint32_t dash,
                           bool label) {
  mesh.DeterminEdges();
  for (auto& eg : mesh.edges) {
    Line(mesh.points[eg[0]][0], mesh.points[eg[0]][1], mesh.points[eg[1]][0],
         mesh.points[eg[1]][1], edge, stroke, dash);
  }
  if (label) {
    for (std::size_t i = 0; i < mesh.triangles.size(); ++i) {
      double cx = (mesh.points[mesh.triangles[i][0]][0] +
                   mesh.points[mesh.triangles[i][1]][0] +
                   mesh.points[mesh.triangles[i][2]][0]) /
                  3.0;
      double cy = (mesh.points[mesh.triangles[i][0]][1] +
                   mesh.points[mesh.triangles[i][1]][1] +
                   mesh.points[mesh.triangles[i][2]][1]) /
                  3.0;
      Text(std::to_string(i), cx, cy);
    }
  }
  if (stroke == 1) {
    stroke = 2;
  }
  for (auto& pt : mesh.points) {
    Circle(pt[0], pt[1], stroke / 2, vertex, vertex);
  }
}

void fem::image::Svg::Triangles(std::vector<std::array<uint64_t, 3>>& triangles,
                                std::vector<std::array<double, 2>>& points,
                                std::string edge, std::string vertex,
                                uint32_t stroke, uint32_t dash, double scale,
                                double shift) {
  std::vector<std::array<uint64_t, 2>> edges;
  for (auto& tri : triangles) {
    for (uint64_t i = 0; i < 3; ++i) {
      uint64_t j = (i + 1) % 3;
      bool present = false;
      for (uint64_t k = 0; k < edges.size() && present == false; ++k) {
        if ((edges[k][0] == tri[i] && edges[k][1] == tri[j]) ||
            (edges[k][1] == tri[i] && edges[k][0] == tri[j])) {
          present = true;
        }
      }
      if (present == false) {
        edges.push_back({{tri[i], tri[j]}});
      }
    }
  }
  for (auto& eg : edges) {
    Line((scale * points[eg[0]][0]) + shift, (scale * points[eg[0]][1]) + shift,
         (scale * points[eg[1]][0]) + shift, (scale * points[eg[1]][1]) + shift,
         edge, stroke, dash);
  }
  if (stroke == 1) {
    stroke = 2;
  }
  for (auto& pt : points) {
    Circle((scale * pt[0]) + shift, (scale * pt[1]) + shift, stroke / 2, vertex,
           vertex);
  }
}

#ifdef _REENTRANT
std::future<bool> fem::image::Svg::WriteSvg(const std::string& file_path) {
  return std::async(std::launch::async, WriteSvgFull, file_path, width_,
                    height_, elements_);
}
#else
bool fem::image::Svg::WriteSvg(const std::string& file_path) {
  return WriteSvgFull(file_path, width_, height_, elements_);
}
#endif
bool fem::image::Svg::WriteSvgWait(const std::string& file_path) {
  return WriteSvgFull(file_path, width_, height_, elements_);
}
