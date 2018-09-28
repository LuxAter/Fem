#ifndef FEM_ASYNC_HPP_
#define FEM_ASYNC_HPP_

#include <future>
#include <vector>

namespace fem {
namespace async {
  template <typename _T>
  void Join(std::vector<std::future<_T>>& threads) {
    for (std::size_t i = 0; i < threads.size(); ++i) {
      // threads[i].get();
    }
  }
}  // namespace async
}  // namespace fem

#endif  // FEM_ASYNC_HPP_
