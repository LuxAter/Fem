#include "image/png.hpp"

#include <cmath>
#include <string>

#include <png.h>

#include "mesh/pslg.hpp"

static uint32_t* pixel_data_ = nullptr;
static unsigned width_ = 0, height_ = 0;

void fem::image::PngInitalizeData(const unsigned& w, const unsigned& h) {
  if (pixel_data_ != nullptr) {
    free(pixel_data_);
  }
  pixel_data_ = (uint32_t*)std::malloc(w * h * sizeof(uint32_t));
  std::fill(pixel_data_, pixel_data_ + (w * h), 0xffffff);
  width_ = w;
  height_ = h;
}
void fem::image::PngFreeData() {
  free(pixel_data_);
  pixel_data_ = nullptr;
  width_ = 0;
  height_ = 0;
}
bool fem::image::PngWriteData(const std::string& file_name) {
  FILE* out = fopen(file_name.c_str(), "wb");
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
  uint8_t** byte_data = (png_bytepp)std::malloc(height_ * sizeof(png_bytep));
  for (unsigned i = 0; i < height_; ++i) {
    byte_data[i] = (png_bytep)std::malloc(6 * width_ * sizeof(png_byte));
  }
  for (unsigned y = 0; y < height_; ++y) {
    for (unsigned x = 0; x < width_; ++x) {
      uint32_t red = ((pixel_data_[y * width_ + x] >> 16) & 0xFF) * 256;
      uint32_t green = ((pixel_data_[y * width_ + x] >> 8) & 0xFF) * 256;
      uint32_t blue = ((pixel_data_[y * width_ + x]) & 0xFF) * 256;
      uint32_t id = x * 6;
      byte_data[y][id] =
          static_cast<uint8_t>(std::floor(static_cast<double>(red) / 256.0));
      byte_data[y][id + 1] = static_cast<uint8_t>(red % 256);
      byte_data[y][id + 2] =
          static_cast<uint8_t>(std::floor(static_cast<double>(green) / 256.0));
      byte_data[y][id + 3] = static_cast<uint8_t>(green % 256);
      byte_data[y][id + 4] =
          static_cast<uint8_t>(std::floor(static_cast<double>(blue) / 256.0));
      byte_data[y][id + 5] = static_cast<uint8_t>(blue % 256);
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

void fem::image::PngDrawPoint(int x, int y, uint32_t c, uint8_t w) {
  if (w == 1) {
    if (x > 0 && y > 0 && x < static_cast<int>(width_) &&
        y < static_cast<int>(height_)) {
      pixel_data_[y * width_ + x] = c;
    }
    return;
  }
  for (int px = x - w; px < x + w; ++px) {
    for (int py = y - w; py < y + 2; ++py) {
      if (px > 0 && py > 0 && px < static_cast<int>(width_) &&
          py < static_cast<int>(height_)) {
        pixel_data_[py * width_ + px] = c;
      }
    }
  }
}
void fem::image::PngDrawLineLow(int x0, int y0, int x1, int y1, uint32_t c,
                                uint8_t w) {
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
    PngDrawPoint(x, y, c, w);
    if (d > 0) {
      y += yi;
      d -= (2 * dx);
    }
    d += (2 * dy);
  }
}
void fem::image::PngDrawLineHigh(int x0, int y0, int x1, int y1, uint32_t c,
                                 uint8_t w) {
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
    PngDrawPoint(x, y, c, w);
    if (d > 0) {
      x += xi;
      d -= (2 * dy);
    }
    d += (2 * dx);
  }
}
void fem::image::PngDrawLine(int x0, int y0, int x1, int y1, uint32_t c,
                             uint8_t w) {
  if (labs(y1 - y0) < labs(x1 - x0)) {
    if (x0 > x1) {
      PngDrawLineLow(x1, y1, x0, y0, c, w);
    } else {
      PngDrawLineLow(x0, y0, x1, y1, c, w);
    }
  } else {
    if (y0 > y1) {
      PngDrawLineHigh(x1, y1, x0, y0, c, w);
    } else {
      PngDrawLineHigh(x0, y0, x1, y1, c, w);
    }
  }
}
void fem::image::PngDrawCircle(int cx, int cy, unsigned r, uint32_t c, int w) {
  if (w > 0) {
    int x = r - 1;
    int y = 0;
    int err = (x * x) - (r * r) + r;
    while (x >= y) {
      PngDrawPoint(cx + x, cy + y, c, w);
      PngDrawPoint(cx - x, cy + y, c, w);
      PngDrawPoint(cx + y, cy + x, c, w);
      PngDrawPoint(cx - y, cy + x, c, w);
      PngDrawPoint(cx - y, cy - x, c, w);
      PngDrawPoint(cx + y, cy - x, c, w);
      PngDrawPoint(cx - x, cy - y, c, w);
      PngDrawPoint(cx + x, cy - y, c, w);
      if (err <= 0) {
        y++;
        err += 2 * y + 1;
      } else {
        x--;
        err -= x * 2 + 1;
      }
    }
  } else if (w < 0) {
    int x = r - 1;
    int y = 0;
    int err = (x * x) - (r * r) + r;
    while (x >= y) {
      for (int i = cx - x; i <= cx + x; ++i) {
        PngDrawPoint(i, cy + y, c, 1);
      }
      for (int i = cx - y; i <= cx + y; ++i) {
        PngDrawPoint(i, cy + x, c, 1);
      }
      for (int i = cx - y; i <= cx + y; ++i) {
        PngDrawPoint(i, cy - x, c, 1);
      }
      for (int i = cx - x; i <= cx + x; ++i) {
        PngDrawPoint(i, cy - y, c, 1);
      }
      if (err <= 0) {
        y++;
        err += 2 * y + 1;
      } else {
        x--;
        err -= x * 2 + 1;
      }
    }
  }
}

bool fem::image::WritePng(const std::string& file_name, const unsigned& w,
                          const unsigned& h, const PSLG& pslg) {
  PngInitalizeData(w, h);
  std::vector<Pt> scaled_points = pslg.points;
  std::vector<Pt> scaled_holes = pslg.holes;
  double x = w / 2.0;
  double y = h / 2.0;
  for (auto& pt : scaled_points) {
    pt.x = (pt.x * (x * 0.98)) + x;
    pt.y = (pt.y * (y * 0.98)) + y;
    PngDrawCircle(pt.x, pt.y, 2, 0);
  }
  for(auto& edg : pslg.edges){
    PngDrawLine(scaled_points[edg.x].x, scaled_points[edg.x].y, scaled_points[edg.y].x, scaled_points[edg.y].y, 0);
  }
  for (auto& pt : scaled_holes) {
    pt.x = (pt.x * x) + x;
    pt.y = (pt.y * y) + y;
    PngDrawCircle(pt.x, pt.y, 2, 0xff0000);
  }
  bool res = PngWriteData(file_name);
  PngFreeData();
  return res;
}
