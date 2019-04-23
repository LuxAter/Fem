#include "plot.hpp"

#include <array>
#include <string>
#include <vector>

#include <cmath>
#include <cstdlib>
#include <cstring>

#include "linalg.hpp"
#include "logger.hpp"
#include "mesh.hpp"
#include "timer.hpp"

#include "cmaps.hpp"

void arta::plot_grid(const std::string& dest, const unsigned& w,
                     const unsigned& h,
                     const std::vector<std::vector<double>>& vals,
                     std::string cmap) {
  std::array<unsigned, 256> color_map = _cmaps[cmap];
  uint32_t** buffer = (uint32_t**)malloc(h * sizeof(uint32_t*));
  for (unsigned i = 0; i < h; ++i) {
    buffer[i] = (uint32_t*)malloc(w * sizeof(uint32_t));
  }

  double v_min = std::numeric_limits<double>::infinity();
  double v_max = -std::numeric_limits<double>::infinity();
  for (auto& row : vals) {
    for (auto& val : row) {
      if (std::isnan(val)) continue;
      v_min = std::min(v_min, val);
      v_max = std::max(v_max, val);
    }
  }

  for (unsigned i = 0; i < h; ++i) {
    for (unsigned j = 0; j < w; ++j) {
      if (std::isnan(vals[i][j])) {
        buffer[i][j] = 0xFFFFFF;
        continue;
      }
      buffer[i][j] = color_map[static_cast<unsigned>(
          clamp(vals[i][j], v_min, v_max, 0.0, 255.0))];
    }
  }

  if (dest.find(".bmp") != std::string::npos) {
    write_bmp(dest, buffer, w, h);
  }

  for (unsigned i = 0; i < h; ++i) {
    free(buffer[i]);
  }
  free(buffer);
}

void arta::plot(const std::string& dest, const PDE* pde, const unsigned& w,
                const unsigned& h, std::string cmap) {
  if (pde->timer) {
    time::start();
  }
  std::vector<std::vector<double>> vals;
  double stepx = (pde->mesh.bounds[2] - pde->mesh.bounds[0]) / w;
  double stepy = (pde->mesh.bounds[3] - pde->mesh.bounds[1]) / h;
  for (double y = pde->mesh.bounds[1]; y < pde->mesh.bounds[3]; y += stepy) {
    vals.push_back(std::vector<double>());
    for (double x = pde->mesh.bounds[0]; x < pde->mesh.bounds[2]; x += stepx) {
      int t = pde->mesh.locate(x, y);
      if (t < 0) {
        vals.back().push_back(std::numeric_limits<double>::quiet_NaN());
        continue;
      }
      // vals.back().push_back(t);
      vals.back().push_back(pde->approx(x, y, t));
    }
  }
  if (pde->timer) {
    log::status("Evaluated Approximation: %f", time::stop());
    time::start();
  }
  plot_grid(dest, w, h, vals, cmap);
  if (pde->timer) {
    log::status("Plot Approximation: %f", time::stop());
  }
}

void arta::write_bmp(const std::string file, uint32_t** buffer, unsigned w,
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

double arta::clamp(double val, double in_min, double in_max, double out_min,
                   double out_max) {
  if (val <= in_min)
    return out_min;
  else if (val >= in_max)
    return out_max;
  return out_min + ((out_max - out_min) / (in_max - in_min)) * (val - in_min);
}
