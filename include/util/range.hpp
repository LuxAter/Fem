#ifndef FEM_UTIL_RANGE_HPP_
#define FEM_UTIL_RANGE_HPP_

#include <cmath>
#include <functional>
#include <type_traits>
#include <vector>

namespace fem {
namespace util {
  template <typename _T>
  typename std::enable_if<std::is_arithmetic<_T>::value, std::vector<_T>>::type
  Range(_T a, _T b, _T size) {
    std::vector<_T> values;
    for (_T i = a; i < b; i += size) {
      values.push_back(i);
    }
    return values;
  }
  template <typename _T>
  typename std::enable_if<std::is_arithmetic<_T>::value, std::vector<_T>>::type
  StepRange(_T a, _T b, std::size_t steps = 50) {
    _T size = std::fabs(a - b) / static_cast<_T>(steps);
    std::vector<_T> values;
    for (_T i = a; i < b; i += size) {
      values.push_back(i);
    }
    return values;
  }
  template <typename _T, std::size_t _N>
    typename std::enable_if<std::is_arithmetic<_T>::value, std::array<_T, _N>>::type
    StepRangeArray(_T a, _T b){
      _T size = std::fabs(a - b) / static_cast<_T>(_N);
      std::array<_T, _N> values;
      std::size_t index = 0;
      for(_T i = a; i < b; i += size, ++index){
        values[index] = i;
      }
      return values;
    }
  template<typename _T, std::size_t _N>
    typename std::enable_if<std::is_arithmetic<_T>::value, std::array<_T, _N>>::type
    FuncStepRangeArray(std::function<_T(_T)> func, _T a, _T b){
      _T size = std::fabs(a - b) / static_cast<_T>(_N);
      std::array<_T, _N> values;
      std::size_t index = 0;
      for(_T i = a; i < b; i += size, ++index){
        values[index] = func(i);
      }
      return values;
    }

  template<typename _T, std::size_t _N>
    typename std::enable_if<std::is_arithmetic<_T>::value, std::array<_T, _N>>::type
    FuncInArray(std::function<_T(_T)> func, std::array<_T, _N> x){
      std::array<_T, _N> values;
      for(std::size_t i = 0; i < _N; ++i){
        values[i] = func(x[i]);
      }
      return values;
    }
}  // namespace util
}  // namespace fem

#endif  // FEM_UTIL_RANGE_HPP_
