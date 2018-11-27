#ifndef FEM_MATH_SPARSE_HPP_
#define FEM_MATH_SPARSE_HPP_

#include <array>
#include <iomanip>
#include <iostream>
#include <vector>

namespace fem {
namespace math {

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

    explicit Vector(std::size_t _N) : size_(_N) {}
    Vector(const Vector& copy)
        : size_(copy.size_), ind_(copy.ind_), values_(copy.values_) {}

    constexpr inline size_type size() const noexcept { return size_; }
    constexpr inline size_type count() const noexcept { return values_.size(); }

    _Tp get(size_type i) const {
      for (std::size_t j = 0; j < ind_.size(); ++j) {
        if (ind_[j] == i) {
          return values_[j];
        }
      }
      return _Tp();
    }

    std::string dump() const {
      std::stringstream ss;
      ss << '[';
      for (std::size_t i = 0; i < size_ - 1; ++i) {
        ss << '[' << get(i) << ']' << ',';
      }
      ss << '[' << get(size_ - 1) << ']';
      ss << ']';
      return ss.str();
    }

    void set(size_type i, _Tp value) {
      for (std::size_t j = 0; j < ind_.size(); ++j) {
        if (ind_[j] == i) {
          values_[j] = value;
          return;
        }
      }
      if (value != _Tp()) {
        ind_.push_back(i);
        values_.push_back(value);
      }
    }

    Vector& operator=(const Vector& vec) {
      size_ = vec.size_;
      ind_ = vec.ind_;
      values_ = vec.values_;
      return *this;
    }

    reference operator()(size_type i) {
      for (std::size_t j = 0; j < ind_.size(); ++j) {
        if (ind_[j] == i) {
          return values_[j];
        }
      }
      ind_.push_back(i);
      values_.push_back(_Tp());
      return values_.back();
    }
    reference operator[](size_type i) {
      for (std::size_t j = 0; j < ind_.size(); ++j) {
        if (ind_[j] == i) {
          return values_[j];
        }
      }
      ind_.push_back(i);
      values_.push_back(_Tp());
      return values_.back();
    }

    template <typename _T>
    inline fem::math::Vector<_Tp>& operator+=(const _T& rhs) {
      *this = *this + rhs;
      return *this;
    }
    template <typename _T>
    inline fem::math::Vector<_Tp>& operator-=(const _T& rhs) {
      *this = *this - rhs;
      return *this;
    }
    template <typename _T>
    inline fem::math::Vector<_Tp>& operator*=(const _T& rhs) {
      *this = *this * rhs;
      return *this;
    }
    template <typename _T>
    inline fem::math::Vector<_Tp>& operator/=(const _T& rhs) {
      *this = *this / rhs;
      return *this;
    }

   private:
    std::size_t size_;
    std::vector<std::size_t> ind_;
    std::vector<_Tp> values_;
  };

  template <typename _Tp = double, typename _Al = std::allocator<_Tp>>
  class Matrix {
   public:
    typedef _Al allocator_type;
    typedef typename _Al::value_type value_type;
    typedef typename std::vector<_Tp>::reference reference;
    typedef typename std::vector<_Tp>::const_reference const_reference;
    typedef typename _Al::difference_type difference_type;
    typedef typename _Al::size_type size_type;
    typedef typename _Al::pointer pointer;
    typedef typename _Al::const_pointer const_pointer;

    explicit Matrix(std::size_t _N)
        : size_(_N), row_ptr_(std::vector<std::size_t>(_N + 1, 0)) {}
    Matrix(const Matrix& copy)
        : size_(copy.size_),
          row_ptr_(copy.row_ptr_),
          col_ind_(copy.col_ind_),
          values_(copy.values_) {}

    constexpr inline size_type size() const noexcept { return size_; }
    constexpr inline size_type count() const noexcept {
      return row_ptr_.back();
    }
    constexpr inline size_type count2() const noexcept {
      return values_.size();
    }

    _Tp get(size_type r, size_type c) const {
      if (row_ptr_[r + 1] - row_ptr_[r] == 0) {
        return _Tp();
      } else {
        for (std::size_t i = row_ptr_[r]; i < row_ptr_[r + 1]; ++i) {
          if (col_ind_[i] == c) {
            return values_[i];
          }
        }
        return _Tp();
      }
    }

