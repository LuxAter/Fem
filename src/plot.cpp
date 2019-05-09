#include "plot.hpp"

#include <array>
#include <future>
#include <string>
#include <vector>

#include <cmath>
#include <cstdlib>
#include <cstring>

#include <png.h>

#include "arta.hpp"
#include "linalg.hpp"
#include "logger.hpp"
#include "mesh.hpp"
#include "pde.hpp"
#include "print.hpp"
#include "script.hpp"
#include "timer.hpp"

#include "cmaps.hpp"

void arta::plot_grid(const std::string& dest, const unsigned& w,
                     const unsigned& h,
                     const std::vector<std::vector<double>>& vals,
                     std::string cmap, uint32_t bg, double v_min,
                     double v_max) {
  std::array<unsigned, 256> color_map = _cmaps[cmap];
  uint32_t** buffer = (uint32_t**)malloc(h * sizeof(uint32_t*));
  for (unsigned i = 0; i < h; ++i) {
    buffer[i] = (uint32_t*)malloc(w * sizeof(uint32_t));
  }

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
        buffer[i][j] = bg;
        continue;
      }
      buffer[i][j] = color_map[static_cast<unsigned>(
          clamp(vals[i][j], v_min, v_max, 0.0, 255.0))];
    }
  }

  if (dest.find(".bmp") != std::string::npos) {
    write_bmp(dest, buffer, w, h);
  } else if (dest.find(".png") != std::string::npos) {
    write_png(dest, buffer, w, h);
  }

  for (unsigned i = 0; i < h; ++i) {
    free(buffer[i]);
  }
  free(buffer);
}

// void arta::plot(const std::string& dest, const PDE* pde, const unsigned& w,
//                 const unsigned& h, std::string cmap, uint32_t bg) {
//   if (pde->timer) {
//     time::start();
//   }
//   std::vector<std::vector<double>> vals;
//   double stepx = (pde->mesh.bounds[2] - pde->mesh.bounds[0]) / w;
//   double stepy = (pde->mesh.bounds[3] - pde->mesh.bounds[1]) / h;
//   for (double y = pde->mesh.bounds[1]; y < pde->mesh.bounds[3]; y += stepy) {
//     vals.push_back(std::vector<double>());
//     for (double x = pde->mesh.bounds[0]; x < pde->mesh.bounds[2]; x += stepx)
//     {
//       int t = pde->mesh.locate(x, y);
//       if (t < 0) {
//         vals.back().push_back(std::numeric_limits<double>::quiet_NaN());
//         continue;
//       }
//       vals.back().push_back(pde->approx(x, y, t));
//     }
//   }
//   if (pde->timer) {
//     log::status("Evaluated Approximation: %f", time::stop());
//     time::start();
//   }
//   plot_grid(dest, w, h, vals, cmap, bg);
//   if (pde->timer) {
//     log::status("Plot Approximation: %f", time::stop());
//   }
// }
void arta::plot(const std::string& dest, const linalg::Vector& U,
                const mesh::Mesh* mesh, const unsigned& w, const unsigned& h,
                std::string cmap, uint32_t bg, double v_min, double v_max) {
  std::vector<std::vector<double>> vals;
  double stepx = (mesh->bounds[2] - mesh->bounds[0]) / w;
  double stepy = (mesh->bounds[3] - mesh->bounds[1]) / h;
  for (double y = mesh->bounds[1]; y < mesh->bounds[3]; y += stepy) {
    vals.push_back(std::vector<double>());
    for (double x = mesh->bounds[0]; x < mesh->bounds[2]; x += stepx) {
      int t = mesh->locate(x, y);
      if (t < 0) {
        vals.back().push_back(std::numeric_limits<double>::quiet_NaN());
        continue;
      }
      vals.back().push_back(approx(x, y, t, U, mesh));
    }
  }
  plot_grid(dest, w, h, vals, cmap, bg, v_min, v_max);
}

