#include "math/linalg.hpp"

#include "math/matrix.hpp"
#include "math/vector.hpp"

fem::Vec fem::ConjugateGradiant(const Mat& A, const Vec& b, unsigned n){
  Vec x(b.size());
  Vec r = b - A * x;
  double rho_ = 0.0;
  for (unsigned i = 0; i < n; ++i){
    double rho = Dot(r, r);
    Vec p(r.size());
    if(i == 0){
      p = r;
    }else {
      double beta;
      beta = rho / rho_;
      p = r + p * beta;
    }
    Vec q = A * p;
    double alpha = rho / Dot(p,q);
    x += p * alpha;
    r -= q * alpha;
    if(Norm(r) <= 1e-10){
      break;
    }
    rho_ = rho;
  }
  return x;
}

fem::Vec fem::LinAlgSolve(const Mat& A, const Vec& b){
  return ConjugateGradiant(A, b);
}
