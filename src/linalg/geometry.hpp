#ifndef ARTA_LINALG_GEOMETRY_HPP_
#define ARTA_LINALG_GEOMETRY_HPP_

#include <array>
#include <vector>

namespace arta {
namespace linalg {

  template <typename _T>
  struct Pair {
    Pair() : x(), y() {}
    template <typename _U>
    Pair(const std::vector<_U>& vec) : x(vec[0]), y(vec[1]) {}
    template <typename _U>
    Pair(const std::array<_U, 2>& arr) : x(arr[0]), y(arr[1]) {}
    _T x, y;
    _T& operator[](const unsigned short& i) { return (i == 0) ? x : y; }
    _T operator[](const unsigned short& i) const { return (i == 0) ? x : y; }
    operator std::vector<_T>() { return {x, y}; }
    operator std::array<_T, 2>() { return {{x, y}}; }
  };

  template <typename _T>
  struct Triple {
    Triple() : x(), y(), z() {}
    template <typename _U>
    Triple(const std::vector<_U>& vec) : x(vec[0]), y(vec[1]), z(vec[2]) {}
    template <typename _U>
    Triple(const std::array<_U, 3>& arr) : x(arr[0]), y(arr[1]), z(arr[2]) {}
    _T x, y, z;
    _T& operator[](const unsigned short& i) {
      return (i == 0) ? x : (i == 1) ? y : z;
    }
    _T operator[](const unsigned short& i) const {
      return (i == 0) ? x : (i == 1) ? y : z;
    }
    operator std::vector<_T>() { return {x, y, z}; }
    operator std::array<_T, 3>() { return {{x, y, z}}; }
  };

  template <typename _T, typename _U>
  bool operator==(const Pair<_T>& lhs, const Pair<_U>& rhs) {
    return lhs.x == rhs.x && lhs.y == rhs.y;
  }
  template <typename _T, typename _U>
  bool operator==(const Triple<_T>& lhs, const Triple<_U>& rhs) {
    return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z;
  }

}  // namespace linalg
}  // namespace arta

#endif  // ARTA_LINALG_GEOMETRY_HPP_
