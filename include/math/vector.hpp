#ifndef FEM_MATH_VECTOR_HPP_
#define FEM_MATH_VECTOR_HPP_

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <functional>
#include <initializer_list>
#include <iostream>
#include <string>

#include "../util/to_string.hpp"

namespace fem {
template <typename _T>
class Vector {
 public:
  Vector() : size_(0), data_(nullptr) {}
  explicit Vector(const unsigned long& n) : size_(n) {
    data_ = (_T*)std::malloc(size_ * sizeof(_T));
    std::fill(data_, data_ + size_, _T());
  }
  template <typename _U>
  Vector(const unsigned long& n, _U (*func)()) : size_(n) {
    data_ = (_T*)std::malloc(size_ * sizeof(_T));
    for (unsigned long i = 0; i < n; ++i) {
      data_[i] = static_cast<_T>(func());
    }
  }
  template <typename _U>
  Vector(const unsigned long& n, _U (*func)(const unsigned long&)) : size_(n) {
    data_ = (_T*)std::malloc(size_ * sizeof(_T));
    for (unsigned long i = 0; i < n; ++i) {
      data_[i] = static_cast<_T>(func(i));
    }
  }
  explicit Vector(const unsigned long& n, const _T& v) : size_(n) {
    data_ = (_T*)std::malloc(size_ * sizeof(_T));
    std::fill(data_, data_ + size_, v);
  }
  explicit Vector(const std::initializer_list<_T>& args) : size_(args.size()) {
    data_ = (_T*)std::malloc(size_ * sizeof(_T));
    std::copy(args.begin(), args.end(), data_);
  }
  Vector(const Vector<_T>& copy) : size_(copy.size_) {
    data_ = (_T*)std::malloc(size_ * sizeof(_T));
    std::copy(copy.begin(), copy.end(), data_);
  }
  ~Vector() { free(data_); }

  Vector<_T>& operator=(const std::initializer_list<_T>& args) {
    free(data_);
    size_ = args.size();
    data_ = (_T*)std::malloc(size_ * sizeof(_T));
    std::copy(args.begin(), args.end(), data_);
    return *this;
  }
  Vector<_T>& operator=(const Vector<_T>& copy) {
    free(data_);
    size_ = copy.size();
    data_ = (_T*)std::malloc(size_ * sizeof(_T));
    std::copy(copy.begin(), copy.end(), data_);
    return *this;
  }

  inline _T& operator[](const unsigned long& i) { return data_[i]; }
  inline const _T& operator[](const unsigned long& i) const { return data_[i]; }

  inline _T at(const unsigned long& i) const { return data_[i]; }
  inline void set(const unsigned long& i, const _T& v) { data_[i] = v; }

  inline unsigned long size() const noexcept { return size_; }
  std::string dump(unsigned perc = 5) const {
    std::string dmp;
    for (unsigned long i = 0; i < size_; ++i) {
      dmp += fem::to_string(data_[i], perc) + ' ';
    }
    return dmp;
  }

  inline void resize(const unsigned long& size) {
    free(data_);
    size_ = size;
    data_ = (_T*)std::malloc(size_ * sizeof(_T));
    std::fill(data_, data_ + size_, _T());
  }

  template <typename _U>
  inline typename std::enable_if<std::is_arithmetic<_U>::value>::type fill(
      const _U& v) {
    std::fill(data_, data_ + size_, static_cast<_T>(v));
  }
  template <typename _U>
  inline void fill(_U (*func)(const unsigned long&)) {
    for (unsigned int i = 0; i < size_; ++i) {
      data_[i] = static_cast<_T>(func(i));
    }
  }
  template <typename _U>
  inline void fill(_U (*func)()) {
    for (unsigned int i = 0; i < size_; ++i) {
      data_[i] = static_cast<_T>(func());
    }
  }
  inline void clear() { std::fill(data_, data_ + size_, _T()); }

  inline _T* begin() { return data_; }
  inline const _T* begin() const { return data_; }
  inline const _T* cbegin() const { return data_; }
  inline _T* end() { return data_ + size_; }
  inline const _T* end() const { return data_ + size_; }
  inline const _T* cend() const { return data_ + size_; }

  template <typename _U>
  inline Vector<_T>& operator+=(const Vector<_U>& rhs) {
    for (unsigned long i = 0; i < std::min(size_, rhs.size()); ++i) {
      data_[i] += rhs[i];
    }
    return *this;
  }
  template <typename _U>
  inline Vector<_T>& operator-=(const Vector<_U>& rhs) {
    for (unsigned long i = 0; i < std::min(size_, rhs.size()); ++i) {
      data_[i] -= rhs[i];
    }
    return *this;
  }
  template <typename _U>
  inline Vector<_T>& operator*=(const Vector<_U>& rhs) {
    for (unsigned long i = 0; i < std::min(size_, rhs.size()); ++i) {
      data_[i] *= rhs[i];
    }
    return *this;
  }
  template <typename _U>
  inline Vector<_T>& operator/=(const Vector<_U>& rhs) {
    for (unsigned long i = 0; i < std::min(size_, rhs.size()); ++i) {
      data_[i] /= rhs[i];
    }
    return *this;
  }

