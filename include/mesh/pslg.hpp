#ifndef FEM_MESH_PSLG_HPP_
#define FEM_MESH_PSLG_HPP_

#include <vector>

#include "../util/util.hpp"

namespace fem {
  struct PSLG{
    std::vector<Pt> points;
    std::vector<Edge> edges;
    std::vector<Pt> holes;
  };

  void SaveToFile(const std::string& file_name, const PSLG& pslg);
  PSLG LoadPslgFromFile(const std::string& file_name);
} // namespace fem

#endif  // FEM_MESH_PSLG_HPP_
