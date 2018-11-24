#include "image/figure.hpp"

#include <cmath>
#include <future>
#include <limits>

#include <png.h>

#include "image/_cmaps.hpp"
#include "mesh/mesh.hpp"

#include <iostream>

bool WritePngFull(std::string file_path, std::size_t width_,
                  std::size_t height_,
                  std::vector<std::vector<uint32_t>> frame_buffer_) {
  FILE* out = fopen(file_path.c_str(), "wb");
  if (!out) {
    return false;
  }
  png_structp png =
      png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
  if (!png) {
    fclose(out);
    return false;
  }
  png_infop info = png_create_info_struct(png);
  if (!info) {
    fclose(out);
    return false;
  }
  if (setjmp(png_jmpbuf(png))) {
    fclose(out);
    return false;
  }
  uint8_t** byte_data = (png_bytepp)malloc(height_ * sizeof(png_bytep));
  for (std::size_t i = 0; i < width_; ++i) {
    byte_data[i] = (png_bytep)malloc(6 * width_ * sizeof(png_byte));
  }
  for (std::size_t y = 0; y < height_; ++y) {
    for (std::size_t x = 0; x < width_; ++x) {
      double r = ((frame_buffer_[x][y] >> 16) & 0xff) * 256.0;
      double g = ((frame_buffer_[x][y] >> 8) & 0xff) * 256.0;
      double b = (frame_buffer_[x][y] & 0xff) * 256.0;
      std::size_t id = 6 * x;
      byte_data[y][id + 0] = static_cast<uint8_t>(r / 256.0);
      byte_data[y][id + 1] = static_cast<uint8_t>(r) % 256;
      byte_data[y][id + 2] = static_cast<uint8_t>(g / 256.0);
      byte_data[y][id + 3] = static_cast<uint8_t>(g) % 256;
      byte_data[y][id + 4] = static_cast<uint8_t>(b / 256.0);
      byte_data[y][id + 5] = static_cast<uint8_t>(b) % 256;
    }
  }
  png_init_io(png, out);
  png_set_IHDR(png, info, width_, height_, 16, PNG_COLOR_TYPE_RGB,
               PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT,
               PNG_FILTER_TYPE_DEFAULT);
  png_write_info(png, info);
  png_write_image(png, byte_data);
  png_write_end(png, NULL);
  png_free_data(png, info, PNG_FREE_ALL, -1);
  png_destroy_write_struct(&png, (png_infopp)NULL);
  fclose(out);
  return true;
}

fem::image::Figure::Figure(const uint64_t& w, const uint64_t& h)
    : width_(w), height_(h) {}
fem::image::Figure::Figure(const Figure& copy)
    : width_(copy.width_),
      height_(copy.height_),
      cmap_(copy.cmap_),
      spacing_(copy.spacing_),
      background_(copy.background_),
      bounds_(copy.bounds_),
      data_(copy.data_),
      overlay_(copy.overlay_) {}

void fem::image::Figure::SetCmap(const std::string& cmap) { cmap_ = cmap; }

void fem::image::Figure::SetSpacing(const double& spacing) {
  spacing_ = spacing;
}
void fem::image::Figure::SetBackground(const uint32_t& rgb) {
  background_ = rgb;
}

void fem::image::Figure::Rectangle(
    const std::function<double(double, double)>& func, const double& x,
    const double& y, const double& w, const double& h, double vmin,
    double vmax) {
  double step_size_x = (w / width_), step_size_y = (h / height_);
  data_.clear();
  bounds_ = {{INFINITY, -INFINITY}};
  for (double i = y; i <= y + h; i += step_size_y) {
    std::vector<double> row;
    for (double j = x; j <= x + w; j += step_size_x) {
      double z_val = func(j, i);
      if (z_val > vmax) {
        z_val = vmax;
      }
      if (z_val < vmin) {
        z_val = vmin;
      }
      bounds_[0] = std::min(z_val, bounds_[0]);
      bounds_[1] = std::max(z_val, bounds_[1]);
      row.push_back(z_val);
    }
    data_.push_back(row);
  }
}
void fem::image::Figure::Circle(
    const std::function<double(double, double)>& func, const double& cx,
    const double& cy, const double& r, double vmin, double vmax) {
  data_.clear();
  bounds_ = {{INFINITY, -INFINITY}};
  double step_size_x = ((r * 2) / width_), step_size_y = ((r * 2) / height_);
  for (double i = -r; i <= r; i += step_size_y) {
    std::vector<double> row;
    for (double j = -r; j <= r; j += step_size_x) {
      if (pow(i, 2) + pow(j, 2) <= pow(r, 2)) {
        double z_val = func(sqrt(pow(j, 2) + pow(i, 2)), atan2(i, j));
        if (z_val > vmax) {
          z_val = vmax;
        }
        if (z_val < vmin) {
          z_val = vmin;
        }
        bounds_[0] = std::min(z_val, bounds_[0]);
        bounds_[1] = std::max(z_val, bounds_[1]);
        row.push_back(z_val);
      } else {
        row.push_back(std::numeric_limits<double>::quiet_NaN());
      }
    }
    data_.push_back(row);
  }
}

