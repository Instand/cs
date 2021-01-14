#ifndef CS_CONCURRENT_COMMON_HPP
#define CS_CONCURRENT_COMMON_HPP

#include <future>

namespace cs {
enum class RunPolicy : unsigned char {
    Thread,
    ThreadPool
};

enum class WatcherState : unsigned char {
    Idle,
    Running,
    Compeleted
};

// future entity
template <typename T>
using Future = std::future<T>;
}

#endif // CS_CONCURRENT_COMMON_HPP
