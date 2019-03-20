#ifndef FEM_PDE_HPP_
#define FEM_PDE_HPP_

#include <functional>

#include "mesh.hpp"
#include "vector.hpp"

namespace fem {
std::function<double(double, double)> sys_lhs(const mesh::Mesh& mesh,
                                              const unsigned& e,
                                              const unsigned& i,
                                              const unsigned& j);

std::function<double(double, double)> sys_rhs(const mesh::Mesh& mesh,
                                              const unsigned& e,
                                              const unsigned& i,
                                              const std::string& func);
double approx(const math::Vector& U, const mesh::Mesh& mesh, const unsigned& e,
              const Pair<double>& pt);
}  // namespace fem

#endif  // FEM_PDE_HPP_
