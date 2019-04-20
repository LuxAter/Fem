#ifndef ARTA_PDE_HPP_
#define ARTA_PDE_HPP_

#include <string>

#include "script.hpp"
#include "mesh.hpp"
#include "argparse.hpp"
#include "calc.hpp"

namespace arta {
class PDE {
  public:
  PDE();
  explicit PDE(argparse::Arguments args);
  PDE(std::string script_, std::string mesh = "",
      std::array<double, 2> mesh_const = {{-1, -1}}, bool time = false);
  ~PDE();

  void construct_matrices();

  std::string script_source;
  std::string mesh_source;
  std::string dest_dir;

  std::array<double, 2> mesh_constraints;

  mesh::Mesh mesh;

  bool timer = false;
  private:
  void load_script();
  void load_mesh();
};
}  // namespace arta

#endif  // ARTA_PDE_HPP_
