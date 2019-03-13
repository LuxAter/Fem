#include "fem.hpp"

#include <iostream>

int main(int argc, char* argv[]) {
  /////////////////////////////////////////////////////////////////////////////
  // INITIALIZATION                                                          //
  /////////////////////////////////////////////////////////////////////////////
  auto args = fem::argparse::parse_args(
      "s|script|[scripts/funcs.lua]: m|mesh|[.mesh/trial]: r|[500]:", argc,
      argv);
  fem::script::open_script(args.options["script"]);
  fem::mesh::Mesh mesh(args.options["mesh"]);


  /////////////////////////////////////////////////////////////////////////////
  // PLOTTING                                                                //
  /////////////////////////////////////////////////////////////////////////////
  float res = args.geti("r");
  float step = (mesh.bounds[2] - mesh.bounds[0]) / res;
  std::vector<std::vector<double>> vals;
  for (double y = mesh.bounds[1]; y <= mesh.bounds[3]; y += step) {
    vals.push_back(std::vector<double>());
    for (double x = mesh.bounds[0]; x <= mesh.bounds[2]; x += step) {
      int t = mesh.locate({x, y});
      if (t < 0) {
        vals.back().push_back(std::numeric_limits<double>::quiet_NaN());
        continue;
      }
      vals.back().push_back(t);
    }
  }
  fem::plot::imsave("test.bmp", vals, "parula_r");

  /////////////////////////////////////////////////////////////////////////////
  // TERMINATION                                                             //
  /////////////////////////////////////////////////////////////////////////////
  fem::script::close_script();
  return 0;
}
