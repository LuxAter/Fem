#include <iostream>
#include <string>

#include <chrono>
#include <functional>

#include "image/figure.hpp"
#include "math/matrix.hpp"
#include "mesh/delaunay.hpp"
#include "util/range.hpp"
#include "data/json.hpp"

using namespace fem::math;
using namespace fem::data;

int main(int argc, char* argv[]) {
  JsonNode json(JsonNode::OBJECT);
  json.insert("hello", "arden");
  json.insert("value", {{3.1415, 1998, "testing"}});
  std::cout << json << '\n';
  json["value"].append("Hello World");
  std::cout << json["value"] << "<<\n";
  json.insert("Arden", JsonNode::OBJECT);
  json["Arden"].insert("age", 20);
  json["Arden"].insert("height", 6.2);
  std::cout << json << '\n';
  // uint64_t size = 1000;
  // fem::image::Svg svg(size, size);
  // std::vector<std::array<double, 2>> points;
  // srand(time(NULL));
  // std::cout << "Generating Points\n";
  // for (uint64_t n = 0; n < 10000; ++n) {
  //   points.push_back({{static_cast<double>(rand() % (size - 20)) + 10,
  //                      static_cast<double>(rand() % (size - 20)) + 10}});
  // }
  // std::cout << "Generating Mesh\n";
  // fem::mesh::Mesh delaunay = fem::del::DelTri(points);
  // std::cout << "Writing SVG\n";
  // svg.Fill("white");
  // svg.Mesh(delaunay);
  // svg.WriteSvg("testing.svg");
  return 0;
}
