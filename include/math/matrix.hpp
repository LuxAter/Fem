#ifndef FEM_MATH_MATRIX_HPP_
#define FEM_MATH_MATRIX_HPP_

#include <array>
#include <iomanip>
#include <iostream>
#include <vector>

namespace fem {
namespace math {

  // TODO(Arden): Implement Tridiagonal matrix optimizations

  template <typename _Tp = double, typename _Al = std::allocator<_Tp>>
  class Vector {
   public:
    typedef _Al allocator_type;
    typedef typename _Al::value_type value_type;
    typedef typename std::vector<_Tp>::reference reference;
    typedef typename std::vector<_Tp>::const_reference const_reference;
    typedef typename _Al::difference_type difference_type;
    typedef typename _Al::size_type size_type;
    typedef typename _Al::pointer pointer;
    typedef typename _Al::const_pointer const_pointer;
    typedef typename std::vector<_Tp>::iterator iterator;
    typedef typename std::vector<_Tp>::const_iterator const_iterator;
    typedef std::reverse_iterator<iterator> reverse_iterator;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

    explicit Vector(const std::size_t& _N) : size_(_N), data_(_N, _Tp()) {}
    explicit Vector(const std::initializer_list<_Tp>& vec)
        : size_(vec.size()), data_(vec) {}
    Vector(const Vector& copy) : size_(copy.size_), data_(copy.data_) {}

    Vector& operator=(const Vector& vec) {
      size_ = vec.size_;
      data_ = vec.data_;
      return *this;
    }

    Vector& operator=(const std::initializer_list<_Tp>& vec) {
      size_ = vec.size();
      data_ = std::vector(vec);
      return *this;
    }

    constexpr inline size_type size() const noexcept { return size_; }
    iterator begin() { return data_.begin(); }
    const_iterator begin() const { return data_.begin(); }
    iterator end() { return data_.end(); }
    const_iterator end() const { return data_.end(); }

    reference operator[](size_type n) noexcept { return reference(data_[n]); }
    constexpr const_reference operator[](size_type n) const noexcept {
      return const_reference(data_[n]);
    }

    template <typename _T>
    Vector& operator+=(const _T& rhs) {
      *this = *this + rhs;
      return *this;
    }
    template <typename _T>
    Vector& operator-=(const _T& rhs) {
      *this = *this - rhs;
      return *this;
    }
    template <typename _T>
    Vector& operator*=(const _T& rhs) {
      *this = *this * rhs;
      return *this;
    }
    template <typename _T>
    Vector& operator/=(const _T& rhs) {
      *this = *this / rhs;
      return *this;
    }

   private:
    std::size_t size_;
    std::vector<_Tp> data_;
  };

  template <typename _Tp = double, typename _Al = std::allocator<_Tp>>
  class Matrix {
   public:
    typedef _Al allocator_type;
    typedef typename _Al::value_type value_type;
    typedef typename std::vector<std::vector<_Tp>>::reference reference;
    typedef
        typename std::vector<std::vector<_Tp>>::const_reference const_reference;
    typedef typename _Al::difference_type difference_type;
    typedef typename _Al::size_type size_type;
    typedef typename _Al::pointer pointer;
    typedef typename _Al::const_pointer const_pointer;
    typedef typename std::vector<std::vector<_Tp>>::iterator iterator;
    typedef
        typename std::vector<std::vector<_Tp>>::const_iterator const_iterator;
    typedef std::reverse_iterator<iterator> reverse_iterator;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

    explicit Matrix(std::size_t _N)
        : size_(_N), data_(_N, std::vector<_Tp>(_N, _Tp())) {}
    explicit Matrix(
        const std::initializer_list<std::initializer_list<_Tp>>& mat)
        : size_(mat.size()), data_(size_, std::vector<_Tp>(size_, _Tp())) {
      typename std::vector<std::vector<_Tp>>::iterator index = data_.begin();
      for (typename std::initializer_list<
               std::initializer_list<_Tp>>::const_iterator it = mat.begin();
           it != mat.end(); ++it) {
        if (it->size() == size_) {
          std::copy(it->begin(), it->end(), index->begin());
          ++index;
        }
      }
    }
    Matrix(const Matrix& copy) : size_(copy.size_), data_(copy.data_) {}

    Matrix& operator=(const Matrix& mat) {
      size_ = mat.size_;
      data_ = mat.data_;
      return *this;
    }
    Matrix& operator=(
        const std::initializer_list<std::initializer_list<_Tp>>& mat) {
      size_ = mat.size();
      typename std::vector<std::vector<_Tp>>::iterator index = data_.begin();
      for (typename std::initializer_list<
               std::initializer_list<_Tp>>::const_iterator it = mat.begin();
           it != mat.end(); ++it) {
        if (it->size() == size_) {
          std::copy(it->begin(), it->end(), index->begin());
          ++index;
        }
      }
      return *this;
    }

