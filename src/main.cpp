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

#define C 5000

int main(int argc, char* argv[]) {
  fem::math::Sparse a(10), b(10);
  a(5, 5) = 55;
  a(2, 8) = 28;
  b(9, 1) = 91;
  b(0, 4) = 4;
  std::cout << a;
  std::cout << a.size() << ';' << a.count() << ';' << a.count2() << '\n';
  fem::math::Sparse c = a - a;
  std::cout << c;
  std::cout << c.size() << ';' << c.count() << ';' << c.count2() << '\n';

  // uint64_t size = 5000;
  // fem::image::Svg svg(size, size);
  // fem::image::Figure tim(size, size);
  // std::vector<std::array<double, 2>> points;
  // std::array<double, C> times;
  // srand(time(NULL));
  // for (uint64_t npoints = 3; npoints < C + 3; npoints += 1) {
  //   points.clear();
  //   for (uint64_t n = 0; n < npoints; ++n) {
  //     points.push_back({{static_cast<double>(rand() % (size - 20)) + 10,
  //                        static_cast<double>(rand() % (size - 20)) + 10}});
  //   }
  //   auto begin = std::chrono::high_resolution_clock::now();
  //   fem::mesh::Mesh delaunay = fem::del::DelTri(points);
  //   auto end = std::chrono::high_resolution_clock::now();
  //   double dt =
  //   std::chrono::duration_cast<std::chrono::nanoseconds>(end-begin).count()
  //   * 1.0e-9; std::cout << npoints << "->" << dt << '\n'; times[npoints - 3]
  //   = dt;
  // }
  // tim.Line(fem::util::StepRangeArray<double, C>(3, C + 3), times, "", "", "",
  // 5); tim.SavePgf("time.tikz"); tim.SaveSvg("time.svg"); return 0;
}
