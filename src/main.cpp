#include <iostream>
#include <string>

#include <functional>

#include "async.hpp"
#include "image/figure.hpp"
#include "image/svg.hpp"
#include "math/differentiate.hpp"
#include "math/function.hpp"
#include "math/integrate.hpp"
#include "math/matrix.hpp"
#include "mesh/delaunay.hpp"
#include "mesh/mesh.hpp"
#include "mesh/pslg.hpp"
#include "mesh/shape.hpp"
#include "util/range.hpp"

#define N 100
#define nu 3

using namespace fem::math;

inline std::function<double(double)> Phie1(const uint32_t& e, const double& h) {
  return [e, h](double x) { return -x / h + e; };
}
inline std::function<double(double)> Phie2(const uint32_t& e, const double& h) {
  return [e, h](double x) { return x / h - e + 1; };
}

inline std::function<double(double)> Phie(const uint32_t& e,
                                          const double& h) {
  if (e == 1) {
    return GenPiecewise(Phie1(1, h), {{0, h}});
  } else if (e == N) {
    return GenPiecewise(Phie2(N - 1, h), {{(N - 2) * h, (N - 1) * h}});
  } else {
    return GenPiecewise(Phie2(e - 1, h), {{(e - 2) * h, (e - 1) * h}},
                        Phie1(e, h), {{(e - 1) * h, e * h}});
  }
}

double Ifdg(const std::function<double(double)>& f,
            const std::function<double(double)>& g, const double& a,
            const double& b) {
  return Integrate<double>([f, g](double x) { return f(x) * Derivative(g)(x); },
                           a, b);
}
double Idfdg(const std::function<double(double)>& f,
             const std::function<double(double)>& g, const double& a,
             const double& b) {
  return Integrate<double>(
      [f, g](double x) { return Derivative(f)(x) * Derivative(g)(x); }, a, b);
}
double Ifg(const std::function<double(double)>& f,
           const std::function<double(double)>& g, const double& a,
           const double& b) {
  return Integrate<double>([f, g](double x) { return f(x) * g(x); }, a, b);
}

std::pair<Matrix<double, 2>, Vector<double, 2>> GenElement(
    const std::function<double(double)>& f, const uint32_t& e,
    const double& h) {
  Matrix<double, 2> Ae;
  Ae[0][0] = Ifdg(Phie1(e, h), Phie1(e, h), (e - 1) * h, e * h) +
             Idfdg(Phie1(e, h), Phie1(e, h), (e - 1) * h, e * h);
  Ae[0][1] = Ifdg(Phie1(e, h), Phie2(e, h), (e - 1) * h, e * h) +
             Idfdg(Phie1(e, h), Phie2(e, h), (e - 1) * h, e * h);
  Ae[1][0] = Ifdg(Phie2(e, h), Phie1(e, h), (e - 1) * h, e * h) +
             Idfdg(Phie2(e, h), Phie1(e, h), (e - 1) * h, e * h);
  Ae[1][1] = Ifdg(Phie2(e, h), Phie2(e, h), (e - 1) * h, e * h) +
             Idfdg(Phie2(e, h), Phie2(e, h), (e - 1) * h, e * h);

  Vector<double, 2> Fe;
  Fe[0] = Ifg(Phie1(e, h), f, (e - 1) * h, e * h);
  Fe[1] = Ifg(Phie2(e, h), f, (e - 1) * h, e * h);

  return std::make_pair(Ae, Fe);
}

std::pair<Matrix<double, N>, Vector<double, N>> ApplyDirchletBc(
    const Matrix<double, N>& A, const Vector<double, N>& F, const double& T0,
    const double& Tc) {
  Matrix<double, N> A_bc = A;
  for(std::size_t i = 0; i < N; ++i){
    A_bc[0][i] = (i == 0) ? 1 : 0;
    A_bc[N-1][i] = (i == N-1) ? 1 : 0;
  }
  Vector<double, N> F_bc = F;
  F_bc[0] = T0;
  F_bc[N-1] = Tc;
  return std::make_pair(A_bc, F_bc);
}

std::function<double(double)> GenApprox(const Vector<double, N>& T, const double& h){
  return [T, h](double x) {
    double y = 0;
    for(std::size_t i = 0; i < N; ++i){
      y += (T[i] * Phie(i+1, h)(x));
    }
    return y;
  };
}

int main(int argc, char* argv[]) {
  double c = 2*M_PI;
  double T0 = 0;
  double Tc = 0;

  std::function<double(double)> f = [c](double x) { return nu*M_PI/c * (cos(nu*M_PI*x/c) + (nu*M_PI/c)*sin(nu*M_PI*x/c)); };
  std::function<double(double)> soln = [c](double x) {return sin(nu*M_PI*x/c);};


  double h = c / (N - 1);

  Matrix<double, N> A = Zeros<double, N>();
  Vector<double, N> F;

  for (uint32_t e = 0; e < N - 1; ++e) {
    std::pair<Matrix<double, 2>, Vector<double, 2>> E = GenElement(f, e + 1, h);
    A[e][e] += E.first[0][0];
    A[e][e + 1] += E.first[0][1];
    A[e + 1][e] += E.first[1][0];
    A[e + 1][e + 1] += E.first[1][1];
    F[e] += E.second[0];
    F[e + 1] += E.second[1];
  }
  std::cout << A << F << "\n";
  std::pair<Matrix<double, N>, Vector<double, N>> EQ =
      ApplyDirchletBc(A, F, T0, Tc);
  Vector<double, N> T = TDM(EQ.first, EQ.second);
  auto Th = GenApprox(T, h);

  fem::image::Figure fig(500, 500);
  fig.Line(fem::util::StepRangeArray<double, 100>(0.0, c),
           fem::util::FuncStepRangeArray<double, 100>(Th, 0.0, c));
  fig.Line(fem::util::StepRangeArray<double, 100>(0.0, c),
           fem::util::FuncStepRangeArray<double, 100>(soln, 0.0, c));

  fig.SaveSvg("test.svg");

  return 0;
}
