#include "mesh/delaunay.hpp"

#include <cmath>

#include "mesh/mesh.hpp"
#include "mesh/pslg.hpp"
#include "mesh/shape.hpp"

void fem::mesh::InsertVertex(fem::mesh::Mesh& mesh,
                             const fem::mesh::Vertex& vert) {}

void fem::mesh::RestoreDelaunay(fem::mesh::Mesh& mesh) {}

std::vector<std::array<double, 2>> fem::mesh::SortPoints(const fem::mesh::Pslg& pslg){
  std::vector<std::vector<std::size_t>> point_bins(
      static_cast<std::size_t>(
          std::sqrt(static_cast<double>(pslg.points.size()))));
  std::size_t n = pslg.points.size();
  std::array<double, 4> bounds = pslg.Bounds();
  for (std::size_t k = 0; k < n; ++k) {
    std::size_t i = static_cast<std::size_t>(0.99 * n * pslg.points[k][1] / bounds[3]),
                j = static_cast<std::size_t>(0.99 * n * pslg.points[k][0] / bounds[2]);
    std::size_t b = (i % 2 == 0) ? i * n + j + 1 : (i + 1) * n - j;
    point_bins[b].push_back(k);
  }
  std::vector<std::array<double, 2>> points;
  for(auto& bin : point_bins){
    for(auto& point : bin){
      points.push_back(pslg.points[point]);
    }
  }
  return points;
}

fem::mesh::Mesh fem::mesh::Delaunay(const fem::mesh::Pslg& pslg) {
  Mesh mesh;
  std::vector<std::array<double, 2>> points = SortPoints(pslg);
  std::vector<std::array<std::size_t, 3>> triangle_vertex, triangle_adjacent;
  return mesh;
}