void fem::image::Figure::Mesh(const std::function<double(double, double)>& func,
                              const fem::mesh::Mesh& mesh, double vmin,
                              double vmax) {
  std::array<double, 2> bounds_x{{INFINITY, -INFINITY}};
  std::array<double, 2> bounds_y{{INFINITY, -INFINITY}};
  for (auto& pt : mesh.points) {
    bounds_x[0] = std::min(bounds_x[0], pt[0]);
    bounds_y[0] = std::min(bounds_y[0], pt[1]);
    bounds_x[1] = std::max(bounds_x[1], pt[0]);
    bounds_y[1] = std::max(bounds_y[1], pt[1]);
  }
  double step_size_x = ((bounds_x[1] - bounds_x[0]) / width_),
         step_size_y = ((bounds_y[1] - bounds_y[0]) / height_);
  data_.clear();
  bounds_ = {{INFINITY, -INFINITY}};
  for (double i = bounds_y[0]; i <= bounds_y[1]; i += step_size_y) {
    std::vector<double> row;
    for (double j = bounds_x[0]; j <= bounds_x[1]; j += step_size_x) {
      if (mesh.InMesh({{j, i}})) {
        double z_val = func(j, i);
        if (z_val > vmax) {
          z_val = vmax;
        }
        if (z_val < vmin) {
          z_val = vmin;
        }
        bounds_[0] = std::min(z_val, bounds_[0]);
        bounds_[1] = std::max(z_val, bounds_[1]);
        row.push_back(z_val);
      } else {
        row.push_back(std::numeric_limits<double>::quiet_NaN());
      }
    }
    data_.push_back(row);
  }
}

void fem::image::Figure::OverlayCircle(int cx, int cy, int r, uint32_t color) {
  int rsq = r * r;
  for (int y = -r; y <= r; ++y) {
    for (int x = -r; x <= r; ++x) {
      if (x * x + y * y < rsq) {
        overlay_[LimitPixel(cy + y, 0, height_)]
                [LimitPixel(cx + x, 0, width_)] = color;
      }
    }
  }
}
void fem::image::Figure::OverlayLine(int x0, int y0, int x1, int y1,
                                     uint8_t size, uint32_t color) {
  if (abs(y1 - y0) < abs(x1 - x0)) {
    if (x0 > x1) {
      OverlayLineLow(x1, y1, x0, y0, size, color);
    } else {
      OverlayLineLow(x0, y0, x1, y1, size, color);
    }
  } else {
    if (y0 > y1) {
      OverlayLineHigh(x1, y1, x0, y0, size, color);
    } else {
      OverlayLineHigh(x0, y0, x1, y1, size, color);
    }
  }
}
void fem::image::Figure::OverlayLineHigh(int x0, int y0, int x1, int y1,
                                         uint8_t size, uint32_t color) {
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
    OverlayCircle(x, y, size, color);
    if (d > 0) {
      x += xi;
      d -= (2 * dy);
    }
    d += (2 * dx);
  }
}
void fem::image::Figure::OverlayLineLow(int x0, int y0, int x1, int y1,
                                        uint8_t size, uint32_t color) {
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
    OverlayCircle(x, y, size, color);
    if (d > 0) {
      y += yi;
      d -= (2 * dx);
    }
    d += (2 * dy);
  }
}

