#include "arta.hpp"

#include <iostream>

int main(int argc, char* argv[]) {
  arta::argparse::Parser parser;
  parser.add_flag('v', "verbose", "Enables verbose output");
  parser.add_flag('t', "time", "Enables timing of execution");
  parser.add_option('r', "res", "100", "Resolution of output images");
  parser.add_option('s', "script", "", "Script file to load");
  parser.add_option('m', "mesh", "", "Mesh file to load");
  parser.add_option('a', "mesh-area", "-1", "Mesh maximum triangle area");
  parser.add_option('q', "mesh-angle", "-1", "Mesh minimum triangle angle");
  auto args = parser.parse_args(argc, argv);
  std::vector<double> times;
  if (!args.flags["verbose"]) {
    arta::log::Console()->set_activation(arta::log::FATAL | arta::log::ERROR |
                                         arta::log::WARNING);
  }
  arta::log::version("ARTA V.%d.%d", ARTA_VERSION_MAJOR, ARTA_VERSION_MINOR);
  arta::PDE pde(args);
  pde.solve_time_indep();
  arta::plot("test.bmp", &pde, 500, 500, "parula");
  return 0;
}
