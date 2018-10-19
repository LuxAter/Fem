#ifndef FEM_MESH_PSLG_HPP_
#define FEM_MESH_PSLG_HPP_

#include <array>
#include <string>
#include <vector>

namespace fem {
namespace mesh {
  class Pslg {
   public:
    void Center();
    void Center(double x, double y);
    void Scale();
    void Scale(double alpha);

    bool Contains(std::array<double, 2> pt);
    bool Contains(std::array<std::size_t, 2> eg);

    std::vector<std::array<double, 2>> points;
    std::vector<std::array<double, 2>> holes;
    std::vector<std::array<std::size_t, 2>> edges;
  };

  std::vector<std::array<double, 2>> GenPoints(const Pslg& pslg,
                                               std::size_t scale = 10);

  Pslg LoadPslg(const std::string& file);
  void SavePslg(const std::string& file, Pslg pslg);
}  // namespace mesh
}  // namespace fem

#endif  // FEM_MESH_PSLG_HPP_
