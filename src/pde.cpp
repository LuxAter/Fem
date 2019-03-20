#include "pde.hpp"

#include <functional>

#include "basis.hpp"
#include "geometry.hpp"
#include "mesh.hpp"
#include "script.hpp"

std::function<double(double, double)> fem::sys_lhs(const mesh::Mesh& mesh,
                                                   const unsigned& e,
                                                   const unsigned& i,
                                                   const unsigned& j) {
  return [mesh, e, i, j](double x, double y) {
    Pair<double> pt = {x, y};
    return basis::local_basis(mesh, pt, e, i) *
               basis::local_basis_deriv_x(mesh, pt, e, j) +
           basis::local_basis(mesh, pt, e, i) *
               basis::local_basis_deriv_y(mesh, pt, e, j) +
           basis::local_basis_deriv_x(mesh, pt, e, i) *
               basis::local_basis_deriv_x(mesh, pt, e, j) +
           basis::local_basis_deriv_y(mesh, pt, e, i) *
               basis::local_basis_deriv_y(mesh, pt, e, j);
  };
}

std::function<double(double, double)> fem::sys_rhs(const mesh::Mesh& mesh,
                                                   const unsigned& e,
                                                   const unsigned& i,
                                                   const std::string& func) {
  return [mesh, e, i, func](double x, double y) {
    Pair<double> pt = {x, y};
    return basis::local_basis(mesh, pt, e, i) * script::call(x, y, func);
  };
}

double fem::approx(const math::Vector& U, const mesh::Mesh& mesh,
                   const unsigned& e, const Pair<double>& pt) {
  double val = 0.0;
  for (unsigned i = 0; i < U.size(); ++i) {
    val += (U[i] * basis::global_basis(mesh, pt, e, i));
  }
  return val;
}
