#include <iostream>
#include <string>

#include <functional>
#include <chrono>

#include "image/figure.hpp"
#include "math/matrix.hpp"
#include "util/range.hpp"
#include "mesh/delaunay.hpp"

using namespace fem::math;

int main(int argc, char* argv[]) {
  fem::image::Svg svg(500, 500);
  std::vector<std::array<double, 2>> points;
  srand(time(NULL));
  for(uint64_t n = 0; n < 10; ++n){
    points.push_back({{static_cast<double>(rand() % 500), static_cast<double>(rand() % 500)}});
  }
  fem::mesh::Mesh delaunay = fem::del::DelTri(points);
  svg.Rectangle(0,0,500,500, "white", "white");
  svg.Mesh(delaunay);
  svg.WriteSvg("testing.svg");
  return 0;
}
