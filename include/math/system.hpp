#ifndef FEM_MATH_SYSTEM_HPP_
#define FEM_MATH_SYSTEM_HPP_

#include "sparse.hpp"

namespace fem {
namespace math {
  Vector<double> Jacobi(const Matrix<double>& A, const Vector<double>& b,
                        uint32_t N = 100);
  Vector<double> GaussSeidel(const Matrix<double>& A, const Vector<double>& b,
                             uint32_t N = 100);
  Vector<double> ConjugateGradient(const Matrix<double>& A,
                                   const Vector<double>& b, uint32_t N = 100);
  Vector<double> ForwardSub(const Matrix<double>& A, const Vector<double>& b);
  Vector<double> BackwardSub(const Matrix<double>& A, const Vector<double>& b);

  Vector<double> CholeskyMethod(const Matrix<double>& A,
                                const Vector<double>& b);
}  // namespace math
}  // namespace fem

#endif  // FEM_MATH_SYSTEM_HPP_
