#include "timer.hpp"

#include <chrono>

static std::chrono::time_point<std::chrono::system_clock> start_time_;

std::chrono::time_point<std::chrono::system_clock> arta::time::now() {
  return std::chrono::system_clock::now();
}

void arta::time::start() { start_time_ = std::chrono::system_clock::now(); }

double arta::time::stop() {
  return std::chrono::duration<double>(std::chrono::system_clock::now() -
                                       start_time_)
      .count();
}