    std::string dump() const {
      std::stringstream ss;
      ss << '[';
      for (std::size_t r = 0; r < size_ - 1; ++r) {
        ss << '[';
        for (std::size_t c = 0; c < size_ - 1; ++c) {
          ss << get(r, c) << ',';
        }
        ss << get(r, size_ - 1);
        ss << ']' << ',';
      }
      ss << '[';
      for (std::size_t c = 0; c < size_ - 1; ++c) {
        ss << get(size_ - 1, c) << ',';
      }
      ss << get(size_ - 1, size_ - 1);
      ss << ']';
      ss << ']';
      return ss.str();
    }

    // XXX: Setting existing value to zero fails
    void set(size_type r, size_type c, _Tp value) {
      if (value == _Tp()) {
        return;
      }
      if (row_ptr_[r + 1] - row_ptr_[r] == 0) {
        values_.insert(values_.begin() + row_ptr_[r + 1], value);
        col_ind_.insert(col_ind_.begin() + row_ptr_[r], c);
        for (std::size_t sr = r + 1; sr < size_ + 1; ++sr) {
          row_ptr_[sr]++;
        }
        return;
      } else {
        for (std::size_t i = row_ptr_[r]; i < row_ptr_[r + 1]; ++i) {
          if (col_ind_[i] == c) {
            values_[i] = value;
            return;
          }
        }
        values_.insert(values_.begin() + row_ptr_[r + 1], value);
        col_ind_.insert(col_ind_.begin() + row_ptr_[r + 1], c);
        for (std::size_t sr = r + 1; sr < size_ + 1; ++sr) {
          row_ptr_[sr]++;
        }
        return;
      }
    }

    Matrix& operator=(const Matrix& mat) {
      size_ = mat.size_;
      row_ptr_ = mat.row_ptr_;
      col_ind_ = mat.col_ind_;
      values_ = mat.values_;
      return *this;
    }

    reference operator()(size_type r, size_type c) {
      if (row_ptr_[r + 1] - row_ptr_[r] == 0) {
        values_.insert(values_.begin() + row_ptr_[r + 1], _Tp());
        col_ind_.insert(col_ind_.begin() + row_ptr_[r], c);
        for (std::size_t sr = r + 1; sr < size_ + 1; ++sr) {
          row_ptr_[sr]++;
        }
        return values_[row_ptr_[r]];
      } else {
        for (std::size_t i = row_ptr_[r]; i < row_ptr_[r + 1]; ++i) {
          if (col_ind_[i] == c) {
            return values_[i];
          }
        }
        values_.insert(values_.begin() + row_ptr_[r + 1], _Tp());
        col_ind_.insert(col_ind_.begin() + row_ptr_[r + 1], c);
        for (std::size_t sr = r + 1; sr < size_ + 1; ++sr) {
          row_ptr_[sr]++;
        }
        return values_[row_ptr_[r + 1] - 1];
      }
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
    std::vector<std::size_t> row_ptr_;
    std::vector<std::size_t> col_ind_;
    std::vector<_Tp> values_;
  };

  template <typename _Tp>
  std::ostream& operator<<(std::ostream& out, const Vector<_Tp>& rhs) {
    out << "\u23a1" << std::fixed;
    out << std::setw(10) << rhs.get(0) << "\u23a4\n";
    for (std::size_t i = 1; i < rhs.size() - 1; ++i) {
      out << "\u23a2" << std::setw(10) << rhs.get(i) << "\u23a5\n";
    }
    out << "\u23a3" << std::setw(10) << rhs.get(rhs.size() - 1) << "\u23a6\n";
    return out;
  }

