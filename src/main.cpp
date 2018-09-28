#include <string>
#include <iostream>

#include "image/image2d.hpp"
#include "mesh/delaunay.hpp"
#include "shape.hpp"
#include "async.hpp"

std::array<double, 2> r(){
  int range = 200;
  int offset = (1000 - range) / 2;
  return {{static_cast<double>((rand() % range) + offset), static_cast<double>((rand() % range) + offset)}};
  // return fem::shape::Point((rand() % range) + offset, (rand() % range) + offset);
}

int main(int argc, char *argv[])
{
  srand(time(NULL));
  fem::image::Image2D img(1000, 1000);
  std::vector<std::array<double, 2>> points;
  for (uint32_t i = 0; i < 10; ++i){
    points.push_back(r());
    img.Point(points.back());
  }
  std::vector<std::array<std::size_t, 3>> tri = fem::mesh::Delaunay(points);
  for (auto it : tri){
    img.Triangle(it, points, 0x00ffff);
  }
  img.WritePng("test.png");
  return 0;
}
