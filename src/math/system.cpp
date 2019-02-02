#include "math/system.hpp"

#include <array>

#include "math/sparse.hpp"

fem::math::Vector<double> fem::math::Jacobi(const Matrix<double>& A,
                                            const Vector<double>& b,
                                            uint32_t N) {
  Vector<double> x(b.size());
  uint32_t n = b.size();
  for (uint32_t k = 0; k < N; ++k) {
    Vector<double> x_bar(n);
    for (uint32_t i = 0; i < n; ++i) {
      for (uint32_t j = 0; j < n; ++j) {
        if (j == i) continue;
        x_bar[i] += A(i, j) * x[j];
      }
      x_bar[i] = (b[i] - x_bar[i]) / A(i, i);
    }
    x = x_bar;
    if (Norm(A * x - b) <= 1e-10) {
      break;
    }
  }
  return x;
}

fem::math::Vector<double> fem::math::GaussSeidel(const Matrix<double>& A,
                                                 const Vector<double>& b,
                                                 uint32_t N) {
  Vector<double> x(b.size());
  uint32_t n = b.size();
  for (uint32_t k = 0; k < N; ++k) {
    for (uint32_t i = 0; i < n; ++i) {
      double sigma = 0;
      for (uint32_t j = 0; j < n; ++j) {
        if (j == i) continue;
        sigma = sigma + A(i, j) * x[j];
      }
      x[i] = (b[i] - sigma) / A(i, i);
    }
    if (Norm(A * x - b) <= 1e-10) {
      break;
    }
  }
  return x;
}

fem::math::Vector<double> fem::math::ConjugateGradient(const Matrix<double>& A,
                                                       const Vector<double>& b,
                                                       uint32_t N) {
  Vector<double> x(b.size());
  Vector<double> r = b - A * x;
  double rho_ = 0.0;
  for (uint32_t i = 0; i < N; ++i) {
    double rho = Dot(r, r);
    double beta;
    Vector<double> p(r.size());
    if (i == 0) {
      p = r;
    } else {
      beta = rho / rho_;
      p = r + p * beta;
    }
    Vector<double> q = A * p;
    double alpha = rho / Dot(p, q);
    x = x + p * alpha;
    r = r - q * alpha;
    if (Norm(r) <= 1e-10) {
      break;
    }
    rho_ = rho;
  }
  return x;
}

fem::math::Vector<double> fem::math::ForwardSub(const Matrix<double>& A,
                                                const Vector<double>& b) {
  Vector<double> x(b.size());
  for (uint64_t m = 0; m < x.size(); ++m) {
    double sum = 0.0;
    for (uint64_t i = 0; i < m; ++i) {
      sum += A.get(m, i) * x[i];
    }
    x[m] = (b[m] - sum) / A.get(m, m);
  }
  return x;
}

fem::math::Vector<double> fem::math::BackwardSub(const Matrix<double>& A,
                                                 const Vector<double>& b) {
  Vector<double> x(b.size());
  for (int64_t i = x.size() - 1; i >= 0; --i) {
    double sum = 0.0;
    for (uint64_t j = i + 1; j < x.size(); ++j) {
      sum += A.get(i, j) * x[j];
    }
    x[i] = (b[i] - sum) / A.get(i, i);
  }
  return x;
}

fem::math::Vector<double> fem::math::CholeskyMethod(const Matrix<double>& A,
                                                    const Vector<double>& b) {
  Matrix<double> L = Cholesky(A);
  Vector<double> y = ForwardSub(L, b);
  Vector<double> x = BackwardSub(Transpose(L), y);
  return x;
}
