#ifndef FEM_LINALG_HPP_
#define FEM_LINALG_HPP_

#include "matrix.hpp"
#include "vector.hpp"

namespace fem {
namespace math {
  bool diag_dominant(const Matrix& A);
  Vector gauss_seidel(const Matrix& A, const Vector& b,
                      const unsigned& n = 100);
  Vector conjugate_gradient(const Matrix& A, const Vector& b,
                            const unsigned& n = 100);
  Vector solve(const Matrix& A, const Vector& b, const unsigned& n = 100);
}  // namespace math
}  // namespace fem

#endif  // FEM_LINALG_HPP_
