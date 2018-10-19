#ifndef FEM_MATH_MATRIX_HPP_
#define FEM_MATH_MATRIX_HPP_

#include <array>
#include <vector>

namespace fem {
namespace math {
  // template <typename _T, typename _Al = std::allocator<_T>>
  // class Matrix {
  //  public:
  //
  //    typedef _Al allocator_type;
  //    typedef typename _Al::value_type value_type;
  //    typedef typename _Al::reference reference;
  //    typedef typename _Al::pointer pointer;
  //    typedef typename _Al::pointer iterator;
  //
  //   Matrix(std::size_t w, std::size_t h)
  //       : __data(h, std::vector<_T>(w, _T())) {}
  //   Matrix(const std::size_t w, std::size_t h, _T val)
  //       : __data(h, std::vector<_T>(w, val)) {}
  //   Matrix(const std::initializer_list<std::initializer_list<_T>>& mat){
  //     // iterator index = begin();
  //     // for(typename
  //     std::initializer_list<std::initializer_list<_T>>::const_iterator it =
  //     mat.begin(); it != mat.end(); ++it){
  //     //
  //     // }
  //   }
  //
  //  private:
  //   std::vector<std::vector<_T>> __data;
  // };
  template <typename _T, std::size_t _N>
  std::array<_T, _N> TDM(std::array<std::array<_T, _N>, _N> mat,
                      std::array<_T, _N> vec) {
    for (std::size_t k = 1; k < _N; ++k) {
      mat[k][k] =
          mat[k][k] - mat[k][k - 1] * (mat[k - 1][k] / mat[k - 1][k - 1]);
      vec[k] = vec[k] - mat[k][k - 1] * (vec[k - 1] / mat[k - 1][k - 1]);
    }
    std::array<_T, _N> x;
    x[_N-1] = vec[_N-1]/mat[_N-1][_N-1];
    for(int k = _N-2; k >= 0; --k){
      x[k]=(vec[k]-mat[k][k+1]*x[k+1])/mat[k][k];
    }
    return x;
  }
}  // namespace math
}  // namespace fem

#endif  // FEM_MATH_MATRIX_HPP_
