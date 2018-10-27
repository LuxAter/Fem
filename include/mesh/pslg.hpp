#ifndef FEM_MESH_PSLG_HPP_
#define FEM_MESH_PSLG_HPP_

#include <array>
#include <string>
#include <vector>

namespace fem {
namespace mesh {
  class Pslg {
   public:
    std::array<double, 4> Bounds() const;

    void Scale();

    std::vector<std::array<double, 2>> points;
    std::vector<std::array<double, 2>> holes;
    std::vector<std::array<std::size_t, 2>> edges;
  };

  Pslg operator*(const Pslg& lhs, const double& rhs);
  Pslg operator+(const Pslg& lhs, const double& rhs);

  Pslg Random(const std::size_t& points, const std::size_t& edges);

  Pslg LoadPslg(const std::string& file);
  Pslg LoadSvgToPslg(const std::string& file);
  void SavePslg(const std::string& file, Pslg pslg);
}  // namespace mesh
}  // namespace fem

#endif  // FEM_MESH_PSLG_HPP_
