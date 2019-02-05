#ifndef FEM_UTIL_TRIPLE_HPP_
#define FEM_UTIL_TRIPLE_HPP_

namespace fem {
template <typename _T>
struct Triple {
  _T x, y, z;
  inline _T& operator[](const unsigned& i) {
    return (i == 0) ? x : (i == 1) ? y : z;
  }
  inline const _T& operator[](const unsigned& i) const {
    return (i == 0) ? x : (i == 1) ? y : z;
  }

  template <typename _U>
  inline Triple<_T>& operator+=(const Triple<_U>& rhs) {
    x += rhs.x;
    y += rhs.y;
    z += rhs.z;
    return *this;
  }
  template <typename _U>
  inline Triple<_T>& operator-=(const Triple<_U>& rhs) {
    x -= rhs.x;
    y -= rhs.y;
    z -= rhs.z;
    return *this;
  }
  template <typename _U>
  inline Triple<_T>& operator*=(const Triple<_U>& rhs) {
    x *= rhs.x;
    y *= rhs.y;
    z *= rhs.z;
    return *this;
  }
  template <typename _U>
  inline Triple<_T>& operator/=(const Triple<_U>& rhs) {
    x /= rhs.x;
    y /= rhs.y;
    z /= rhs.z;
    return *this;
  }

  template <typename _U>
  inline Triple<_T>& operator+=(const _U& rhs) {
    x += rhs;
    y += rhs;
    z += rhs;
    return *this;
  }
  template <typename _U>
  inline Triple<_T>& operator-=(const _U& rhs) {
    x -= rhs;
    y -= rhs;
    z -= rhs;
    return *this;
  }
  template <typename _U>
  inline Triple<_T>& operator*=(const _U& rhs) {
    x *= rhs;
    y *= rhs;
    z *= rhs;
    return *this;
  }
  template <typename _U>
  inline Triple<_T>& operator/=(const _U& rhs) {
    x /= rhs;
    y /= rhs;
    z /= rhs;
    return *this;
  }
};

template <typename _T, typename _U>
inline bool operator==(const Triple<_T>& lhs, const Triple<_U>& rhs) {
  return (lhs.x == rhs.x) && (lhs.y == rhs.y) && (lhs.z == rhs.z);
}
template <typename _T, typename _U>
inline bool operator!=(const Triple<_T>& lhs, const Triple<_U>& rhs) {
  return !(lhs == rhs);
}
template <typename _T, typename _U>
inline bool equal(const Triple<_T>& lhs, const Triple<_U>& rhs) {
  return (lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z) ||
         (lhs.x == rhs.y && lhs.y == rhs.z && lhs.z == rhs.x) ||
         (lhs.x == rhs.z && lhs.y == rhs.x && lhs.z == rhs.y);
}

template <typename _T, typename _U>
inline Triple<_T> operator+(const Triple<_T>& lhs, const Triple<_U>& rhs) {
  return {lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z};
}
template <typename _T, typename _U>
inline Triple<_T> operator-(const Triple<_T>& lhs, const Triple<_U>& rhs) {
  return {lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z};
}
template <typename _T, typename _U>
inline Triple<_T> operator*(const Triple<_T>& lhs, const Triple<_U>& rhs) {
  return {lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z};
}
template <typename _T, typename _U>
inline Triple<_T> operator/(const Triple<_T>& lhs, const Triple<_U>& rhs) {
  return {lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z};
}

template <typename _T, typename _U>
inline Triple<_T> operator+(const Triple<_T>& lhs, const _U& rhs) {
  return {lhs.x + rhs, lhs.y + rhs, lhs.z + rhs};
}
template <typename _T, typename _U>
inline Triple<_T> operator-(const Triple<_T>& lhs, const _U& rhs) {
  return {lhs.x - rhs, lhs.y - rhs, lhs.z - rhs};
}
template <typename _T, typename _U>
inline Triple<_T> operator*(const Triple<_T>& lhs, const _U& rhs) {
  return {lhs.x * rhs, lhs.y * rhs, lhs.z * rhs};
}
template <typename _T, typename _U>
inline Triple<_T> operator/(const Triple<_T>& lhs, const _U& rhs) {
  return {lhs.x / rhs, lhs.y / rhs, lhs.z / rhs};
}

typedef Triple<double> Trio;
typedef Triple<double> Pt3;
typedef Triple<unsigned long> Tri;

}  // namespace fem

#endif  // FEM_UTIL_TRIPLE_HPP_
