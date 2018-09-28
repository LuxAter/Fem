#include "image/image2d.hpp"
#include "image/image.hpp"
#include "shape.hpp"

fem::image::Image2D::Image2D() : Image() {}
fem::image::Image2D::Image2D(std::size_t width, std::size_t height)
    : Image(width, height) {}

void fem::image::Image2D::Point(int32_t x, int32_t y, uint32_t color) {
  if (x >= 0 && y >= 0 && x < static_cast<int32_t>(width_) &&
      y < static_cast<int32_t>(height_)) {
    SetPixel(x, y, color);
  }
}
void fem::image::Image2D::Point(const std::array<double, 2>& pt, uint32_t color){
  if (pt[0] >= 0 && pt[1] >= 0 && pt[0] < static_cast<int32_t>(width_) &&
      pt[1] < static_cast<int32_t>(height_)) {
    SetPixel(pt[0], pt[1], color);
  }
}

void fem::image::Image2D::Point(const fem::shape::Point& pt, uint32_t color) {
  if (pt.x >= 0 && pt.y >= 0 && pt.x < static_cast<int32_t>(width_) &&
      pt.y < static_cast<int32_t>(height_)) {
    SetPixel(pt.x, pt.y, color);
  }
}

void fem::image::Image2D::Line(int32_t x0, int32_t y0, int32_t x1, int32_t y1,
                               uint32_t color) {
  if (abs(y1 - y0) < abs(x1 - x0)) {
    if (x0 > x1) {
      LineLow(x1, y1, x0, y0, color);
    } else {
      LineLow(x0, y0, x1, y1, color);
    }
  } else {
    if (y0 > y1) {
      LineHeigh(x1, y1, x0, y0, color);
    } else {
      LineHeigh(x0, y0, x1, y1, color);
    }
  }
}

void fem::image::Image2D::Triangle(int32_t x0, int32_t y0, int32_t x1,
                                   int32_t y1, int32_t x2, int32_t y2,
                                   uint32_t color) {
  Line(x0, y0, x1, y1, color);
  Line(x0, y0, x2, y2, color);
  Line(x1, y1, x2, y2, color);
}
void fem::image::Image2D::Triangle(
    const std::array<std::size_t, 3>& tri,
    const std::vector<std::array<double, 2>>& points, uint32_t color) {
  Line(points[tri[0]][0], points[tri[0]][1], points[tri[1]][0],
       points[tri[1]][1], color);
  Line(points[tri[0]][0], points[tri[0]][1], points[tri[2]][0],
       points[tri[2]][1], color);
  Line(points[tri[1]][0], points[tri[1]][1], points[tri[2]][0],
       points[tri[2]][1], color);
}
void fem::image::Image2D::Triangle(const fem::shape::Triangle& tri,
                                   uint32_t color) {
  Line(tri.x0, tri.y0, tri.x1, tri.y1, color);
  Line(tri.x0, tri.y0, tri.x2, tri.y2, color);
  Line(tri.x1, tri.y1, tri.x2, tri.y2, color);
}

void fem::image::Image2D::Rectangle(int32_t x, int32_t y, int32_t w, int32_t h,
                                    uint32_t color) {
  Line(x, y, x + w, y, color);
  Line(x, y, x, y + h, color);
  Line(x + w, y, x + w, y + h, color);
  Line(x, y + h, x + w, y + h, color);
}

void fem::image::Image2D::Circle(int32_t x, int32_t y, int32_t r,
                                 uint32_t color) {
  int vx = r - 1;
  int vy = 0;
  int err = (vx * vx) - (r * r) + r;
  while (vx >= vy) {
    Point(x + vx, y + vy, color);
    Point(x + vy, y + vx, color);
    Point(x - vy, y + vx, color);
    Point(x - vx, y + vy, color);
    Point(x - vx, y - vy, color);
    Point(x - vy, y - vx, color);
    Point(x + vy, y - vx, color);
    Point(x + vx, y - vy, color);
    if (err <= 0) {
      vy++;
      err += 2 * vy + 1;
    } else {
      vx--;
      err -= vx * 2 + 1;
    }
  }
}

void fem::image::Image2D::Circle(const std::array<double, 2>& pt, int32_t r, uint32_t color){
  Circle(pt[0], pt[1], r, color);
}
void fem::image::Image2D::Circle(const fem::shape::Circle& cir,
                                 uint32_t color) {
  Circle(cir.x, cir.y, cir.r, color);
}

void fem::image::Image2D::Polygon(std::vector<int32_t> x,
                                  std::vector<int32_t> y, uint32_t color) {
  for (std::size_t i = 1; i < x.size() && i < y.size(); ++i) {
    Line(x[i - 1], y[i - 1], x[i], y[i], color);
  }
  Line(x[0], y[0], x.back(), y.back(), color);
}

void fem::image::Image2D::Polygon(std::vector<std::array<int32_t, 2>> points,
                                  uint32_t color) {
  for (std::size_t i = 1; i < points.size(); ++i) {
    Line(points[i - 1][0], points[i - 1][1], points[i][0], points[i][1], color);
  }
  Line(points[0][0], points[0][1], points.back()[0], points.back()[1], color);
}

void fem::image::Image2D::LineLow(int32_t x0, int32_t y0, int32_t x1,
                                  int32_t y1, uint32_t color) {
  int dx = x1 - x0;
  int dy = y1 - y0;
  int yi = 1;
  if (dy < 0) {
    yi = -1;
    dy = -dy;
  }
  int d = 2 * dy - dx;
  int y = y0;
  for (int x = x0; x <= x1; ++x) {
    Point(x, y, color);
    if (d > 0) {
      y += yi;
      d -= (2 * dx);
    }
    d += (2 * dy);
  }
}
void fem::image::Image2D::LineHeigh(int32_t x0, int32_t y0, int32_t x1,
                                    int32_t y1, uint32_t color) {
  int dx = x1 - x0;
  int dy = y1 - y0;
  int xi = 1;
  if (dx < 0) {
    xi = -1;
    dx = -dx;
  }
  int d = 2 * dx - dy;
  int x = x0;
  for (int y = y0; y <= y1; ++y) {
    Point(x, y, color);
    if (d > 0) {
      x += xi;
      d -= (2 * dy);
    }
    d += (2 * dx);
  }
}