// void arta::plot_tri(const std::string& dest, const PDE* pde, const unsigned&
// w,
//                     const unsigned& h, std::string cmap, uint32_t bg) {
//   if (pde->timer) {
//     time::start();
//   }
//   std::vector<std::vector<double>> vals;
//   double stepx = (pde->mesh.bounds[2] - pde->mesh.bounds[0]) / w;
//   double stepy = (pde->mesh.bounds[3] - pde->mesh.bounds[1]) / h;
//   for (double y = pde->mesh.bounds[1]; y < pde->mesh.bounds[3]; y += stepy) {
//     vals.push_back(std::vector<double>());
//     for (double x = pde->mesh.bounds[0]; x < pde->mesh.bounds[2]; x += stepx)
//     {
//       int t = pde->mesh.locate(x, y);
//       if (t < 0) {
//         vals.back().push_back(std::numeric_limits<double>::quiet_NaN());
//         continue;
//       }
//       vals.back().push_back(t);
//     }
//   }
//   if (pde->timer) {
//     log::status("Evaluated Tris: %f", time::stop());
//     time::start();
//   }
//   plot_grid(dest, w, h, vals, cmap, bg);
//   if (pde->timer) {
//     log::status("Plot Tris: %f", time::stop());
//   }
// }
void arta::plot_tri(const std::string& dest, const mesh::Mesh* mesh,
                    const unsigned& w, const unsigned& h, std::string cmap,
                    uint32_t bg) {
  std::vector<std::vector<double>> vals;
  double stepx = (mesh->bounds[2] - mesh->bounds[0]) / w;
  double stepy = (mesh->bounds[3] - mesh->bounds[1]) / h;
  for (double y = mesh->bounds[1]; y < mesh->bounds[3]; y += stepy) {
    vals.push_back(std::vector<double>());
    for (double x = mesh->bounds[0]; x < mesh->bounds[2]; x += stepx) {
      int t = mesh->locate(x, y);
      if (t < 0) {
        vals.back().push_back(std::numeric_limits<double>::quiet_NaN());
        continue;
      }
      vals.back().push_back(t);
    }
  }
  plot_grid(dest, w, h, vals, cmap, bg);
}
void arta::plot_func(const std::string& dest, const std::string& func,
                     const mesh::Mesh* mesh, const unsigned& w,
                     const unsigned& h, std::string cmap, uint32_t bg) {
  std::vector<std::vector<double>> vals;
  double stepx = (mesh->bounds[2] - mesh->bounds[0]) / w;
  double stepy = (mesh->bounds[3] - mesh->bounds[1]) / h;
  for (double y = mesh->bounds[1]; y < mesh->bounds[3]; y += stepy) {
    vals.push_back(std::vector<double>());
    for (double x = mesh->bounds[0]; x < mesh->bounds[2]; x += stepx) {
      int t = mesh->locate(x, y);
      if (t < 0) {
        vals.back().push_back(std::numeric_limits<double>::quiet_NaN());
        continue;
      }
      vals.back().push_back(script::call(func, x, y));
    }
  }
  plot_grid(dest, w, h, vals, cmap, bg);
}

void arta::write_bmp(const std::string& file, uint32_t** buffer, unsigned w,
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
      uint8_t r = ((buffer[h - y - 1][x] >> 16) & 0XFF);
      uint8_t g = ((buffer[h - y - 1][x] >> 8) & 0XFF);
      uint8_t b = ((buffer[h - y - 1][x]) & 0XFF);
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

void arta::write_png(const std::string& file, uint32_t** buffer, unsigned w,
                     unsigned h) {
  FILE* out = fopen(file.c_str(), "wb");
  if (out == NULL) {
    return;
  }
  png_structp png =
      png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
  if (!png) {
    fclose(out);
    return;
  }
  png_infop info = png_create_info_struct(png);
  if (!info) {
    fclose(out);
    return;
  }
  if (setjmp(png_jmpbuf(png))) {
    fclose(out);
    return;
  }
  uint8_t** byte_data = (uint8_t**)std::malloc(h * sizeof(uint8_t*));
  for (unsigned i = 0; i < h; ++i) {
    byte_data[i] = (uint8_t*)std::malloc(3 * w * sizeof(uint8_t));
  }
  for (unsigned x = 0; x < w; ++x) {
    for (unsigned y = 0; y < h; ++y) {
      uint8_t r = ((buffer[h - y - 1][x] >> 16) & 0XFF);
      uint8_t g = ((buffer[h - y - 1][x] >> 8) & 0XFF);
      uint8_t b = ((buffer[h - y - 1][x]) & 0XFF);
      unsigned id = 3 * x;
      byte_data[y][id + 0] = r;
      byte_data[y][id + 1] = g;
      byte_data[y][id + 2] = b;
    }
  }
  png_init_io(png, out);
  png_set_IHDR(png, info, w, h, 8, PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE,
               PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);
  png_write_info(png, info);
  png_write_image(png, byte_data);
  png_write_end(png, NULL);
  png_free_data(png, info, PNG_FREE_ALL, -1);
  png_destroy_write_struct(&png, (png_infopp)NULL);
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

static std::vector<std::future<void>> plot_threads_;

void arta::plot_async(const std::string& dest, const linalg::Vector U,
                      const mesh::Mesh* mesh, const unsigned& w,
                      const unsigned& h, std::string cmap, uint32_t bg) {
  if (plot_threads_.size() > ARTA_THREAD_MAX) {
    plot_threads_[0].get();
    plot_threads_.erase(plot_threads_.begin());
  }
  plot_threads_.push_back(std::async(arta::plot, dest, U, mesh, w, h, cmap, bg,
                                     std::numeric_limits<double>::infinity(),
                                     -std::numeric_limits<double>::infinity()));
}
void arta::plot_async(const std::string& dest,
                      const std::vector<linalg::Vector> U,
                      const mesh::Mesh* mesh, const unsigned& w,
                      const unsigned& h, std::string cmap, uint32_t bg) {
  double v_min = std::numeric_limits<double>::infinity();
  double v_max = -std::numeric_limits<double>::infinity();
  for (std::size_t i = 0; i < U.size(); ++i) {
    for (std::size_t j = 0; j < U[i].size(); ++j) {
      v_min = std::min(v_min, U[i][j]);
      v_max = std::max(v_max, U[i][j]);
    }
  }
  for (std::size_t i = 0; i < U.size(); ++i) {
    if (plot_threads_.size() > ARTA_THREAD_MAX) {
      plot_threads_[0].get();
      plot_threads_.erase(plot_threads_.begin());
    }
    plot_threads_.push_back(std::async(
        arta::plot, dest + fmt_val(static_cast<unsigned>(i)) + ".png", U[i],
        mesh, w, h, cmap, bg, v_min, v_max));
  }
}
void arta::plot_join() {
  for (auto& it : plot_threads_) {
    it.get();
  }
  plot_threads_.clear();
}
