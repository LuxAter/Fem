#ifndef FEM_UTIL_RAND_HPP_
#define FEM_UTIL_RAND_HPP_

#include <random>
#include <type_traits>

namespace fem {
namespace util {
  template <typename _T>
  typename std::enable_if<std::is_arithmetic<_T>::value, _T>::type Rand(
      _T minimum = 0, _T maximum = 1) {
    std::random_device rand_dev;
    std::mt19937 generator(rand_dev());
    std::uniform_real_distribution<long double> dist(
        static_cast<long double>(minimum), static_cast<long double>(maximum));
    return static_cast<_T>(dist(generator));
  }
}  // namespace util
}  // namespace fem

#endif  // FEM_UTIL_RAND_HPP_