  template <typename _Tp>
  inline Vector<_Tp> operator+(const Vector<_Tp>& lhs, const _Tp& rhs) {
    Vector<_Tp> vec(lhs.size());
    for (std::size_t i = 0; i < lhs.size(); ++i) {
      vec.set(i, lhs.get(i) + rhs);
    }
    return vec;
  }
  template <typename _Tp>
  inline Vector<_Tp> operator+(const Vector<_Tp>& lhs, const Vector<_Tp>& rhs) {
    Vector<_Tp> vec(std::min(lhs.size(), rhs.size()));
    for (std::size_t i = 0; i < vec.size(); ++i) {
      vec.set(i, lhs.get(i) + rhs.get(i));
    }
    return vec;
  }
  template <typename _Tp>
  inline Vector<_Tp> operator-(const Vector<_Tp>& lhs, const _Tp& rhs) {
    Vector<_Tp> vec(lhs.size());
    for (std::size_t i = 0; i < lhs.size(); ++i) {
      vec.set(i, lhs.get(i) - rhs);
    }
    return vec;
  }
  template <typename _Tp>
  inline Vector<_Tp> operator-(const Vector<_Tp>& lhs, const Vector<_Tp>& rhs) {
    Vector<_Tp> vec(std::min(lhs.size(), rhs.size()));
    for (std::size_t i = 0; i < vec.size(); ++i) {
      vec.set(i, lhs.get(i) - rhs.get(i));
    }
    return vec;
  }
  template <typename _Tp>
  inline Vector<_Tp> operator*(const Vector<_Tp>& lhs, const _Tp& rhs) {
    Vector<_Tp> vec(lhs.size());
    for (std::size_t i = 0; i < lhs.size(); ++i) {
      vec.set(i, lhs.get(i) * rhs);
    }
    return vec;
  }
  template <typename _Tp>
  inline Vector<_Tp> operator*(const Vector<_Tp>& lhs, const Vector<_Tp>& rhs) {
    Vector<_Tp> vec(std::min(lhs.size(), rhs.size()));
    for (std::size_t i = 0; i < vec.size(); ++i) {
      vec.set(i, lhs.get(i) * rhs.get(i));
    }
    return vec;
  }
  template <typename _Tp>
  inline Vector<_Tp> operator/(const Vector<_Tp>& lhs, const _Tp& rhs) {
    Vector<_Tp> vec(lhs.size());
    for (std::size_t i = 0; i < lhs.size(); ++i) {
      vec.set(i, lhs.get(i) / rhs);
    }
    return vec;
  }
  template <typename _Tp>
  inline Vector<_Tp> operator/(const Vector<_Tp>& lhs, const Vector<_Tp>& rhs) {
    Vector<_Tp> vec(std::min(lhs.size(), rhs.size()));
    for (std::size_t i = 0; i < vec.size(); ++i) {
      vec.set(i, lhs.get(i) / rhs.get(i));
    }
    return vec;
  }

  template <typename _Tp>
  std::ostream& operator<<(std::ostream& out, const Matrix<_Tp>& rhs) {
    out << "\u23a1" << std::fixed;
    for (std::size_t j = 0; j < rhs.size() - 1; ++j) {
      out << std::setw(10) << rhs.get(0, j) << ' ';
    }
    out << std::setw(10) << rhs.get(0, rhs.size() - 1) << "\u23a4\n";
    for (std::size_t i = 1; i < rhs.size() - 1; ++i) {
      out << "\u23a2";
      for (std::size_t j = 0; j < rhs.size() - 1; ++j) {
        out << std::setw(10) << rhs.get(i, j) << ' ';
      }
      out << std::setw(10) << rhs.get(i, rhs.size() - 1) << "\u23a5\n";
    }
    out << "\u23a3";
    for (std::size_t j = 0; j < rhs.size() - 1; ++j) {
      out << std::setw(10) << rhs.get(rhs.size() - 1, j) << ' ';
    }
    out << std::setw(10) << rhs.get(rhs.size() - 1, rhs.size() - 1)
        << "\u23a6\n";
    return out;
  }