    constexpr inline size_type size() const noexcept { return size_; }
    iterator begin() { return data_.begin(); }
    const_iterator begin() const { return data_.begin(); }
    iterator end() { return data_.end(); }
    const_iterator end() const { return data_.end(); }

    reference operator[](size_type n) noexcept { return reference(data_[n]); }
    constexpr const_reference operator[](size_type n) const noexcept {
      return const_reference(data_[n]);
    }

    template <typename _T>
    inline fem::math::Matrix<_Tp>& operator+=(const _T& rhs) {
      *this = *this + rhs;
      return *this;
    }
    template <typename _T>
    inline fem::math::Matrix<_Tp>& operator-=(const _T& rhs) {
      *this = *this - rhs;
      return *this;
    }
    template <typename _T>
    inline fem::math::Matrix<_Tp>& operator*=(const _T& rhs) {
      *this = *this * rhs;
      return *this;
    }
    template <typename _T>
    inline fem::math::Matrix<_Tp>& operator/=(const _T& rhs) {
      *this = *this / rhs;
      return *this;
    }

   private:
    std::size_t size_;
    std::vector<std::vector<_Tp>> data_;
  };

  template <typename _T>
  std::ostream& operator<<(std::ostream& out, const Matrix<_T>& rhs) {
    out << "\u23a1" << std::fixed;
    for (std::size_t j = 0; j < rhs.size() - 1; ++j) {
      out << std::setw(10) << rhs[0][j] << ' ';
    }
    out << std::setw(10) << rhs[0][rhs.size() - 1] << "\u23a4\n";
    for (std::size_t i = 1; i < rhs.size() - 1; ++i) {
      out << "\u23a2";
      for (std::size_t j = 0; j < rhs.size() - 1; ++j) {
        out << std::setw(10) << rhs[i][j] << ' ';
      }
      out << std::setw(10) << rhs[i][rhs.size() - 1] << "\u23a5\n";
    }
    out << "\u23a3";
    for (std::size_t j = 0; j < rhs.size() - 1; ++j) {
      out << std::setw(10) << rhs[rhs.size() - 1][j] << ' ';
    }
    out << std::setw(10) << rhs[rhs.size() - 1][rhs.size() - 1] << "\u23a6\n";
    return out;
  }

  template <typename _T>
  std::ostream& operator<<(std::ostream& out, const Vector<_T>& rhs) {
    out << "\u27e8" << std::fixed;
    for (std::size_t j = 0; j < rhs.size() - 1; ++j) {
      out << std::setw(10) << rhs[j] << ' ';
    }
    out << std::setw(10) << rhs[rhs.size() - 1] << "\u27e9\n";
    return out;
  }

  template <typename _Tp>
  inline Vector<_Tp> operator+(const Vector<_Tp>& lhs, const _Tp& rhs) {
    Vector<_Tp> vec(lhs);
    for (auto& row : vec) {
      row += rhs;
    }
    return vec;
  }
  template <typename _Tp>
  inline Vector<_Tp> operator+(const Vector<_Tp>& lhs, const Vector<_Tp>& rhs) {
    Vector<_Tp> vec(std::min(lhs.size(), rhs.size()));
    for (std::size_t row = 0; row < std::min(lhs.size(), rhs.size()); ++row) {
      vec[row] = lhs[row] + rhs[row];
    }
    return vec;
  }
  template <typename _Tp>
  inline Vector<_Tp> operator-(const Vector<_Tp>& lhs, const _Tp& rhs) {
    Vector<_Tp> vec(lhs);
    for (auto& row : vec) {
      row -= rhs;
    }
    return vec;
  }
  template <typename _Tp>
  inline Vector<_Tp> operator-(const Vector<_Tp>& lhs, const Vector<_Tp>& rhs) {
    Vector<_Tp> vec(std::min(lhs.size(), rhs.size()));
    for (std::size_t row = 0; row < std::min(lhs.size(), rhs.size()); ++row) {
      vec[row] = lhs[row] - rhs[row];
    }
    return vec;
  }
  template <typename _Tp>
  inline Vector<_Tp> operator*(const Vector<_Tp>& lhs, const _Tp& rhs) {
    Vector<_Tp> vec(lhs);
    for (auto& row : vec) {
      row *= rhs;
    }
    return vec;
  }
  template <typename _Tp>
  inline Vector<_Tp> operator*(const Vector<_Tp>& lhs, const Vector<_Tp>& rhs) {
    Vector<_Tp> vec(std::min(lhs.size(), rhs.size()));
    for (std::size_t row = 0; row < std::min(lhs.size(), rhs.size()); ++row) {
      vec[row] = lhs[row] * rhs[row];
    }
    return vec;
  }
  template <typename _Tp>
  inline Vector<_Tp> operator/(const Vector<_Tp>& lhs, const _Tp& rhs) {
    Vector<_Tp> vec(lhs);
    for (auto& row : vec) {
      row /= rhs;
    }
    return vec;
  }
  template <typename _Tp>
  inline Vector<_Tp> operator/(const Vector<_Tp>& lhs, const Vector<_Tp>& rhs) {
    Vector<_Tp> vec(std::min(lhs.size(), rhs.size()));
    for (std::size_t row = 0; row < std::min(lhs.size(), rhs.size()); ++row) {
      vec[row] = lhs[row] / rhs[row];
    }
    return vec;
  }

