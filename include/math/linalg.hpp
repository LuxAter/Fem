#ifndef FEM_MATH_LINALG_HPP_
#define FEM_MATH_LINALG_HPP_

#include "matrix.hpp"
#include "vector.hpp"

namespace fem {
Vec ConjugateGradiant(const Mat& A, const Vec& b, unsigned n = 100);
Vec LinAlgSolve(const Mat& A, const Vec& b);
}  // namespace fem

#endif  // FEM_MATH_LINALG_HPP_
