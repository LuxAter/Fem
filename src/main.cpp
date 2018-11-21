#include <iostream>
#include <string>

#include <chrono>
#include <functional>

#include "data/json.hpp"
#include "image/figure.hpp"
#include "math/matrix.hpp"
#include "math/sparse.hpp"
#include "mesh/delaunay.hpp"
#include "util/range.hpp"

using namespace fem::math;
using namespace fem::data;

#define POINTS 10
#define EDGES 100
#define S 10

int main(int argc, char* argv[]) {
  uint64_t size = 1000;
  fem::image::Svg svga(size, size);
  fem::image::Svg svgb(size, size);
  std::vector<std::array<double, 2>> points;
  std::vector<std::array<uint64_t, 2>> edges;
  // srand(time(NULL));
  srand(3141);
  points.push_back({{100, 100}});
  points.push_back({{900, 100}});
  points.push_back({{100, 900}});
  points.push_back({{900, 900}});
  for (uint64_t n = 0; n < POINTS; ++n) {
    points.push_back({{static_cast<double>(rand() % (size - 20)) + 10,
                       static_cast<double>(rand() % (size - 20)) + 10}});
  }
  edges.push_back({{0, 1}});
  edges.push_back({{2, 3}});
  edges.push_back({{0, 3}});
  // for (uint64_t n = 0; n < EDGES; ++n) {
  //   edges.push_back({{static_cast<uint64_t>(rand() % POINTS),
  //                     static_cast<uint64_t>(rand() % POINTS)}});
  // }
  fem::mesh::Mesh dt = fem::del::DelTri(points);
  fem::mesh::Mesh cdt = fem::del::ConDelTri(points, edges);
  std::cout << "Saving Images\n";
  svga.Fill("white");
  svga.Mesh(dt, "black", "black", 4, 0, true);
  svga.WriteSvg("dt.svg");
  svgb.Fill("white");
  svgb.Mesh(cdt, "black", "black", 4, 0, true);
  for (auto& it : edges) {
    svgb.Line(points[it[0]][0], points[it[0]][1], points[it[1]][0],
              points[it[1]][1], "blue", 10, 4);
  }
  svgb.WriteSvg("cdt.svg");
  return 0;
}
