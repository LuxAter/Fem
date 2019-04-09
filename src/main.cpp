#include "arta.hpp"

#include <iostream>

int main(int argc, char *argv[])
{
  arta::log::version("ARTA V.%d.%d", ARTA_VERSION_MAJOR, ARTA_VERSION_MINOR);
  arta::argparse::Parser parser;
  parser.add_flag('v', "verbose", "Enables verbose output");
  parser.add_flag('t', "time", "Enables timing of execution");
  auto args = parser.parse_args(argc, argv);
  for(auto& flg : args.flags){
    std::cout << flg.first << ":" << flg.second << "\n";
  }
  for(auto& flg : args.options){
    std::cout << flg.first << ":" << flg.second << "\n";
  }
  arta::script::load_script("../resources/test.lua");
  std::cout << arta::script::has("soln") << "\n";
  arta::script::close_script();
  return 0;
}
