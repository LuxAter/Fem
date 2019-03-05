#include "argparse.hpp"

#include <map>
#include <string>
#include <vector>

fem::argparse::Retval fem::argparse::parse_args(std::string fmt, int argc,
                                                char* argv[]) {
  Retval args;
  // h|?|help f|func:
  std::map<std::string, std::string> flag_table;
  std::map<std::string, std::string> option_table;
  std::size_t a = 0, b = 0;
  while (a < fmt.size() && b < fmt.size()) {
    a = b + 1;
    if (a == 1) a = 0;
    b = fmt.find(' ', b + 1);
    if (b == std::string::npos) {
      b = fmt.size();
    }
    std::string argument = fmt.substr(a, b - a);
    std::vector<std::string> names;
    bool opt = false;
    if (argument.back() == ':') {
      opt = true;
      argument.pop_back();
    }
    std::size_t c = 0, d = 0;
    std::string default_val = "";
    while (c < argument.size() && d < argument.size()) {
      c = d + 1;
      if (c == 1) c = 0;
      d = argument.find('|', c);
      if (d == std::string::npos) {
        d = argument.size();
      }
      if (argument.substr(c, d - c).front() == '[' &&
          argument.substr(c, d - c).back() == ']') {
        default_val = argument.substr(c + 1, d - c - 2);
      } else {
        names.push_back(argument.substr(c, d - c));
      }
    }
    std::string name = "";
    for (auto& it : names) {
      if (it.size() > name.size()) {
        name = it;
      }
    }
    if (opt) {
      for (auto& it : names) {
        option_table[it] = name;
      }
      args.options[name] = default_val;
    } else {
      for (auto& it : names) {
        flag_table[it] = name;
      }
      args.flags[name] = false;
    }
  }

  for (int i = 1; i < argc; ++i) {
    std::string arg(argv[i]);
    while (arg.front() == '-') {
      arg = arg.substr(1);
    }
    typename std::map<std::string, std::string>::iterator it =
        flag_table.find(arg);
    if (it != flag_table.end()) {
      args.flags[it->second] = true;
    } else {
      it = option_table.find(arg);
      if (it != option_table.end()) {
        args.options[it->second] = argv[i + 1];
        i++;
      } else {
        args.args.push_back(argv[i]);
      }
    }
  }
  return args;
}
