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

void arta::argparse::Parser::display_help() {
  std::size_t flag_length = 0;
  for (auto& flag : flags_) {
    std::size_t len = 0;
    for (auto& it : name_map_) {
      if (it.second == flag[0]) {
        len += it.first.length() + 2;
      }
    }
    flag_length = std::max(flag_length, len);
  }
  std::size_t opt_length = 0;
  for (auto& opt : options_) {
    std::size_t len = 0;
    for (auto& it : name_map_) {
      if (it.second == opt[0]) {
        len += it.second.length() + 2;
      }
    }
    opt_length = std::max(opt_length, len);
  }
  printf("FLAGS:\n");
  for (auto& flag : flags_) {
    std::string flag_str = "  ";
    for (auto& it : name_map_) {
      if (it.second == flag[0]) {
        flag_str += it.first + ", ";
      }
    }
    flag_str.pop_back();
    flag_str.pop_back();
    flag_str += std::string(flag_length - flag_str.length() + 2, ' ');
    printf("%s  %s\n", flag_str.c_str(), flag[1].c_str());
  }
  printf("OPTIONS:\n");
  for (auto& opt : options_) {
    std::string opt_str = "  ";
    for (auto& it : name_map_) {
      if (it.second == opt[0]) {
        opt_str += it.first + ", ";
      }
    }
    opt_str.pop_back();
    opt_str.pop_back();
    opt_str += std::string(opt_length - opt_str.length() + 2, ' ');
    printf("%s  %s[%s]\n", opt_str.c_str(), opt[2].c_str(), opt[1].c_str());
  }
  exit(0);
}

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
            args.options[it->second] =
                (i - 1 < sargv.size()) ? sargv[i - 1] : "";
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
