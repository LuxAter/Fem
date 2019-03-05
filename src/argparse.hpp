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
  };

  Retval parse_args(std::string fmt, int argc, char* argv[]);
}  // namespace argparse
}  // namespace fem

#endif  // FEM_ARGPARSE_HPP_
