#ifndef FEM_1D_HPP_
#define FEM_1D_HPP_

#include <functional>

#include "math/differentiate.hpp"
#include "math/function.hpp"
#include "math/integrate.hpp"
#include "math/matrix.hpp"
#include "util/range.hpp"

namespace fem {
namespace d1 {

  typedef typename std::function<double(double)> Func;

  inline Func Phie1(const uint32_t& e, const double& h) {
    return [e, h](double x) { return -x / h + e; };
  }
  inline Func Phie2(const uint32_t& e, const double& h) {
    return [e, h](double x) { return x / h - e + 1; };
  }
  inline Func Phie(const uint32_t& e, const double& h, const uint32_t& N) {
    if (e == 1) {
      return fem::math::GenPiecewise(Phie1(1, h), {{0, h}});
    } else if (e == N) {
      return fem::math::GenPiecewise(Phie2(N - 1, h),
                                     {{(N - 2) * h, (N - 1) * h}});
    } else {
      return fem::math::GenPiecewise(Phie2(e - 1, h),
                                     {{(e - 2) * h, (e - 1) * h}}, Phie1(e, h),
                                     {{(e - 1) * h, e * h}});
    }
  }

  double Ifg(const Func& f, const Func& g, const double& a, const double& b);
  double Ifdg(const Func& f, const Func& g, const double& a, const double& b);
  double Idfdg(const Func& f, const Func& g, const double& a, const double& b);

  std::pair<math::Matrix<double>, math::Vector<double>> GenElement(
      const Func& f, const uint32_t& e, const double& h);

  std::pair<math::Matrix<double>, math::Vector<double>> ApplyDirchlet(
      const math::Matrix<double>& A, const math::Vector<double>& F,
      const double& T0, const double& Tc, const uint32_t& N);

  Func GenApprox(const math::Vector<double>& T, const double& h,
                 const uint32_t& N);

  double CalcError(const Func& f, const Func& g, const double& a,
                   const double& b);

  Func FEA(const Func& f, const double& c, const double& T0, const double& Tc,
           const uint32_t& N);

}  // namespace d1
}  // namespace fem

#endif  // FEM_1D_HPP_
