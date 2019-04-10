#ifndef ARTA_PDE_HPP_
#define ARTA_PDE_HPP_

#include <string>

#include "script.hpp"

namespace arta {
struct PDE {
  PDE();
  PDE(std::string script_, std::string mesh="");
  ~PDE();

  std::string script_source;
  std::string mesh_source;
};
}  // namespace arta

#endif  // ARTA_PDE_HPP_
