#ifndef FEM_IMAGE_PNG_HPP_
#define FEM_IMAGE_PNG_HPP_

#include <string>

#include "../mesh/pslg.hpp"

namespace fem {
  namespace image {
    bool WritePng(const std::string& file_name, const PSLG& pslg);
  } // namespace image
} // namespace fem

#endif  // FEM_IMAGE_PNG_HPP_
