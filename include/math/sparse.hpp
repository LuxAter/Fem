#ifndef FEM_MATH_SPARSE_HPP_
#define FEM_MATH_SPARSE_HPP_

#include <array>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <random>
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

    explicit Vector(std::size_t _N) : values_(std::vector<_Tp>(_N, _Tp())) {}
    Vector(std::size_t _N, _Tp val) : values_(std::vector<_Tp>(_N, val)) {}
    Vector(const Vector& copy) : values_(copy.values_) {}

    constexpr inline size_type size() const noexcept { return values_.size(); }
    constexpr inline size_type count() const noexcept { return values_.size(); }

    inline _Tp get(size_type i) const { return values_[i]; }

    std::string dump() const {
      std::stringstream ss;
      ss << '[';
      for (std::size_t i = 0; i < values_.size() - 1; ++i) {
        ss << '[' << get(i) << ']' << ',';
      }
      ss << '[' << get(values_.size() - 1) << ']';
      ss << ']';
      return ss.str();
    }

    void set(size_type i, _Tp value) { values_[i] = value; }

    Vector& operator=(const Vector& vec) {
      values_ = vec.values_;
      return *this;
    }

    reference operator()(size_type i) { return values_[i]; }
    _Tp operator()(size_type i) const { return values_[i]; }
    reference operator[](size_type i) { return values_[i]; }
    _Tp operator[](size_type i) const { return values_[i]; }

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
        : size_(_N),
          n_row_(_N),
          n_col_(_N),
          row_ptr_(std::vector<std::size_t>(_N + 1, 0)) {}
    explicit Matrix(std::size_t n_row, std::size_t n_col)
        : n_row_(n_row),
          n_col_(n_col),
          row_ptr_(std::vector<std::size_t>(n_row_ + 1, 0)) {}
    Matrix(const Matrix& copy)
        : size_(copy.size_),
          n_row_(copy.n_row_),
          n_col_(copy.n_col_),
          row_ptr_(copy.row_ptr_),
          col_ind_(copy.col_ind_),
          values_(copy.values_) {}

    constexpr inline size_type size() const noexcept { return n_row_ * n_col_; }
    constexpr inline size_type rows() const noexcept { return n_row_; }
    constexpr inline size_type cols() const noexcept { return n_col_; }
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

    Vector<_Tp> row(size_type r) const {
      Vector<_Tp> vec(size_);
      for (std::size_t i = 0; i < n_col_; ++i) {
        vec.set(i, get(r, i));
      }
      return vec;
    }

    Vector<_Tp> col(size_type c) const {
      Vector<_Tp> vec(size_);
      for (std::size_t i = 0; i < n_row_; ++i) {
        vec.set(i, get(i, c));
      }
      return vec;
    }

    std::string dump() const {
      std::stringstream ss;
      ss << '[';
      for (std::size_t r = 0; r < n_row_ - 1; ++r) {
        ss << '[';
        for (std::size_t c = 0; c < n_col_ - 1; ++c) {
          ss << get(r, c) << ',';
        }
        ss << get(r, n_col_ - 1);
        ss << ']' << ',';
      }
      ss << '[';
      for (std::size_t c = 0; c < n_col_ - 1; ++c) {
        ss << get(n_row_ - 1, c) << ',';
      }
      ss << get(n_row_ - 1, n_col_ - 1);
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
        for (std::size_t sr = r + 1; sr < n_row_ + 1; ++sr) {
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
        for (std::size_t sr = r + 1; sr < n_row_ + 1; ++sr) {
          row_ptr_[sr]++;
        }
        return;
      }
    }
    void set_row(size_type r, Vector<_Tp> vec) {
      for (std::size_t i = 0; i < std::min(vec.size(), n_col_); ++i) {
        set(r, i, vec.get(i));
      }
    }
    void set_col(size_type c, Vector<_Tp> vec) {
      for (std::size_t i = 0; i < std::min(vec.size(), n_row_); ++i) {
        set(i, c, vec.get(i));
      }
    }

    Matrix& operator=(const Matrix& mat) {
      size_ = mat.size_;
      n_row_ = mat.n_row_;
      n_col_ = mat.n_col_;
      row_ptr_ = mat.row_ptr_;
      col_ind_ = mat.col_ind_;
      values_ = mat.values_;
      return *this;
    }

    reference operator()(size_type r, size_type c) {
      if (row_ptr_[r + 1] - row_ptr_[r] == 0) {
        values_.insert(values_.begin() + row_ptr_[r + 1], _Tp());
        col_ind_.insert(col_ind_.begin() + row_ptr_[r], c);
        for (std::size_t sr = r + 1; sr < n_row_ + 1; ++sr) {
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
        for (std::size_t sr = r + 1; sr < n_row_ + 1; ++sr) {
          row_ptr_[sr]++;
        }
        return values_[row_ptr_[r + 1] - 1];
      }
    }
    _Tp operator()(size_type r, size_type c) const {
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

    std::size_t size_;
    std::size_t n_row_, n_col_;
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
    for (std::size_t j = 0; j < rhs.cols() - 1; ++j) {
      out << std::setw(10) << rhs.get(0, j) << ' ';
    }
    out << std::setw(10) << rhs.get(0, rhs.cols() - 1) << "\u23a4\n";
    for (std::size_t i = 1; i < rhs.rows() - 1; ++i) {
      out << "\u23a2";
      for (std::size_t j = 0; j < rhs.cols() - 1; ++j) {
        out << std::setw(10) << rhs.get(i, j) << ' ';
      }
      out << std::setw(10) << rhs.get(i, rhs.cols() - 1) << "\u23a5\n";
    }
    out << "\u23a3";
    for (std::size_t j = 0; j < rhs.cols() - 1; ++j) {
      out << std::setw(10) << rhs.get(rhs.rows() - 1, j) << ' ';
    }
    out << std::setw(10) << rhs.get(rhs.rows() - 1, rhs.cols() - 1)
        << "\u23a6\n";
    return out;
  }

  template <typename _Tp>
  inline Matrix<_Tp> operator+(const Matrix<_Tp>& lhs, const _Tp& rhs) {
    Matrix<_Tp> mat(lhs);
    for (auto& val : mat.values_) {
      val += rhs;
    }
    return mat;
  }
  template <typename _Tp>
  inline Matrix<_Tp> operator+(const Matrix<_Tp>& lhs, const Matrix<_Tp>& rhs) {
    Matrix<_Tp> mat(std::min(lhs.rows(), rhs.rows()),
                    std::min(lhs.cols(), rhs.cols()));
    for (std::size_t row = 0; row < mat.rows(); ++row) {
      for (std::size_t col = 0; col < mat.cols(); ++col) {
        mat.set(row, col, lhs.get(row, col) + rhs.get(row, col));
      }
    }
    return mat;
  }
  template <typename _Tp>
  inline Matrix<_Tp> operator-(const Matrix<_Tp>& lhs, const _Tp& rhs) {
    Matrix<_Tp> mat(lhs);
    for (auto& val : mat.values_) {
      val -= rhs;
    }
    return mat;
  }
  template <typename _Tp>
  inline Matrix<_Tp> operator-(const Matrix<_Tp>& lhs, const Matrix<_Tp>& rhs) {
    Matrix<_Tp> mat(std::min(lhs.rows(), rhs.rows()),
                    std::min(lhs.cols(), rhs.cols()));
    for (std::size_t row = 0; row < mat.rows(); ++row) {
      for (std::size_t col = 0; col < mat.cols(); ++col) {
        mat.set(row, col, lhs.get(row, col) - rhs.get(row, col));
      }
    }
    return mat;
  }
  template <typename _Tp>
  inline Matrix<_Tp> operator*(const Matrix<_Tp>& lhs, const _Tp& rhs) {
    Matrix<_Tp> mat(lhs);
    for (auto& val : mat.values_) {
      val *= rhs;
    }
    return mat;
  }
  template <typename _Tp>
  inline Matrix<_Tp> operator*(const Matrix<_Tp>& lhs, const Matrix<_Tp>& rhs) {
    Matrix<_Tp> mat(std::min(lhs.rows(), rhs.rows()),
                    std::min(lhs.cols(), rhs.cols()));
    for (std::size_t r = 0; r < mat.rows(); ++r) {
      for (std::size_t c = 0; c < mat.cols(); ++c) {
        _Tp sum = _Tp();
        for (std::size_t k = 0; k < std::min(mat.rows(), mat.cols()); ++k) {
          sum += (lhs.get(r, k) * rhs.get(k, c));
        }
        mat.set(r, c, sum);
      }
    }
    return mat;
  }
  template <typename _Tp>
  inline Matrix<_Tp> operator/(const Matrix<_Tp>& lhs, const _Tp& rhs) {
    Matrix<_Tp> mat(lhs);
    for (auto& val : mat.values_) {
      val /= rhs;
    }
    return mat;
  }
  template <typename _Tp>
  inline Matrix<_Tp> operator/(const Matrix<_Tp>& lhs, const Matrix<_Tp>& rhs) {
    Matrix<_Tp> mat(std::min(lhs.rows(), rhs.rows()),
                    std::min(lhs.cols(), rhs.cols()));
    for (std::size_t row = 0; row < mat.rows(); ++row) {
      for (std::size_t col = 0; col < mat.cols(); ++col) {
        mat.set(row, col, lhs.get(row, col) / rhs.get(row, col));
      }
    }
    return mat;
  }

  template <typename _Tp>
  inline Vector<_Tp> operator*(const Matrix<_Tp>& lhs, const Vector<_Tp>& rhs) {
    Vector<_Tp> vec(lhs.rows());
    for (std::size_t r = 0; r < lhs.rows(); ++r) {
      _Tp sum = _Tp();
      for (std::size_t c = 0; c < rhs.size() && c < lhs.cols(); ++c) {
        sum += (lhs.get(r, c) * rhs.get(c));
      }
      vec.set(r, sum);
    }
    return vec;
  }

  template <typename _Tp = double>
  Matrix<_Tp> Identity(std::size_t r, std::size_t c) {
    Matrix<_Tp> mat(r, c);
    for (std::size_t i = 0; i < r && i < c; ++i) {
      mat.set(i, i, 1.0);
    }
    return mat;
  }

  template <typename _Tp = double>
  Vector<_Tp> RandVector(std::size_t n, double sparcity, _Tp minimum = _Tp(),
                         _Tp maximum = _Tp(1)) {
    std::random_device rand_dev;
    std::mt19937 generator(rand_dev());
    std::uniform_real_distribution<long double> dist(
        static_cast<long double>(minimum), static_cast<long double>(maximum));
    std::uniform_real_distribution<long double> gen(
        static_cast<long double>(0), static_cast<long double>(1.0));
    Vector<_Tp> vec(n);
    for (std::size_t i = 0; i < n; ++i) {
      if (gen(generator) <= sparcity) {
        vec[i] = dist(generator);
      }
    }
    return vec;
  }
  template <typename _Tp = double>
  Matrix<_Tp> RandMatrix(std::size_t row, std::size_t col, double sparcity,
                         _Tp minimum = _Tp(), _Tp maximum = _Tp(1)) {
    std::random_device rand_dev;
    std::mt19937 generator(rand_dev());
    std::uniform_real_distribution<long double> dist(
        static_cast<long double>(minimum), static_cast<long double>(maximum));
    std::uniform_real_distribution<long double> gen(
        static_cast<long double>(0), static_cast<long double>(1.0));
    Matrix<_Tp> mat(row, col);
    for (std::size_t r = 0; r < row; ++r) {
      for (std::size_t c = 0; c < col; ++c) {
        if (gen(generator) <= sparcity) {
          mat.set(r, c, dist(generator));
        }
      }
    }
    return mat;
  }
  template <typename _Tp = double>
  Matrix<_Tp> RandSymMatrix(std::size_t row, std::size_t col, double sparcity,
                            _Tp minimum = _Tp(), _Tp maximum = _Tp(1)) {
    std::random_device rand_dev;
    std::mt19937 generator(rand_dev());
    std::uniform_real_distribution<long double> dist(
        static_cast<long double>(minimum), static_cast<long double>(maximum));
    std::uniform_real_distribution<long double> gen(
        static_cast<long double>(0), static_cast<long double>(1.0));
    Matrix<_Tp> mat = Identity<_Tp>(row, col);
    for (std::size_t r = 0; r < row; ++r) {
      for (std::size_t c = r; c < col; ++c) {
        if (gen(generator) <= sparcity) {
          mat.set(r, c, dist(generator));
          mat.set(c, r, mat.get(r, c));
        }
      }
    }
    return mat;
  }

  template <typename _Tp = double>
  Matrix<_Tp> RandPSDMatrix(std::size_t row, std::size_t col, double sparcity,
                            _Tp minimum = _Tp(), _Tp maximum = _Tp(1)) {
    Matrix<_Tp> mat = RandSymMatrix(row, col, sparcity, minimum, maximum);
    mat += (Identity<_Tp>(row, col) * _Tp(col * maximum));
    return mat;
  }

  template <typename _Tp>
  std::array<Matrix<_Tp>, 3> LowerDiagUpper(const Matrix<_Tp>& mat) {
    Matrix<_Tp> D(mat.rows(), mat.cols());
    Matrix<_Tp> L(mat.rows(), mat.cols());
    Matrix<_Tp> U(mat.rows(), mat.cols());
    for (std::size_t r = 0; r < mat.rows(); ++r) {
      if (mat.row_ptr_[r + 1] - mat.row_ptr_[r] == 0) {
        continue;
      }
      for (std::size_t i = mat.row_ptr_[r]; i < mat.row_ptr_[r + 1]; ++i) {
        if (r == mat.col_ind_[i]) {
          D.set(r, mat.col_ind_[i], mat.values_[i]);
        } else if (r > mat.col_ind_[i]) {
          L.set(r, mat.col_ind_[i], mat.values_[i]);
        } else {
          U.set(r, mat.col_ind_[i], mat.values_[i]);
        }
      }
    }
    return std::array<Matrix<_Tp>, 3>{{D, L, U}};
  }

  template <typename _Tp>
  Matrix<_Tp> Cholesky(const Matrix<_Tp>& A) {
    Matrix<_Tp> L(A.rows(), A.cols());
    for (uint64_t j = 0; j < L.cols(); ++j) {
      _Tp sum = _Tp();
      for (uint64_t k = 0; k < j; ++k) {
        sum += L.get(j, k) * L.get(j, k);
      }
      L(j, j) = std::sqrt(A(j, j) - sum);
      for (uint64_t i = j; i < L.rows(); ++i) {
        sum = _Tp();
        for (uint64_t k = 0; k < j; ++k) {
          sum += L.get(i, k) * L.get(j, k);
        }
        L(i, j) = (A(i, j) - sum) / L(j, j);
      }
    }
    return L;
  }

  template <typename _Tp>
  Matrix<_Tp> Transpose(const Matrix<_Tp>& mat) {
    Matrix<_Tp> tran(mat.cols(), mat.rows());
    for (std::size_t r = 0; r < mat.rows(); ++r) {
      if (mat.row_ptr_[r + 1] - mat.row_ptr_[r] == 0) {
        continue;
      }
      for (std::size_t i = mat.row_ptr_[r]; i < mat.row_ptr_[r + 1]; ++i) {
        tran.set(mat.col_ind_[i], r, mat.values_[i]);
      }
    }
    return tran;
  }
  template <typename _Tp>
  Matrix<_Tp> Transpose(const Vector<_Tp>& vec) {
    Matrix<_Tp> tran(1, vec.size());
    for (uint32_t i = 0; i < vec.size(); ++i) {
      tran.set(0, i, vec[i]);
    }
    return tran;
  }

  template <typename _Tp>
  _Tp Norm(const Vector<_Tp>& vec) {
    _Tp val = _Tp();
    for (std::size_t i = 0; i < vec.size(); ++i) {
      val += std::pow(vec.get(i), 2.0);
    }
    return std::sqrt(val);
  }

  template <typename _Tp>
  _Tp Dot(const Vector<_Tp>& lhs, const Vector<_Tp>& rhs) {
    _Tp val = _Tp();
    for (std::size_t i = 0; i < lhs.size() && i < rhs.size(); ++i) {
      val += (lhs.get(i) * rhs.get(i));
    }
    return val;
  }

  template <typename _Tp>
  _Tp Norm(const Matrix<_Tp>& mat) {
    _Tp val = _Tp();
    for (std::size_t i = 0; i < mat.size(); ++i) {
      for (std::size_t j = 0; j < mat.size(); ++j) {
        val += std::pow(mat.get(i, j), 2.0);
      }
    }
    return std::sqrt(val);
  }

  template <typename _Out = double, typename _In = int>
  Vector<_Out> Convert(const Vector<_In>& vec) {
    Vector<_Out> out(vec.size());
    for (uint32_t i = 0; i < vec.size(); ++i) {
      out[i] = _Out(vec[i]);
    }
    return out;
  }
  template <typename _Out = double, typename _In = int>
  Matrix<_Out> Convert(const Matrix<_In>& mat) {
    Matrix<_Out> out(mat.rows(), mat.cols());
    for (std::size_t r = 0; r < mat.rows(); ++r) {
      if (mat.row_ptr_[r + 1] - mat.row_ptr_[r] == 0) {
        continue;
      }
      for (std::size_t i = mat.row_ptr_[r]; i < mat.row_ptr_[r + 1]; ++i) {
        out.set(mat.col_ind_[i], r, _Out(mat.values_[i]));
      }
    }
    return out;
  }
}  // namespace math
}  // namespace fem

#endif  // FEM_MATH_SPARSE_HPP_
