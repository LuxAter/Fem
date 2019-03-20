#ifndef FEM_ARGPARSE_HPP_
#define FEM_ARGPARSE_HPP_

#include <map>
#include <string>
#include <vector>

namespace fem {
namespace argparse {
  struct Retval {
    std::map<std::string, bool> flags;
    std::map<std::string, std::string> options;
    std::vector<std::string> args;
    inline double getf(std::string n) { return std::atof(options[n].c_str()); }
    inline int geti(std::string n) { return std::atoi(options[n].c_str()); }
  };

  Retval parse_args(std::string fmt, int argc, char* argv[]);
  void print_help(const char** help, unsigned n, std::string usage = "",
                  std::string cmd = "");
}  // namespace argparse
}  // namespace fem

#endif  // FEM_ARGPARSE_HPP_
