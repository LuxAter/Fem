#include "arta.hpp"

#include <iostream>

double test(double x, double y) { return y * x * x + x * y * y; }

int main(int argc, char* argv[]) {
  arta::argparse::Parser parser;
  parser.add_flag('v', "verbose", "Enables verbose output");
  parser.add_flag('t', "time", "Enables timing of execution");
  parser.add_option('r', "res", "500", "Resolution of output images");
  parser.add_option('s', "script", "", "Script file to load");
  parser.add_option('m', "mesh", "", "Mesh file to load");
  parser.add_option('a', "mesh-area", "-1", "Mesh maximum triangle area");
  parser.add_option('q', "mesh-angle", "-1", "Mesh minimum triangle angle");
  parser.add_option('c', "cmap", "parula", "Plot color map basis");
  auto args = parser.parse_args(argc, argv);
  std::vector<double> times;
  if (!args.flags["verbose"]) {
    arta::log::Console()->set_activation(arta::log::FATAL | arta::log::ERROR |
                                         arta::log::WARNING);
  }
  arta::log::Default()->format_all("[__TYPE__](__FUNC__:__LINE__) __BODY__");
  arta::log::Console()->set_color(true);
  arta::log::version("ARTA V.%d.%d", ARTA_VERSION_MAJOR, ARTA_VERSION_MINOR);
  arta::PDE pde(args);
  pde.solve_time_indep();
  arta::plot(pde.dest_dir + "approx.png", &pde, args.geti("res"),
             args.geti("res"), args.options["cmap"]);
  arta::plot_tri(pde.dest_dir + "tri.png", &pde, args.geti("res"),
                 args.geti("res"), args.options["cmap"]);
  return 0;
}