void fem::image::Figure::OverlayGrid(uint32_t x_steps, uint32_t y_steps,
                                     uint8_t size, uint32_t color) {
  overlay_ = std::vector<std::vector<int64_t>>(
      height_, std::vector<int64_t>(width_, -1));
  for (int64_t x = 0; x < width_; x += (width_ / x_steps)) {
    for (int64_t y = 0; y < height_; ++y) {
      for (int64_t sx = -size; sx < size; ++sx) {
        overlay_[y][LimitPixel(x + sx, 0, width_)] = color;
      }
    }
  }
  for (int64_t y = 0; y < height_; y += (height_ / y_steps)) {
    for (int64_t x = 0; x < width_; ++x) {
      for (int64_t sx = -size; sx < size; ++sx) {
        overlay_[LimitPixel(y + sx, 0, height_)][x] = color;
      }
    }
  }
}
void fem::image::Figure::OverlayMesh(fem::mesh::Mesh& mesh, uint8_t size,
                                     uint32_t color) {
  overlay_ = std::vector<std::vector<int64_t>>(
      height_, std::vector<int64_t>(width_, -1));
  std::array<double, 2> bounds_x{{INFINITY, -INFINITY}};
  std::array<double, 2> bounds_y{{INFINITY, -INFINITY}};
  mesh.DeterminEdges();
  for (auto& pt : mesh.points) {
    bounds_x[0] = std::min(bounds_x[0], pt[0]);
    bounds_y[0] = std::min(bounds_y[0], pt[1]);
    bounds_x[1] = std::max(bounds_x[1], pt[0]);
    bounds_y[1] = std::max(bounds_y[1], pt[1]);
  }
  for (auto& tri : mesh.triangles) {
    for (uint8_t i = 0; i < 3; ++i) {
      uint8_t j = (i + 1) % 3;
      std::array<double, 2> v1 = mesh.points[tri[i]], v2 = mesh.points[tri[j]];
      int64_t x0 = width_/(bounds_x[1] - bounds_x[0]) * (v1[0] - bounds_x[0]);
      int64_t y0 = height_/(bounds_y[1] - bounds_y[0]) * (v1[1] - bounds_y[0]);
      int64_t x1 = width_/(bounds_x[1] - bounds_x[0]) * (v2[0] - bounds_x[0]);
      int64_t y1 = height_/(bounds_y[1] - bounds_y[0]) * (v2[1] - bounds_y[0]);
      OverlayLine(x0, y0, x1, y1, size, color);
    }
  }
}

#ifdef _REENTRANT
std::future<bool> fem::image::Figure::WritePng(const std::string& file_path) {
  std::vector<std::vector<uint32_t>> frame_buffer_ = GenFrameBuffer();
  return std::async(std::launch::async, WritePngFull, file_path, width_,
                    height_, frame_buffer_);
}
#else
bool fem::image::Figure::WritePng(const std::string& file_path) {
  std::vector<std::vector<uint32_t>> frame_buffer_ = GenFrameBuffer();
  return WritePngFull(file_path, width_, height_, frame_buffer_);
}
#endif

bool fem::image::Figure::WritePngWait(const std::string& file_path) {
  std::vector<std::vector<uint32_t>> frame_buffer_ = GenFrameBuffer();
  return WritePngFull(file_path, width_, height_, frame_buffer_);
}

std::array<double, 3> fem::image::Figure::CmapLookUp(
    const double& z, const std::vector<std::array<double, 3>>& cmap) const {
  uint64_t index = static_cast<uint64_t>(
      (cmap.size() - 1) / (bounds_[1] - bounds_[0]) * (z - bounds_[0]));
  return cmap[index];
}

std::vector<std::array<double, 3>> fem::image::Figure::LoadCmap(
    const std::string& cmap) const {
  if (cmap == "magam") {
    return fem::image::_cmap::magma;
  } else if (cmap == "inferno") {
    return fem::image::_cmap::inferno;
  } else if (cmap == "plasma") {
    return fem::image::_cmap::plasma;
  } else {
    return fem::image::_cmap::viridis;
  }
}

std::vector<std::vector<uint32_t>> fem::image::Figure::GenFrameBuffer() const {
  std::vector<std::vector<uint32_t>> frame_buffer(
      width_, std::vector<uint32_t>(height_, background_));
  std::vector<std::array<double, 3>> cmap = LoadCmap(cmap_);
  for (uint64_t x = 0; x < width_; ++x) {
    for (uint64_t y = 0; y < height_; ++y) {
      uint64_t fb_x = static_cast<uint64_t>((spacing_ * width_) +
                                            (1.0 - 2.0 * spacing_) *
                                                static_cast<double>(x));
      uint64_t fb_y = static_cast<uint64_t>((spacing_ * height_) +
                                            (1.0 - 2.0 * spacing_) *
                                                static_cast<double>(y));
      if (overlay_.size() != 0 && overlay_[y][x] != -1) {
        frame_buffer[fb_x][fb_y] = overlay_[y][x];
      } else if (!std::isnan(data_[y][x])) {
        std::array<double, 3> color = CmapLookUp(data_[y][x], cmap);
        frame_buffer[fb_x][fb_y] = ConvertRgb(color);
      }
    }
  }
  return frame_buffer;
}
