#ifndef ARTA_PDE_HPP_
#define ARTA_PDE_HPP_

#include <string>

#include "argparse.hpp"
#include "calc.hpp"
#include "linalg.hpp"
#include "mesh.hpp"
#include "script.hpp"

#include <functional>

namespace arta {
class PDE {
 public:
  PDE();
  explicit PDE(argparse::Arguments args);
  PDE(std::string script_, std::string mesh = "",
      std::array<double, 2> mesh_const = {{-1, -1}}, bool time = false);
  ~PDE();

  void construct_matrices();
  void construct_forcing(const double& t);

  linalg::Vector solve_time_indep();

  double approx(const double& x, const double& y, const unsigned& e) const;

  std::function<double(double, double)> G(const unsigned& e, const unsigned& i,
                                          const unsigned& j);
  std::function<double(double, double)> A(const unsigned& e, const unsigned& i,
                                          const unsigned& j);
  std::function<double(double, double)> B(const unsigned& e, const unsigned& i,
                                          const unsigned& j);
  std::function<double(double, double)> C(const unsigned& e, const unsigned& i,
                                          const unsigned& j);
  std::function<double(double, double)> F(const unsigned& e, const unsigned& i,
                                          const double& t);

  std::string script_source;
  std::string mesh_source;
  std::string dest_dir;

  std::array<double, 2> mesh_constraints;

  double time_;

  linalg::Matrix G_, M_;
  linalg::Vector F_, U_;

  mesh::Mesh mesh;

  bool timer = false;

 private:
  void load_script();
  void load_mesh();
};
}  // namespace arta

#endif  // ARTA_PDE_HPP_
