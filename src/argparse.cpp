#include "argparse.hpp"

#include <map>
#include <string>
#include <tuple>
#include <vector>

#include "logger.hpp"

void arta::argparse::Parser::add_flag(const std::string& name,
                                      std::string help) {
  flags_.push_back({{name, help}});
  name_map_[name] = name;
}
void arta::argparse::Parser::add_flag(const char& short_name,
                                      const std::string& long_name,
                                      std::string help) {
  flags_.push_back({{long_name, help}});
  name_map_[long_name] = long_name;
  name_map_[std::string(1, short_name)] = long_name;
}
void arta::argparse::Parser::add_flag(const std::vector<std::string>& names,
                                      std::string help) {
  std::string longest;
  for (auto name : names) {
    if (name.length() > longest.length()) {
      longest = name;
    }
  }
  for (auto name : names) {
    name_map_[name] = longest;
  }
  flags_.push_back({{longest, help}});
}
void arta::argparse::Parser::add_option(const std::string& name,
                                        std::string default_val,
                                        std::string help) {
  options_.push_back({{name, default_val, help}});
  name_map_[name] = name;
}
void arta::argparse::Parser::add_option(const char& short_name,
                                        const std::string& long_name,
                                        std::string default_val,
                                        std::string help) {
  options_.push_back({{long_name, default_val, help}});
  name_map_[long_name] = long_name;
  name_map_[std::string(1, short_name)] = long_name;
}
void arta::argparse::Parser::add_option(const std::vector<std::string>& names,
                                        std::string default_val,
                                        std::string help) {
  std::string longest;
  for (auto name : names) {
    if (name.length() > longest.length()) {
      longest = name;
    }
  }
  for (auto name : names) {
    name_map_[name] = longest;
  }
  options_.push_back({{longest, default_val, help}});
}

void arta::argparse::Parser::display_help() { exit(0); }

arta::argparse::Arguments arta::argparse::Parser::parse_args(int argc,
                                                             char* argv[]) {
  Arguments args;
  for (auto& flg : flags_) {
    args.flags[flg[0]] = false;
  }
  for (auto& opt : options_) {
    args.options[opt[0]] = opt[1];
  }
  std::vector<std::string> sargv;
  for (int i = 1; i < argc; ++i) {
    sargv.push_back(std::string(argv[i]));
  }
  for (int i = 1; i < argc; ++i) {
    std::string arg = sargv[i - 1];
    while (arg.front() == '-') {
      arg = arg.substr(1);
    }
    bool has_var = arg.find('=') != std::string::npos;
    std::string val = arg.substr(arg.find('=') + 1);
    arg = arg.substr(0, arg.find('='));
    if (arg == "help" || arg == "h" || arg == "?") {
      display_help();
    }
    typename std::map<std::string, std::string>::iterator it =
        name_map_.find(arg);
    if (it != name_map_.end()) {
      for (auto& flag : flags_) {
        if (flag[0] == it->second) {
          args.flags[it->second] = true;
          break;
        }
      }
      for (auto& opt : options_) {
        if (opt[0] == it->second) {
          if (has_var) {
            args.options[it->second] = val;
          } else {
            i++;
            args.options[it->second] = (i < sargv.size()) ? sargv[i - 1] : "";
          }
          break;
        }
      }
    } else {
      it = name_map_.find(arg.substr(0, 1));
      if (it != name_map_.end()) {
        for (auto& flag : flags_) {
          if (flag[0] == it->second) {
            args.flags[it->second] = true;
            break;
          }
        }
        sargv[i - 1] = sargv[i - 1].substr(2);
        i--;
      } else {
        args.args.push_back(arg);
      }
    }
  }
  return args;
}
