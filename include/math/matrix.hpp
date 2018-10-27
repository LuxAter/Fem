#ifndef FEM_MATH_MATRIX_HPP_
#define FEM_MATH_MATRIX_HPP_

#include <array>
#include <vector>

namespace fem {
namespace math {
  template <typename _T, std::size_t _N>
    using Matrix = std::array<std::array<_T, _N>, _N>;
  template <typename _T, std::size_t _N>
    using Vector= std::array<std::array<_T, _N>, _N>;


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

  template<typename _T, std::size_t _N>
    Matrix<_T,_N> operator+(const Matrix<_T, _N>& lhs, const Matrix<_T,_N>&rhs){
      Matrix<_T, _N> mat;
      for(std::size_t row = 0; row < _N; ++row){
        for(std::size_t col = 0; col < _N; ++col){
          mat[row][col] = lhs[row][col] + rhs[row][col];
        }
      }
    }
  template<typename _T, std::size_t _N>
    Matrix<_T,_N> operator+(const Matrix<_T, _N>& lhs, const _T& rhs){
      Matrix<_T, _N> mat;
      for(std::size_t row = 0; row < _N; ++row){
        for(std::size_t col = 0; col < _N; ++col){
          mat[row][col] = lhs[row][col] + rhs;
        }
      }
    }
  template<typename _T, std::size_t _N>
    Matrix<_T,_N> operator+(const _T& lhs, const Matrix<_T, _N>& rhs){
      Matrix<_T, _N> mat;
      for(std::size_t row = 0; row < _N; ++row){
        for(std::size_t col = 0; col < _N; ++col){
          mat[row][col] = lhs[row][col] + rhs;
        }
      }
    }
  template<typename _T, std::size_t _N>
    Matrix<_T,_N> operator-(const Matrix<_T, _N>& lhs, const Matrix<_T,_N>&rhs){
      Matrix<_T, _N> mat;
      for(std::size_t row = 0; row < _N; ++row){
        for(std::size_t col = 0; col < _N; ++col){
          mat[row][col] = lhs[row][col] - rhs[row][col];
        }
      }
    }

}  // namespace math
}  // namespace fem

#endif  // FEM_MATH_MATRIX_HPP_
