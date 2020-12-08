#ifndef CONCURRENT_COMMON_HPP
#define CONCURRENT_COMMON_HPP

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
}

#endif // CONCURRENT_COMMON_HPP