  template <typename _Tp>
  inline Matrix<_Tp> operator+(const Matrix<_Tp>& lhs, const _Tp& rhs) {
    Matrix<_Tp> mat(lhs);
    for (auto& row : mat) {
      for (auto& col : row) {
        col += rhs;
      }
    }
    return mat;
  }
  template <typename _Tp>
  inline Matrix<_Tp> operator+(const Matrix<_Tp>& lhs, const Matrix<_Tp>& rhs) {
    Matrix<_Tp> mat(std::min(lhs.size(), rhs.size()));
    for (std::size_t row = 0; row < std::min(lhs.size(), rhs.size()); ++row) {
      for (std::size_t col = 0;
           col < std::min(lhs[row].size(), rhs[row].size()); ++col) {
        mat[row][col] = lhs[row][col] + rhs[row][col];
      }
    }
    return mat;
  }
  template <typename _Tp>
  inline Matrix<_Tp> operator-(const Matrix<_Tp>& lhs, const _Tp& rhs) {
    Matrix<_Tp> mat(lhs);
    for (auto& row : mat) {
      for (auto& col : row) {
        col -= rhs;
      }
    }
    return mat;
  }
  template <typename _Tp>
  inline Matrix<_Tp> operator-(const Matrix<_Tp>& lhs, const Matrix<_Tp>& rhs) {
    Matrix<_Tp> mat(std::min(lhs.size(), rhs.size()));
    for (std::size_t row = 0; row < std::min(lhs.size(), rhs.size()); ++row) {
      for (std::size_t col = 0;
           col < std::min(lhs[row].size(), rhs[row].size()); ++col) {
        mat[row][col] = lhs[row][col] - rhs[row][col];
      }
    }
    return mat;
  }
  template <typename _Tp>
  inline Matrix<_Tp> operator*(const Matrix<_Tp>& lhs, const _Tp& rhs) {
    Matrix<_Tp> mat(lhs);
    for (auto& row : mat) {
      for (auto& col : row) {
        col *= rhs;
      }
    }
    return mat;
  }
  template <typename _Tp>
  inline Matrix<_Tp> operator*(const Matrix<_Tp>& lhs, const Matrix<_Tp>& rhs) {
    Matrix<_Tp> mat(std::min(lhs.size(), rhs.size()));
    for (std::size_t row = 0; row < std::min(lhs.size(), rhs.size()); ++row) {
      for (std::size_t col = 0;
           col < std::min(lhs[row].size(), rhs[row].size()); ++col) {
        mat[row][col] = lhs[row][col] * rhs[row][col];
      }
    }
    return mat;
  }
  template <typename _Tp>
  inline Matrix<_Tp> operator/(const Matrix<_Tp>& lhs, const _Tp& rhs) {
    Matrix<_Tp> mat(lhs);
    for (auto& row : mat) {
      for (auto& col : row) {
        col /= rhs;
      }
    }
    return mat;
  }
  template <typename _Tp>
  inline Matrix<_Tp> operator/(const Matrix<_Tp>& lhs, const Matrix<_Tp>& rhs) {
    Matrix<_Tp> mat(std::min(lhs.size(), rhs.size()));
    for (std::size_t row = 0; row < std::min(lhs.size(), rhs.size()); ++row) {
      for (std::size_t col = 0;
           col < std::min(lhs[row].size(), rhs[row].size()); ++col) {
        mat[row][col] = lhs[row][col] / rhs[row][col];
      }
    }
    return mat;
  }

  template <typename _Tp>
  Vector<_Tp> TDM(Matrix<_Tp> mat, Vector<_Tp> vec) {
    if (vec.size() != mat.size()) {
      return Vector(0);
    }
    for (std::size_t k = 1; k < vec.size(); ++k) {
      mat[k][k] =
          mat[k][k] - mat[k][k - 1] * (mat[k - 1][k] / mat[k - 1][k - 1]);
      vec[k] = vec[k] - mat[k][k - 1] * (vec[k - 1] / mat[k - 1][k - 1]);
    }
    Vector x(vec.size());
    x[vec.size() - 1] =
        vec[vec.size() - 1] / mat[vec.size() - 1][vec.size() - 1];
    for (int k = vec.size() - 2; k >= 0; --k) {
      x[k] = (vec[k] - mat[k][k + 1] * x[k + 1]) / mat[k][k];
    }
    return x;
  }

}  // namespace math
}  // namespace fem

#endif  // FEM_MATH_MATRIX_HPP_
