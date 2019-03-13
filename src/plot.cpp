#include "plot.hpp"

#include <array>
#include <string>
#include <vector>

#include <cmath>
#include <cstdlib>
#include <cstring>

#include "cmaps.hpp"

void fem::plot::imsave(std::string file, std::vector<std::vector<double>> vals,
                       std::string cmap, double v_min, double v_max) {
  unsigned w = vals.front().size(), h = vals.size();
  uint32_t** buffer;
  buffer = (uint32_t**)malloc(h * sizeof(uint32_t*));
  if (buffer == NULL) return;
  for (uint32_t i = 0; i < h; ++i) {
    buffer[i] = (uint32_t*)malloc(w * sizeof(uint32_t));
    if (buffer[i] == NULL) {
      for (uint32_t j = 0; j < i; ++j) {
        free(buffer[j]);
      }
      free(buffer);
      return;
    }
    for (uint32_t j = 0; j < w; ++j) {
      buffer[i][j] = 0xffffff;
    }
  }

  std::array<unsigned, 256> color_map = _cmaps[cmap];

  if (std::isinf(v_min) || std::isinf(v_max)) {
    bool min_v = std::isinf(v_min), max_v = std::isinf(v_max);
    for (auto& row : vals) {
      for (auto& val : row) {
        if (std::isnan(val)) continue;
        if (min_v) v_min = std::min(v_min, val);
        if (max_v) v_max = std::max(v_max, val);
      }
    }
  }
  for (std::size_t i = 0; i < h; ++i) {
    for (std::size_t j = 0; j < w; ++j) {
      if (std::isnan(vals[i][j])) {
        buffer[i][j] = 0x000000;
        continue;
      }
      buffer[i][j] = color_map[static_cast<unsigned>(
          clamp(vals[i][j], v_min, v_max, 0.0, 255.0))];
    }
  }
  if (file.find(".bmp") != std::string::npos) {
    write_bmp(file, buffer, w, h);
  }
  for (uint32_t i = 0; i < h; ++i) {
    free(buffer[i]);
  }
  free(buffer);
}

void fem::plot::write_bmp(const std::string file, uint32_t** buffer, unsigned w,
                          unsigned h) {
  unsigned file_size = 54 + 3 * w * h;

  uint8_t* byte_data = (uint8_t*)malloc(3 * w * h);
  if (byte_data == NULL) {
    return;
  }
  std::memset(byte_data, 0, 3 * w * h);
  for (unsigned x = 0; x < w; ++x) {
    for (unsigned y = 0; y < h; ++y) {
      unsigned x_ind = x, y_ind = (h - 1) - y;
      uint8_t r = ((buffer[y][x] >> 16) & 0XFF);
      uint8_t g = ((buffer[y][x] >> 8) & 0XFF);
      uint8_t b = ((buffer[y][x]) & 0XFF);
      byte_data[(x_ind + y_ind * w) * 3 + 0] = b;
      byte_data[(x_ind + y_ind * w) * 3 + 1] = g;
      byte_data[(x_ind + y_ind * w) * 3 + 2] = r;
    }
  }

  unsigned char file_header[14] = {'B', 'M', 0, 0,  0, 0, 0,
                                   0,   0,   0, 54, 0, 0, 0};
  unsigned char info_header[40] = {40, 0, 0, 0, 0, 0, 0,  0,
                                   0,  0, 0, 0, 1, 0, 24, 0};
  unsigned char pad[3] = {0, 0, 0};

  file_header[2] = (unsigned char)(file_size);
  file_header[3] = (unsigned char)(file_size >> 8);
  file_header[4] = (unsigned char)(file_size >> 14);
  file_header[5] = (unsigned char)(file_size >> 24);

  info_header[4] = (unsigned char)(w);
  info_header[5] = (unsigned char)(w >> 8);
  info_header[6] = (unsigned char)(w >> 16);
  info_header[7] = (unsigned char)(w >> 24);
  info_header[8] = (unsigned char)(h);
  info_header[9] = (unsigned char)(h >> 8);
  info_header[10] = (unsigned char)(h >> 16);
  info_header[11] = (unsigned char)(h >> 24);

  FILE* out = fopen(file.c_str(), "wb");
  if (out == NULL) {
    free(byte_data);
    return;
  }
  fwrite(file_header, 1, 14, out);
  fwrite(info_header, 1, 40, out);
  for (unsigned i = 0; i < h; ++i) {
    fwrite(byte_data + (w * (h - i - 1) * 3), 3, w, out);
    fwrite(pad, 1, (4 - (w * 3) % 4) % 4, out);
  }
  free(byte_data);
  fclose(out);
}

double fem::plot::clamp(double val, double in_min, double in_max,
                        double out_min, double out_max) {
  if (val <= in_min)
    return out_min;
  else if (val >= in_max)
    return out_max;
  return out_min + ((out_max - out_min) / (in_max - in_min)) * (val - in_min);
}
