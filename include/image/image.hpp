#ifndef FEM_IMAGE_IMAGE_HPP_
#define FEM_IMAGE_IMAGE_HPP_

#include <cstdint>
#include <string>
#include <vector>
#include <future>

namespace fem {
namespace image {
  inline std::string GenFileName(std::string base_name, uint64_t i){
    char buff[255];
    snprintf(buff, 255, "%s/%07lu%s.png", base_name.c_str(), i, base_name.c_str());
    return std::string(buff);
  }
  class Image {
   public:
    Image();
    Image(std::size_t width, std::size_t height);
    Image(const Image& copy);

    static inline uint32_t GetColor(float r, float g, float b) {
      uint32_t rgb = (r > 1 ? static_cast<uint32_t>(r) : static_cast<uint32_t>(r*255));
      rgb = (rgb << 8) + (g > 1 ? static_cast<uint32_t>(g) : static_cast<uint32_t>(g*255));
      rgb = (rgb << 8) + (b > 1 ? static_cast<uint32_t>(b) : static_cast<uint32_t>(b*255));
      return rgb;
    }

    void SetPixel(std::size_t x, std::size_t y, uint32_t rgb);
    void Fill(uint32_t rgb);

    std::future<bool> WritePng(const std::string& file_path);
    bool WritePngWait(const std::string& file_path);

   protected:
    std::size_t width_, height_;
    std::vector<std::vector<uint32_t>> frame_buffer_;

   private:
  };
}  // namespace image
}  // namespace fem

#endif  // FEM_IMAGE_IMAGE_HPP_
