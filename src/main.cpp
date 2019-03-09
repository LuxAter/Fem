#include "fem.hpp"

#include <iostream>

int main(int argc, char* argv[]) {
  auto args = fem::argparse::parse_args(
      "s|script|[scripts/funcs.lua]: m|mesh|[.mesh/trial]: r|[500]:",
      argc, argv);
  fem::script::open_script(args.options["script"]);
  fem::mesh::Mesh mesh(args.options["mesh"]);

  float res = args.geti("r");
  float step = 2.0 / res;
  std::vector<std::vector<double>> vals;
  for(double y = -1.0; y < 1.0; y += step){
    vals.push_back(std::vector<double>());
    for(double x = -1.0; x < 1.0; x += step){
      int t = mesh.locate({x, y});
      double val = fem::basis::global_basis(mesh, {x, y}, t, 5);
      vals.back().push_back(val);
    }
  }
  fem::plot::imsave("test.bmp", vals, "parula");

  fem::script::close_script();
  return 0;
}
