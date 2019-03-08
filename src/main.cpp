#include "fem.hpp"

#include <iostream>

int main(int argc, char* argv[]) {
  auto args = fem::argparse::parse_args(
      "s|script|[scripts/funcs.lua]: m|mesh|[.mesh/trial]:",
      argc, argv);
  fem::script::open_script(args.options["script"]);
  fem::mesh::Mesh mesh(args.options["mesh"]);

  fem::script::close_script();
  return 0;
}
