#ifndef FEM_MATH_MATRIX_HPP_
#define FEM_MATH_MATRIX_HPP_

#include <array>
#include <iostream>
#include <vector>
#include <iomanip>

namespace fem {
namespace math {

  // TODO(Arden): Implement Tridiagonal matrix optimizations

  template <typename _T, std::size_t _N>
  using Matrix = std::array<std::array<_T, _N>, _N>;
  template <typename _T, std::size_t _N>
  using Vector = std::array<_T, _N>;

  template<typename _T, std::size_t _N>
    Matrix<_T, _N> Zeros(){
      Matrix<_T, _N> mat;
      for(std::size_t i = 0; i < _N; ++i){
        for(std::size_t j = 0; j < _N; ++j){
          mat[i][j] = 0.0;
        }
      }
      return mat;
    }
  template<typename _T, std::size_t _N>
    Vector<_T, _N> VZeros(){
      Vector<_T, _N> vec;
      for(std::size_t i = 0; i < _N; ++i){
        vec[0] = 0.0;
      }
      return vec;
    }

  template <typename _T, std::size_t _N>
  std::array<_T, _N> TDM(std::array<std::array<_T, _N>, _N> mat,
                         std::array<_T, _N> vec) {
    for (std::size_t k = 1; k < _N; ++k) {
      mat[k][k] =
          mat[k][k] - mat[k][k - 1] * (mat[k - 1][k] / mat[k - 1][k - 1]);
      vec[k] = vec[k] - mat[k][k - 1] * (vec[k - 1] / mat[k - 1][k - 1]);
    }
    std::array<_T, _N> x;
    x[_N - 1] = vec[_N - 1] / mat[_N - 1][_N - 1];
    for (int k = _N - 2; k >= 0; --k) {
      x[k] = (vec[k] - mat[k][k + 1] * x[k + 1]) / mat[k][k];
    }
    return x;
  }

  template <typename _T, std::size_t _N>
  Matrix<_T, _N> operator+(const Matrix<_T, _N>& lhs,
                           const Matrix<_T, _N>& rhs) {
    Matrix<_T, _N> mat;
    for (std::size_t row = 0; row < _N; ++row) {
      for (std::size_t col = 0; col < _N; ++col) {
        mat[row][col] = lhs[row][col] + rhs[row][col];
      }
    }
    return mat;
  }
  template <typename _T, std::size_t _N>
  Matrix<_T, _N> operator+(const Matrix<_T, _N>& lhs, const _T& rhs) {
    Matrix<_T, _N> mat;
    for (std::size_t row = 0; row < _N; ++row) {
      for (std::size_t col = 0; col < _N; ++col) {
        mat[row][col] = lhs[row][col] + rhs;
      }
    }
    return mat;
  }
  template <typename _T, std::size_t _N>
  Matrix<_T, _N> operator+(const _T& lhs, const Matrix<_T, _N>& rhs) {
    Matrix<_T, _N> mat;
    for (std::size_t row = 0; row < _N; ++row) {
      for (std::size_t col = 0; col < _N; ++col) {
        mat[row][col] = lhs[row][col] + rhs;
      }
    }
    return mat;
  }
  template <typename _T, std::size_t _N>
  Matrix<_T, _N> operator-(const Matrix<_T, _N>& lhs,
                           const Matrix<_T, _N>& rhs) {
    Matrix<_T, _N> mat;
    for (std::size_t row = 0; row < _N; ++row) {
      for (std::size_t col = 0; col < _N; ++col) {
        mat[row][col] = lhs[row][col] - rhs[row][col];
      }
    }
    return mat;
  }
  template <typename _T, std::size_t _N>
  Matrix<_T, _N> operator*(const _T& lhs, const Matrix<_T, _N>& rhs) {
    Matrix<_T, _N> mat;
    for (std::size_t row = 0; row < _N; ++row) {
      for (std::size_t col = 0; col < _N; ++col) {
        mat[row][col] = lhs * rhs[row][col];
      }
    }
    return mat;
  }

  template <typename _T, std::size_t _N>
  std::ostream& operator<<(std::ostream& out, const Vector<_T, _N>& rhs) {
    out << "<";
    for (std::size_t i = 0; i < _N - 1; ++i) {
      out << rhs[i] << ", ";
    }
    out << rhs[_N - 1] << ">";
    return out;
  }

  template <typename _T, std::size_t _N>
  std::ostream& operator<<(std::ostream& out, const Matrix<_T, _N>& rhs) {
    out << "\u23a1" << std::fixed;
    for (std::size_t j = 0; j < _N - 1; ++j) {
      out << std::setw(10) << rhs[0][j] << ' ';
    }
    out << std::setw(10) << rhs[0][_N - 1] << "\u23a4\n";
    for (std::size_t i = 1; i < _N - 1; ++i) {
      out << "\u23a2";
      for (std::size_t j = 0; j < _N - 1; ++j) {
        out << std::setw(10) << rhs[i][j] << ' ';
      }
      out << std::setw(10) << rhs[i][_N - 1] << "\u23a5\n";
    }
    out << "\u23a3";
    for (std::size_t j = 0; j < _N - 1; ++j) {
      out << std::setw(10) << rhs[_N - 1][j] << ' ';
    }
    out << std::setw(10) << rhs[_N - 1][_N - 1] << "\u23a6\n";
    return out;
  }

}  // namespace math
}  // namespace fem

#endif  // FEM_MATH_MATRIX_HPP_
