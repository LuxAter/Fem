#ifndef ARTA_LINALG_SOLVER_HPP_
#define ARTA_LINALG_SOLVER_HPP_

#include "matrix.hpp"
#include "vector.hpp"

namespace arta {
namespace linalg {
  bool diag_dominant(const Matrix& A);
  Vector gauss_seidel(const Matrix& A, const Vector& b,
                      const unsigned& n = 100);
  Vector conjugate_gradient(const Matrix& A, const Vector& b,
                            const unsigned& n = 100);
  Vector solve(const Matrix& A, const Vector& b, const unsigned& n = 100);
}  // namespace linalg
}  // namespace arta

#endif  // ARTA_LINALG_SOLVER_HPP_
