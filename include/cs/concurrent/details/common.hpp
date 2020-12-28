#ifndef CONCURRENT_COMMON_HPP
#define CONCURRENT_COMMON_HPP

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

#endif // CONCURRENT_COMMON_HPP
