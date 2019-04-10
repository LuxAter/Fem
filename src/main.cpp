#include "arta.hpp"

#include <iostream>

int main(int argc, char* argv[]) {
  arta::argparse::Parser parser;
  parser.add_flag('v', "verbose", "Enables verbose output");
  parser.add_flag('t', "time", "Enables timing of execution");
  parser.add_option('r', "res", "100", "Resolution of output images");
  parser.add_option('s', "script", "", "Script file to load");
  parser.add_option('m', "mesh", "", "Mesh file to load");
  auto args = parser.parse_args(argc, argv);
  if (!args.flags["verbose"]) {
    arta::log::Console()->set_activation(arta::log::FATAL | arta::log::ERROR |
                                         arta::log::WARNING);
  }
  arta::log::version("ARTA V.%d.%d", ARTA_VERSION_MAJOR, ARTA_VERSION_MINOR);
  arta::PDE pde(args.options["script"], args.options["mesh"]);
  return 0;
}