  template <typename _Tp>
  inline Matrix<_Tp> operator+(const Matrix<_Tp>& lhs, const _Tp& rhs) {
    Matrix<_Tp> mat(lhs.size());
    for (std::size_t row = 0; row < lhs.size(); ++row) {
      for (std::size_t col = 0; col < lhs.size(); ++col) {
        mat.set(row, col, lhs.get(row, col) + rhs);
      }
    }
    return mat;
  }
  template <typename _Tp>
  inline Matrix<_Tp> operator+(const Matrix<_Tp>& lhs, const Matrix<_Tp>& rhs) {
    Matrix<_Tp> mat(std::min(lhs.size(), rhs.size()));
    for (std::size_t row = 0; row < std::min(lhs.size(), rhs.size()); ++row) {
      for (std::size_t col = 0; col < std::min(lhs.size(), rhs.size()); ++col) {
        mat.set(row, col, lhs.get(row, col) + rhs.get(row, col));
      }
    }
    return mat;
  }
  template <typename _Tp>
  inline Matrix<_Tp> operator-(const Matrix<_Tp>& lhs, const _Tp& rhs) {
    Matrix<_Tp> mat(lhs.size());
    for (std::size_t row = 0; row < lhs.size(); ++row) {
      for (std::size_t col = 0; col < lhs.size(); ++col) {
        mat.set(row, col, lhs.get(row, col) - rhs);
      }
    }
    return mat;
  }
  template <typename _Tp>
  inline Matrix<_Tp> operator-(const Matrix<_Tp>& lhs, const Matrix<_Tp>& rhs) {
    Matrix<_Tp> mat(std::min(lhs.size(), rhs.size()));
    for (std::size_t row = 0; row < std::min(lhs.size(), rhs.size()); ++row) {
      for (std::size_t col = 0; col < std::min(lhs.size(), rhs.size()); ++col) {
        mat.set(row, col, lhs.get(row, col) - rhs.get(row, col));
      }
    }
    return mat;
  }
  template <typename _Tp>
  inline Matrix<_Tp> operator*(const Matrix<_Tp>& lhs, const _Tp& rhs) {
    Matrix<_Tp> mat(lhs.size());
    for (std::size_t row = 0; row < lhs.size(); ++row) {
      for (std::size_t col = 0; col < lhs.size(); ++col) {
        mat.set(row, col, lhs.get(row, col) * rhs);
      }
    }
    return mat;
  }
  // TODO Implement faster algorithm for matrix multiplication
  template <typename _Tp>
  inline Matrix<_Tp> operator*(const Matrix<_Tp>& lhs, const Matrix<_Tp>& rhs) {
    Matrix<_Tp> mat(std::min(lhs.size(), rhs.size()));
    for (std::size_t r = 0; r < mat.size(); ++r) {
      for (std::size_t c = 0; c < mat.size(); ++c) {
        _Tp sum = _Tp();
        for (std::size_t k = 0; k < mat.size(); ++k) {
          sum += (lhs.get(r, k) * rhs.get(k, c));
        }
        mat.set(r, c, sum);
      }
    }
    return mat;
  }
  template <typename _Tp>
  inline Matrix<_Tp> operator/(const Matrix<_Tp>& lhs, const _Tp& rhs) {
    Matrix<_Tp> mat(lhs.size());
    for (std::size_t row = 0; row < lhs.size(); ++row) {
      for (std::size_t col = 0; col < lhs.size(); ++col) {
        mat.set(row, col, lhs.get(row, col) / rhs);
      }
    }
    return mat;
  }
  template <typename _Tp>
  inline Matrix<_Tp> operator/(const Matrix<_Tp>& lhs, const Matrix<_Tp>& rhs) {
    Matrix<_Tp> mat(std::min(lhs.size(), rhs.size()));
    for (std::size_t row = 0; row < std::min(lhs.size(), rhs.size()); ++row) {
      for (std::size_t col = 0; col < std::min(lhs.size(), rhs.size()); ++col) {
        mat.set(row, col, lhs.get(row, col) / rhs.get(row, col));
      }
    }
    return mat;
  }

  template <typename _Tp>
  inline Vector<_Tp> operator*(const Matrix<_Tp>& lhs, const Vector<_Tp>& rhs) {
    Vector<_Tp> vec(lhs.size());
    for (std::size_t r = 0; r < lhs.size(); ++r) {
      _Tp sum = _Tp();
      for (std::size_t c = 0; c < rhs.size() && c < lhs.size(); ++c) {
        sum += (lhs.get(r, c) * rhs.get(c));
      }
      vec.set(r, sum);
    }
    return vec;
  }
}  // namespace math
}  // namespace fem

#endif  // FEM_MATH_SPARSE_HPP_
