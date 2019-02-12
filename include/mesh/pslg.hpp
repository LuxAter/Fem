#ifndef FEM_MESH_PSLG_HPP_
#define FEM_MESH_PSLG_HPP_

#include <vector>

#include "../util/util.hpp"

namespace fem {
  struct PSLG{
    std::vector<Pt> points;
    std::vector<Edge> edges;
    std::vector<Pt> holes;
    double _dmax, _x_min, _y_min, _x_max, _y_max;
  };

  PSLG Normalize(const PSLG& pslg, double min_v = 0.0, double max_v = 1.0);
  PSLG UnNormalize(const PSLG& pslg, double min_v = 0.0, double max_v = 1.0);

  void SaveToFile(const std::string& file_name, const PSLG& pslg);
  PSLG LoadPslgFromFile(const std::string& file_name);
} // namespace fem

#endif  // FEM_MESH_PSLG_HPP_
