#ifndef ARTA_HPP_
#define ARTA_HPP_

#define ARTA_VERSION_MAJOR 3
#define ARTA_VERSION_MINOR 4
#define ARTA_THREAD_MAX 8

#include "logger.hpp"
#include "script.hpp"
#include "argparse.hpp"
#include "pde.hpp"
#include "plot.hpp"

namespace arta {
  void get_version(int& major, int& minor);
} // namespace arta

#endif  // ARTA_HPP_
