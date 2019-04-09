#ifndef ARTA_TIMER_HPP_
#define ARTA_TIMER_HPP_

#include <chrono>

namespace arta {
namespace time {
  typedef std::chrono::time_point<std::chrono::system_clock> time_t;

  std::chrono::time_point<std::chrono::system_clock> now();
  void start();
  double stop();
}  // namespace time
}  // namespace arta

#endif  // ARTA_TIMER_HPP_
