#include <iostream>
#include <string>

#include <functional>
#include <chrono>

// #include "async.hpp"
#include "image/figure.hpp"
// #include "image/svg.hpp"
// #include "math/differentiate.hpp"
// #include "math/function.hpp"
// #include "math/integrate.hpp"
#include "math/matrix.hpp"
// #include "mesh/delaunay.hpp"
// #include "mesh/mesh.hpp"
// #include "mesh/pslg.hpp"
// #include "mesh/shape.hpp"
#include "util/range.hpp"
//
#include "fem_1d.hpp"

using namespace fem::math;
//
// inline std::function<double(double)> Phie1(const uint32_t& e, const double&
// h) {
//   return [e, h](double x) { return -x / h + e; };
// }
// inline std::function<double(double)> Phie2(const uint32_t& e, const double&
// h) {
//   return [e, h](double x) { return x / h - e + 1; };
// }
//
// inline std::function<double(double)> Phie(const uint32_t& e, const double& h)
// {
//   if (e == 1) {
//     return GenPiecewise(Phie1(1, h), {{0, h}});
//   } else if (e == N) {
//     return GenPiecewise(Phie2(N - 1, h), {{(N - 2) * h, (N - 1) * h}});
//   } else {
//     return GenPiecewise(Phie2(e - 1, h), {{(e - 2) * h, (e - 1) * h}},
//                         Phie1(e, h), {{(e - 1) * h, e * h}});
//   }
// }
//
// double Ifdg(const std::function<double(double)>& f,
//             const std::function<double(double)>& g, const double& a,
//             const double& b) {
//   return Integrate<double>([f, g](double x) { return f(x) * Derivative(g)(x);
//   },
//                            a, b);
// }
// double Idfdg(const std::function<double(double)>& f,
//              const std::function<double(double)>& g, const double& a,
//              const double& b) {
//   return Integrate<double>(
//       [f, g](double x) { return Derivative(f)(x) * Derivative(g)(x); }, a,
//       b);
// }
// double Ifg(const std::function<double(double)>& f,
//            const std::function<double(double)>& g, const double& a,
//            const double& b) {
//   return Integrate<double>([f, g](double x) { return f(x) * g(x); }, a, b);
// }
//
// std::pair<Matrix<double, 2>, Vector<double, 2>> GenElement(
//     const std::function<double(double)>& f, const uint32_t& e,
//     const double& h) {
//   Matrix<double, 2> Ae;
//   Ae[0][0] = Ifdg(Phie1(e, h), Phie1(e, h), (e - 1) * h, e * h) +
//              Idfdg(Phie1(e, h), Phie1(e, h), (e - 1) * h, e * h);
//   Ae[0][1] = Ifdg(Phie1(e, h), Phie2(e, h), (e - 1) * h, e * h) +
//              Idfdg(Phie1(e, h), Phie2(e, h), (e - 1) * h, e * h);
//   Ae[1][0] = Ifdg(Phie2(e, h), Phie1(e, h), (e - 1) * h, e * h) +
//              Idfdg(Phie2(e, h), Phie1(e, h), (e - 1) * h, e * h);
//   Ae[1][1] = Ifdg(Phie2(e, h), Phie2(e, h), (e - 1) * h, e * h) +
//              Idfdg(Phie2(e, h), Phie2(e, h), (e - 1) * h, e * h);
//
//   Vector<double, 2> Fe;
//   Fe[0] = Ifg(Phie1(e, h), f, (e - 1) * h, e * h);
//   Fe[1] = Ifg(Phie2(e, h), f, (e - 1) * h, e * h);
//
//   return std::make_pair(Ae, Fe);
// }
//
// std::pair<Matrix<double, N>, Vector<double, N>> ApplyDirchletBc(
//     const Matrix<double, N>& A, const Vector<double, N>& F, const double& T0,
//     const double& Tc) {
//   Matrix<double, N> A_bc = A;
//   for (std::size_t i = 0; i < N; ++i) {
//     A_bc[0][i] = (i == 0) ? 1 : 0;
//     A_bc[N - 1][i] = (i == N - 1) ? 1 : 0;
//   }
//   Vector<double, N> F_bc = F;
//   F_bc[0] = T0;
//   F_bc[N - 1] = Tc;
//   return std::make_pair(A_bc, F_bc);
// }
//
// std::function<double(double)> GenApprox(const Vector<double, N>& T,
//                                         const double& h) {
//   return [T, h](double x) {
//     double y = 0;
//     for (std::size_t i = 0; i < N; ++i) {
//       y += (T[i] * Phie(i + 1, h)(x));
//     }
//     return y;
//   };
// }
//
// double CalcError(const std::function<double(double)>& f,
//                  const std::function<double(double)>& g, const double& a,
//                  const double& b) {
//   return pow(
//       Integrate<double>([f, g](double x) { return pow(fabs(f(x) - g(x)), 2);
//       },
//                         a, b, 1000),
//       0.5);
// }

