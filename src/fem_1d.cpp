#include "fem_1d.hpp"

#include <functional>

#include "math/differentiate.hpp"
#include "math/function.hpp"
#include "math/integrate.hpp"
#include "math/matrix.hpp"
#include "util/range.hpp"

// inline fem::d1::Func fem::d1::Phie1(const uint32_t& e, const double& h) {
//   return [e, h](double x) { return -x / h + e; };
// }
// inline fem::d1::Func fem::d1::Phie2(const uint32_t& e, const double& h) {
//   return [e, h](double x) { return x / h - e + 1; };
// }
// inline fem::d1::Func fem::d1::Phie(const uint32_t& e, const double& h,
//                           const uint32_t& N) {
//   if (e == 1) {
//     return fem::math::GenPiecewise(Phie1(1, h), {{0, h}});
//   } else if (e == N) {
//     return fem::math::GenPiecewise(Phie2(N - 1, h),
//                                    {{(N - 2) * h, (N - 1) * h}});
//   } else {
//     return fem::math::GenPiecewise(Phie2(e - 1, h),
//                                    {{(e - 2) * h, (e - 1) * h}}, Phie1(e, h),
//                                    {{(e - 1) * h, e * h}});
//   }
// }

double fem::d1::Ifg(const fem::d1::Func& f, const fem::d1::Func& g,
                    const double& a, const double& b) {
  return fem::math::Integrate<double>([f, g](double x) { return f(x) * g(x); },
                                      a, b);
}
double fem::d1::Ifdg(const fem::d1::Func& f, const fem::d1::Func& g,
                     const double& a, const double& b) {
  return fem::math::Integrate<double>(
      [f, g](double x) {
        return fem::math::Derivative(f)(x) * fem::math::Derivative(g)(x);
      },
      a, b);
}
double fem::d1::Idfdg(const fem::d1::Func& f, const fem::d1::Func& g,
                      const double& a, const double& b) {
  return fem::math::Integrate<double>(
      [f, g](double x) { return f(x) * fem::math::Derivative(g)(x); }, a, b);
}

std::pair<fem::math::Matrix<double>, fem::math::Vector<double>>
fem::d1::GenElement(const fem::d1::Func& f, const uint32_t& e,
                    const double& h) {
  fem::math::Matrix<double> Ae(2);
  Ae[0][0] = Ifdg(Phie1(e, h), Phie1(e, h), (e - 1) * h, e * h) +
             Idfdg(Phie1(e, h), Phie1(e, h), (e - 1) * h, e * h);
  Ae[0][1] = Ifdg(Phie1(e, h), Phie2(e, h), (e - 1) * h, e * h) +
             Idfdg(Phie1(e, h), Phie2(e, h), (e - 1) * h, e * h);
  Ae[1][0] = Ifdg(Phie2(e, h), Phie1(e, h), (e - 1) * h, e * h) +
             Idfdg(Phie2(e, h), Phie1(e, h), (e - 1) * h, e * h);
  Ae[1][1] = Ifdg(Phie2(e, h), Phie2(e, h), (e - 1) * h, e * h) +
             Idfdg(Phie2(e, h), Phie2(e, h), (e - 1) * h, e * h);

  fem::math::Vector<double> Fe(2);
  Fe[0] = Ifg(Phie1(e, h), f, (e - 1) * h, e * h);
  Fe[1] = Ifg(Phie2(e, h), f, (e - 1) * h, e * h);

  return std::make_pair(Ae, Fe);
}

std::pair<fem::math::Matrix<double>, fem::math::Vector<double>>
fem::d1::ApplyDirchlet(const math::Matrix<double>& A,
                       const math::Vector<double>& F, const double& T0,
                       const double& Tc, const uint32_t& N) {
  fem::math::Matrix<double> A_bc = A;
  for (std::size_t i = 0; i < N; ++i) {
    A_bc[0][i] = (i == 0) ? 1 : 0;
    A_bc[A.size() - 1][i] = (i == A.size() - 1) ? 1 : 0;
  }
  fem::math::Vector<double> F_bc = F;
  F_bc[0] = T0;
  F_bc[F.size() - 1] = Tc;
  return std::make_pair(A_bc, F_bc);
}

fem::d1::Func fem::d1::GenApprox(const math::Vector<double>& T, const double& h,
                                 const uint32_t& N) {
  return [T, h, N](double x) {
    double y = 0;
    for (std::size_t i = 0; i < N; ++i) {
      y += (T[i] * Phie(i + 1, h, N)(x));
    }
    return y;
  };
}

double fem::d1::CalcError(const fem::d1::Func& f, const fem::d1::Func& g,
                          const double& a, const double& b) {
  return pow(
      fem::math::Integrate<double>(
          [f, g](double x) { return pow(fabs(f(x) - g(x)), 2); }, a, b, 1000),
      0.5);
}

fem::d1::Func fem::d1::FEA(const fem::d1::Func& f, const double& c,
                           const double& T0, const double& Tc,
                           const uint32_t& N) {
  double h = c / (N - 1);
  fem::math::Matrix<double> A(N);
  fem::math::Vector<double> F(N);
  for (uint32_t e = 0; e < N - 1; ++e) {
    std::pair<fem::math::Matrix<double>, fem::math::Vector<double>> E =
        GenElement(f, e + 1, h);
    A[e][e] += E.first[0][0];
    A[e][e + 1] += E.first[0][1];
    A[e + 1][e] += E.first[1][0];
    A[e + 1][e + 1] += E.first[1][1];
    F[e] += E.second[0];
    F[e + 1] += E.second[1];
  }
  // std::cout << A << F << "<<\n";
  std::pair<fem::math::Matrix<double>, fem::math::Vector<double>> EQ =
      ApplyDirchlet(A, F, T0, Tc, N);
  fem::math::Vector<double> T = fem::math::TDM(EQ.first, EQ.second);

  // std::cout << T << "\n";
  return GenApprox(T, h, N);
}
