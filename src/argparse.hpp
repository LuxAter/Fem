#ifndef ARTA_ARGPARSE_HPP_
#define ARTA_ARGPARSE_HPP_

#include <map>
#include <tuple>
#include <string>
#include <vector>

namespace arta {
namespace argparse {
  struct Arguments {
    std::map<std::string, bool> flags;
    std::map<std::string, std::string> options;
    std::vector<std::string> args;
    inline double getf(const std::string& n) {
      return std::atof(options[n].c_str());
    }
    inline double geti(const std::string& n) {
      return std::atoi(options[n].c_str());
    }
  };
  class Parser {
   public:
    std::vector<std::array<std::string, 2>> flags_;
    std::vector<std::array<std::string, 3>> options_;
    std::map<std::string, std::string> name_map_;

    void add_flag(const std::string& name, std::string help = "");
    void add_flag(const char& short_name, const std::string& long_name,
                  std::string help = "");
    void add_flag(const std::vector<std::string>& names, std::string help = "");
    void add_option(const std::string& name, std::string default_val = "",
                    std::string help = "");
    void add_option(const char& short_name, const std::string& long_name,
                    std::string default_val = "", std::string help = "");
    void add_option(const std::vector<std::string>& names,
                    std::string default_val = "", std::string help = "");

    void display_help();
    Arguments parse_args(int argc, char* argv[]);
  };
}  // namespace argparse
}  // namespace arta

#endif  // ARTA_ARGPARSE_HPP_
