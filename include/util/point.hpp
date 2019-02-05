#ifndef FEM_UTIL_POINT_HPP_
#define FEM_UTIL_POINT_HPP_

#include <string>
#include <iostream>

namespace fem {
template <typename _T>
struct Point {
  _T x, y;

  inline _T& operator[](const unsigned& i) { return (i == 0) ? x : y; }
  inline const _T& operator[](const unsigned& i) const {
    return (i == 0) ? x : y;
  }

  std::string dump() const {
    return std::to_string(x) + " " + std::to_string(y) + " ";
  }

  template <typename _U>
  inline Point<_T>& operator+=(const Point<_U>& rhs) {
    x += rhs.x;
    y += rhs.y;
    return *this;
  }
  template <typename _U>
  inline Point<_T>& operator-=(const Point<_U>& rhs) {
    x -= rhs.x;
    y -= rhs.y;
    return *this;
  }
  template <typename _U>
  inline Point<_T>& operator*=(const Point<_U>& rhs) {
    x *= rhs.x;
    y *= rhs.y;
    return *this;
  }
  template <typename _U>
  inline Point<_T>& operator/=(const Point<_U>& rhs) {
    x /= rhs.x;
    y /= rhs.y;
    return *this;
  }

  template <typename _U>
  inline Point<_T>& operator+=(const _U& rhs) {
    x += rhs;
    y += rhs;
    return *this;
  }
  template <typename _U>
  inline Point<_T>& operator-=(const _U& rhs) {
    x -= rhs;
    y -= rhs;
    return *this;
  }
  template <typename _U>
  inline Point<_T>& operator*=(const _U& rhs) {
    x *= rhs;
    y *= rhs;
    return *this;
  }
  template <typename _U>
  inline Point<_T>& operator/=(const _U& rhs) {
    x /= rhs;
    y /= rhs;
    return *this;
  }
};

template <typename _T>
inline std::ostream& operator<<(std::ostream& out, const Point<_T>& rhs){
  out << '(' << rhs.dump() << ')';
  return out;
}

template <typename _T, typename _U>
inline bool operator==(const Point<_T>& lhs, const Point<_U>& rhs) {
  return (lhs.x == rhs.x) && (lhs.y == rhs.y);
}
template <typename _T, typename _U>
inline bool operator!=(const Point<_T>& lhs, const Point<_U>& rhs) {
  return !((lhs.x == rhs.x) && (lhs.y == rhs.y));
}

template <typename _T, typename _U>
inline bool equal(const Point<_T>& lhs, const Point<_U>& rhs) {
  return (lhs.x == rhs.x && lhs.y == rhs.y) ||
         (lhs.x == rhs.y && lhs.y == rhs.x);
}

template <typename _T, typename _U>
inline Point<_T> operator+(const Point<_T>& lhs, const Point<_U>& rhs) {
  return {lhs.x + rhs.x, lhs.y + rhs.y};
}
template <typename _T, typename _U>
inline Point<_T> operator-(const Point<_T>& lhs, const Point<_U>& rhs) {
  return {lhs.x - rhs.x, lhs.y - rhs.y};
}
template <typename _T, typename _U>
inline Point<_T> operator*(const Point<_T>& lhs, const Point<_U>& rhs) {
  return {lhs.x * rhs.x, lhs.y * rhs.y};
}
template <typename _T, typename _U>
inline Point<_T> operator/(const Point<_T>& lhs, const Point<_U>& rhs) {
  return {lhs.x / rhs.x, lhs.y / rhs.y};
}

template <typename _T, typename _U>
inline Point<_T> operator+(const Point<_T>& lhs, const _U& rhs) {
  return {lhs.x + rhs, lhs.y + rhs};
}
template <typename _T, typename _U>
inline Point<_T> operator-(const Point<_T>& lhs, const _U& rhs) {
  return {lhs.x - rhs, lhs.y - rhs};
}
template <typename _T, typename _U>
inline Point<_T> operator*(const Point<_T>& lhs, const _U& rhs) {
  return {lhs.x * rhs, lhs.y * rhs};
}
template <typename _T, typename _U>
inline Point<_T> operator/(const Point<_T>& lhs, const _U& rhs) {
  return {lhs.x / rhs, lhs.y / rhs};
}

typedef Point<double> Pt;
typedef Point<double> Pt2;
typedef Point<unsigned long> Edge;
}  // namespace fem

#endif  // FEM_UTIL_POINT_HPP_
