#ifndef FEM_MESH_PSLG_HPP_
#define FEM_MESH_PSLG_HPP_

#include <vector>

#include "../util/util.hpp"

namespace fem {
  struct PSLG{
    std::vector<Pt> points_;
    std::vector<Edge> edges_;
    std::vector<Pt> holes_;
  };

  void SaveToFile(const std::string_view& file_name, const PSLG& pslg);
  PSLG LoadFromFile(const std::string_view& file_name);
} // namespace fem

#endif  // FEM_MESH_PSLG_HPP_
