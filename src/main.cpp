#include <iostream>
#include <string>

#include <chrono>
#include <functional>

#include "data/json.hpp"
#include "image/figure.hpp"
#include "image/figure1d.hpp"
#include "image/svg.hpp"
#include "math/sparse.hpp"
#include "mesh/delaunay.hpp"
#include "mesh/quad_tree.hpp"
#include "util/range.hpp"

using namespace fem::math;
using namespace fem::data;

#define POINTS 10000
#define EDGES 100
#define S 10

double func(double x, double y) {
  // return sqrt(pow(x, 2) + pow(y, 2));
  return pow(x, 3) - 3 * x * pow(y, 2);
  // return sin(0.05*sqrt(pow(x - 500,2)+pow(y - 500,2)));
  // return tan(sqrt(pow(x,2)+pow(y,2)));
}
double rad(double r, double theta) { return r * sin(10 * theta); }

int main(int argc, char* argv[]) {

  // uint64_t size = 1000;
  // srand(time(NULL));
  // std::vector<std::array<double, 2>> points;
  // for (uint64_t n = 0; n < POINTS; ++n) {
  //   points.push_back({{static_cast<double>(rand() % size),
  //                      static_cast<double>(rand() % size)}});
  // }
  // fem::mesh::Mesh dt = fem::del::DelTri(points);
  // return 0;

  // TODO implement boundary conditions
  // uint64_t size = 1000;
  // fem::image::Svg svga(size, size);
  // fem::image::Svg svgb(size, size);
  // std::vector<std::array<double, 2>> points;
  // std::vector<std::array<uint64_t, 2>> edges;
  // // srand(time(NULL));
  // srand(3141);
  // points.push_back({{100, 100}});
  // points.push_back({{900, 100}});
  // points.push_back({{100, 900}});
  // points.push_back({{900, 900}});
  // for (uint64_t n = 0; n < POINTS; ++n) {
  //   points.push_back({{static_cast<double>(rand() % (size - 20)) + 10,
  //                      static_cast<double>(rand() % (size - 20)) + 10}});
  // }
  // edges.push_back({{0, 1}});
  // edges.push_back({{2, 3}});
  // edges.push_back({{0, 3}});
  // // for (uint64_t n = 0; n < EDGES; ++n) {
  // //   edges.push_back({{static_cast<uint64_t>(rand() % POINTS),
  // //                     static_cast<uint64_t>(rand() % POINTS)}});
  // // }
  // fem::mesh::Mesh dt = fem::del::DelTri(points);
  // fem::mesh::Mesh cdt = fem::del::ConDelTri(points, edges);
  // std::cout << "Saving Images\n";
  // svga.Fill("white");
  // svga.Mesh(dt, "black", "black", 4, 0, true);
  // svga.WriteSvg("dt.svg");
  // svgb.Fill("white");
  // svgb.Mesh(cdt, "black", "black", 4, 0, true);
  // for (auto& it : edges) {
  //   svgb.Line(points[it[0]][0], points[it[0]][1], points[it[1]][0],
  //             points[it[1]][1], "blue", 10, 4);
  // }
  // svgb.WriteSvg("cdt.svg");
  // return 0;
}
