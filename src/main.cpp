#include "fem.hpp"

#include <iostream>

int main(int argc, char* argv[]) {
  auto args = fem::argparse::parse_args(
      "p|plot|[plot.png]: f|func|[func]: o|out|[out.out]:", argc, argv);
  fem::script::open_script("scripts/test.lua");
  std::cout << fem::script::call(-2, 2, 0, args.options["func"]) << std::endl;
  fem::script::close_script();
  return 0;
}
