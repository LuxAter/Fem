#include "fem.hpp"

#include <iostream>

int main(int argc, char* argv[]) {
  auto args = fem::argparse::parse_args(
      "p|plot|[plot.png]: f|func|[func]: o|out|[out.out]: r|res|[100]: "
      "cmap|[parula]: b|[-1.0]: e|[1.0]:",
      argc, argv);
  fem::script::open_script("scripts/test.lua");
  std::vector<std::vector<double>> data;
  double res = (args.getf("e") - args.getf("b")) / args.geti("res");
  for (double y = args.getf("b"); y <= args.getf("e"); y += res) {
    data.push_back(std::vector<double>());
    for (double x = args.getf("b"); x <= args.getf("e"); x += res) {
      data.back().push_back(fem::script::call(x, y, 0.0, args.options["func"]));
    }
  }
  fem::plot::imsave("test.bmp", data, args.options["cmap"]);
  fem::script::close_script();
  return 0;
}
