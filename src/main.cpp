#include <iostream>
#include <string>

#include <functional>

#include "async.hpp"
#include "image/image2d.hpp"
#include "image/svg.hpp"
#include "mesh/delaunay.hpp"
#include "mesh/mesh.hpp"
#include "mesh/pslg.hpp"
#include "shape.hpp"
#include "math/matrix.hpp"
#include "math/integrate.hpp"
#include "util/range.hpp"

std::array<double, 2> r() {
  int range = 900;
  int offset = (1000 - range) / 2;
  return {{static_cast<double>((rand() % range) + offset),
           static_cast<double>((rand() % range) + offset)}};
  // return fem::shape::Point((rand() % range) + offset, (rand() % range) +
  // offset);
}

int main(int argc, char* argv[]) {
  // srand(time(NULL));
  //
  // std::array<std::array<double, 4>, 4> mat = {{{{2.04, -1, 0, 0}},
  //                                              {{-1, 2.04, -1, 0}},
  //                                              {{0, -1, 2.04, -1}},
  //                                              {{0, 0, -1, 2.04}}}};
  // std::array<double, 4> vec = {{40.8, 0.8, 0.8, 200.8}};
  // std::array<double, 4> T = fem::math::TDM(mat, vec);
  // for(int i = 0; i < T.size(); ++i){
  //   std::cout << i << ":" << T[i] << "\n";
  // }
  //
  // std::function<double(double)> func = [](double x)->double{return x;};
  // double val = fem::math::integrate(func, 0.0, 1.0);
  // std::cout << val << ":" << func(7.32) << "<<\n";

  // std::vector<std::vector<double>> mat = {{1, }}
  // fem::math::Matrix<double> mat;
  


  fem::image::Svg svg(2000, 2000);
  std::vector<std::array<double, 2>> points;
  svg.Fill("white");
  // for (uint32_t i = 0; i < 1000; ++i){
  //   points.push_back(r());
  // }
  // std::vector<std::array<std::size_t, 3>> tri = fem::mesh::Delaunay(points);
  fem::mesh::Pslg pslg = fem::mesh::LoadPslg("A.pslg");
  pslg.Scale(500);
  pslg.Center(2500, 2500);
  // points = fem::mesh::GenPoints(pslg, 20);
  svg.Pslg(pslg, "#000000", "#000000", "#ff0000", 1);
  pslg.points = fem::mesh::GenPoints(pslg, 20);
  fem::mesh::Mesh mesh = fem::mesh::Delaunay(pslg);
  // fem::mesh::Mesh mesh = fem::mesh::Delaunay(points);
  // fem::mesh::Mesh mesh(pslg.points, fem::mesh::Delaunay(pslg.points));
  // fem::mesh::Mesh mesh(points, fem::mesh::Delaunay(points));
  // fem::mesh::Mesh mesh(points, {});
  // for (auto it : tri){
  //   svg.Triangle(it, points, "black");
  // }
  svg.Mesh(mesh, "#ff0000", "#ff0000", 1, 1);
  svg.WriteSvgWait("test.svg");
  
  // fem::image::Svg svg(1000, 1000);
  // svg.Fill("white");
  // fem::mesh::Pslg pslg = fem::mesh::LoadPslg("A.pslg");
  // pslg.Scale(400);
  // pslg.Center(500, 500);
  // svg.Pslg(pslg, "#000000", "#000000", "#ff0000");
  // svg.WriteSvgWait("test.svg");
  return 0;
}
