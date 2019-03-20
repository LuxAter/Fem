#include "argparse.hpp"

#include <cmath>
#include <cstdio>
#include <map>
#include <string>
#include <vector>

fem::argparse::Retval fem::argparse::parse_args(std::string fmt, int argc,
                                                char* argv[]) {
  Retval args;
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

void fem::argparse::print_help(const char** help, unsigned n, std::string usage,
                               std::string cmd) {
  if (cmd != "") {
    printf("./%s  ", cmd.c_str());
  }
  if (usage != "") {
    printf("%s", usage.c_str());
  }
  printf("\n\n");
  unsigned long max_width = 0;
  for (unsigned i = 0; i < n; ++i) {
    std::string it(help[i]);
    max_width = std::max(max_width, it.substr(0, it.find(':')).size());
  }
  const char* padding = "                                              ";
  for (unsigned i = 0; i < n; ++i) {
    std::string it(help[i]);
    unsigned pad_length = max_width - it.substr(0, it.find(':')).size();
    std::string raw = it.substr(it.find(':') + 1);
    std::string msg;
    while (raw.rfind(' ', 80 - max_width - 4) != std::string::npos &&
           raw.size() >= (80 - max_width - 4)) {
      if (msg != "") {
        msg += std::string(max_width + 4, ' ') +
               raw.substr(0, raw.rfind(' ', 80 - max_width - 4)) + '\n';
      } else {
        msg += raw.substr(0, raw.rfind(' ', 80 - max_width - 4)) + '\n';
      }
      raw = raw.substr(raw.rfind(' ', 80 - max_width - 4) + 1);
    }
    if (msg != "") {
      msg += std::string(max_width + 4, ' ') + raw + '\n';
    } else {
      msg += raw + '\n';
    }
    printf("  %*.*s%s  %s", pad_length, pad_length, padding,
           it.substr(0, it.find(':')).c_str(), msg.c_str());
  }
}