int main(int argc, char* argv[]) {
  double c = 5;
  double T0 = 0;
  double Tc = 0;
  uint32_t nu = 2;
  //
  // // sin(x)
  std::function<double(double)> f = [c, nu](double x) {
    return nu * M_PI / c *
           (cos(nu * M_PI * x / c) + (nu * M_PI / c) * sin(nu * M_PI * x / c));
  };
  std::function<double(double)> soln = [c, nu](double x) {
    return sin(nu * M_PI * x / c);
  };
  //
  fem::image::Figure err(1000, 1000);
  fem::image::Figure tim(1000, 1000);
  // fig.Line(fem::util::StepRangeArray<double, 200>(0.0, c),
  //          fem::util::FuncStepRangeArray<double, 200>(soln, 0.0, c), "", "",
  //          "", 5);
#define C 10000
  std::array<double, C> Errors;
  std::array<double, C> Times;
  auto begin = std::chrono::high_resolution_clock::now();
  auto Th = fem::d1::FEA(f, c, T0, Tc, C);
  auto end = std::chrono::high_resolution_clock::now();
  double dt = std::chrono::duration_cast<std::chrono::nanoseconds>(end-begin).count() * 1.0e-9;
  double error = fem::d1::CalcError(Th, soln, 0.0, c);
  std::cout << C << "\n";
  std::cout << error << "\n";
  std::cout << dt << "\n";
  // fig.SavePgf("example.tikz");
  // fig.SaveSvg("example.svg");
  // for (uint32_t N = 2; N < C + 2; ++N) {
  //   // fem::image::Figure fig(1000, 1000);
  //   auto begin = std::chrono::high_resolution_clock::now();
  //   auto Th = fem::d1::FEA(f, c, T0, Tc, N);
  //   auto end = std::chrono::high_resolution_clock::now();
  //   double error = fem::d1::CalcError(Th, soln, 0.0, c);
  //   double dt = std::chrono::duration_cast<std::chrono::nanoseconds>(end-begin).count() * 1.0e-9;
  //   Times[N-2] = dt;
  //   Errors[N - 2] = error;
  //   std::cout << N << ":" << error << ":" << dt << "\n";
  //   // fig.Line(fem::util::StepRangeArray<double, 200>(0.0, c),
  //   //          fem::util::FuncStepRangeArray<double, 200>(soln, 0.0, c), "black",
  //   //          "black", "black", 5, 10);
  //   // fig.Line(fem::util::StepRangeArray<double, 200>(0.0, c),
  //   //          fem::util::FuncStepRangeArray<double, 200>(Th, 0.0, c), "black",
  //   //          "black", "black", 5);
  //   // fig.SaveSvg(fem::image::GenFileName("animation", "svg", N - 2));
  // }
  // err.Line(fem::util::StepRangeArray<double, C>(2, C + 2), Errors, "black", "black", "black", 5);
  // // err.Line(fem::util::StepRangeArray<double, C>(2, C + 2), Times, "", "", "", 5);
  // err.Line(fem::util::StepRangeArray<double, C>(2, C + 2), fem::util::FuncStepRangeArray<double, C>([](double x){return 1;}, 2, C + 2), "black", "black", "black", 5, 10);
  // err.Line(fem::util::StepRangeArray<double, C>(2, C + 2), fem::util::FuncStepRangeArray<double, C>([](double x){return 0.1;}, 2, C + 2), "black", "black", "black", 5, 10);
  // err.Line(fem::util::StepRangeArray<double, C>(2, C + 2), fem::util::FuncStepRangeArray<double, C>([](double x){return 0.01;}, 2, C + 2), "black", "black", "black", 5, 10);
  //
  // err.SavePgf("error.tikz");
  // err.SaveSvg("error.svg");
  //
  // tim.Line(fem::util::StepRangeArray<double, C>(2, C + 2), Times, "", "", "", 5);
  // // tim.Line(fem::util::StepRangeArray<double, C>(2, C + 2), fem::util::FuncStepRangeArray<double, C>([](double x){return 1;}, 2, C + 2), "black", "black", "black", 5, 10);
  // // tim.Line(fem::util::StepRangeArray<double, C>(2, C + 2), fem::util::FuncStepRangeArray<double, C>([](double x){return 0.1;}, 2, C + 2), "black", "black", "black", 5, 10);
  // // tim.Line(fem::util::StepRangeArray<double, C>(2, C + 2), fem::util::FuncStepRangeArray<double, C>([](double x){return 0.01;}, 2, C + 2), "black", "black", "black", 5, 10);
  // tim.SavePgf("time.tikz");
  // tim.SaveSvg("time.svg");
  // fig.SaveSvg("test.svg");

  return 0;
}
