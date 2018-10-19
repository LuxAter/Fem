#ifndef FEM_UTIL_RANGE_HPP_
#define FEM_UTIL_RANGE_HPP_

#include <type_traits>
#include <vector>
#include <functional>
#include <cmath>

namespace fem {
  namespace util {
    template <typename _T>
      typename std::enable_if<std::is_arithmetic<_T>::value, std::vector<_T>>::type Range(_T a, _T b, _T size){
        std::vector<_T> values;
        for(_T i = a; i < b; i += size){
          values.push_back(i);
        }
        return values;
      }
    template <typename _T>
      typename std::enable_if<std::is_arithmetic<_T>::value, std::vector<_T>>::type StepRange(_T a, _T b, std::size_t steps=50){
        _T size = std::fabs(a - b) / static_cast<_T>(steps);
        std::vector<_T> values;
        for(_T i = a; i < b; i += size){
          values.push_back(i);
        }
        return values;
      }
  } // namespace util
} // namespace fem

#endif  // FEM_UTIL_RANGE_HPP_