  template <typename _U>
  inline Vector<_T>& operator+=(const _U& rhs) {
    for (unsigned long i = 0; i < size_; ++i) {
      data_[i] += rhs;
    }
    return *this;
  }
  template <typename _U>
  inline Vector<_T>& operator-=(const _U& rhs) {
    for (unsigned long i = 0; i < size_; ++i) {
      data_[i] -= rhs;
    }
    return *this;
  }
  template <typename _U>
  inline Vector<_T>& operator*=(const _U& rhs) {
    for (unsigned long i = 0; i < size_; ++i) {
      data_[i] *= rhs;
    }
    return *this;
  }
  template <typename _U>
  inline Vector<_T>& operator/=(const _U& rhs) {
    for (unsigned long i = 0; i < size_; ++i) {
      data_[i] /= rhs;
    }
    return *this;
  }

 private:
  unsigned long size_ = 0;
  _T* data_ = nullptr;
};

template <typename _T>
inline std::ostream& operator<<(std::ostream& out, const Vector<_T>& rhs) {
  out << '<' << rhs.dump() << '>';
  return out;
}

template <typename _T, typename _U>
inline bool operator==(const Vector<_T>& lhs, const Vector<_U>& rhs) {
  return std::equal(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}
template <typename _T, typename _U>
inline bool operator!=(const Vector<_T>& lhs, const Vector<_U>& rhs) {
  return !std::equal(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <typename _T, typename _U>
inline Vector<_T> operator+(const Vector<_T>& lhs, const Vector<_U>& rhs) {
  Vector<_T> res(std::min(lhs.size(), rhs.size()));
  for (unsigned long i = 0; i < res.size(); ++i) {
    res[i] = lhs[i] + rhs[i];
  }
  return res;
}
template <typename _T, typename _U>
inline Vector<_T> operator-(const Vector<_T>& lhs, const Vector<_U>& rhs) {
  Vector<_T> res(std::min(lhs.size(), rhs.size()));
  for (unsigned long i = 0; i < res.size(); ++i) {
    res[i] = lhs[i] - rhs[i];
  }
  return res;
}
template <typename _T, typename _U>
inline Vector<_T> operator*(const Vector<_T>& lhs, const Vector<_U>& rhs) {
  Vector<_T> res(std::min(lhs.size(), rhs.size()));
  for (unsigned long i = 0; i < res.size(); ++i) {
    res[i] = lhs[i] * rhs[i];
  }
  return res;
}
template <typename _T, typename _U>
inline Vector<_T> operator/(const Vector<_T>& lhs, const Vector<_U>& rhs) {
  Vector<_T> res(std::min(lhs.size(), rhs.size()));
  for (unsigned long i = 0; i < res.size(); ++i) {
    res[i] = lhs[i] / rhs[i];
  }
  return res;
}

template <typename _T, typename _U>
inline Vector<_T> operator+(const Vector<_T>& lhs, const _U& rhs) {
  Vector<_T> res(lhs);
  for (unsigned long i = 0; i < res.size(); ++i) {
    res[i] += rhs;
  }
  return res;
}
template <typename _T, typename _U>
inline Vector<_T> operator-(const Vector<_T>& lhs, const _U& rhs) {
  Vector<_T> res(lhs);
  for (unsigned long i = 0; i < res.size(); ++i) {
    res[i] -= rhs;
  }
  return res;
}
template <typename _T, typename _U>
inline Vector<_T> operator*(const Vector<_T>& lhs, const _U& rhs) {
  Vector<_T> res(lhs);
  for (unsigned long i = 0; i < res.size(); ++i) {
    res[i] *= rhs;
  }
  return res;
}
template <typename _T, typename _U>
inline Vector<_T> operator/(const Vector<_T>& lhs, const _U& rhs) {
  Vector<_T> res(lhs);
  for (unsigned long i = 0; i < res.size(); ++i) {
    res[i] /= rhs;
  }
  return res;
}

template <typename _T, typename _U>
_T Dot(const Vector<_T>& lhs, const Vector<_U>& rhs) {
  _T val = _T();
  for (unsigned long i = 0; i < lhs.size() && i < rhs.size(); ++i) {
    val += (lhs.at(i) * rhs.at(i));
  }
  return val;
}

template <typename _T>
_T Norm(const Vector<_T>& lhs) {
  _T val = _T();
  for (unsigned long i = 0; i < lhs.size(); ++i) {
    val += (lhs.at(i) * lhs.at(i));
  }
  return std::sqrt(val);
}

template <typename _T = double>
void SaveToFile(const std::string& file_name, const Vector<_T>& vec) {
  std::ofstream out(file_name.c_str());
  if (out.is_open()) {
    out << vec.size() << '\n' << vec.dump(16);
    out.close();
  }
}

template <typename _T = double>
Vector<_T> LoadVecFromFile(const std::string& file_name) {
  Vector<_T> res;
  std::ifstream src(file_name.c_str());
  if (src.is_open()) {
    unsigned long size;
    src >> size;
    res.resize(size);
    for (unsigned long i = 0; i < size; ++i) {
      _T val;
      src >> val;
      res[i] = val;
    }
    src.close();
  }
  return res;
}

typedef Vector<double> Vec;

}  // namespace fem

#endif  // FEM_MATH_VECTOR_HPP_
