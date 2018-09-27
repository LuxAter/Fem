#include "image/image.hpp"

#include <cmath>
#include <cstdint>
#include <string>
#include <vector>

#include <png.h>

#include <iostream>

fem::image::Image::Image() : width_(0), height_(0) {}
fem::image::Image::Image(std::size_t width, std::size_t height)
    : width_(width),
      height_(height),
      frame_buffer_(width_, std::vector<uint32_t>(height_, 0)) {}
fem::image::Image::Image(const Image& copy)
    : width_(copy.width_),
      height_(copy.height_),
      frame_buffer_(copy.frame_buffer_) {}

void fem::image::Image::SetPixel(std::size_t x, std::size_t y, uint32_t rgb) {
  frame_buffer_[x][y] = rgb;
}

void fem::image::Image::Fill(uint32_t rgb) {
  frame_buffer_ = std::vector<std::vector<uint32_t>>(
      width_, std::vector<uint32_t>(height_, rgb));
}

bool fem::image::Image::WritePng(const std::string& file_path) {
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
