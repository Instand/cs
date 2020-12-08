#ifndef WORKER_HPP
#define WORKER_HPP

#include <thread>

#include <cs/concurrent/detail/common.hpp>
#include <cs/concurrent/thread_pool.hpp>

namespace cs {
template <typename T>
class FutureBase;

template <typename T>
class FutureWatcher;

// concurrent private helper
class Worker {
private:
    template <typename Func>
    static void runThreadPool(Func&& function) {
        auto& threadPool = ThreadPool::instance();
        threadPool.enqueue(std::forward<Func>(function));
    }

    template <typename Func>
    static void runThread(Func&& function) {
        try {
            std::thread thread(std::forward<Func>(function));
            thread.detach();
        }
        catch (...) {
            runThreadPool(std::forward<Func>(function));
        }
    }

    template <typename Func>
    static void execute(RunPolicy policy, Func&& function) {
        if (policy == RunPolicy::Thread) {
            runThread(std::forward(function));
        }
        else {
            runThreadPool(std::forward(function));
        }
    }

    template <typename T>
    friend class FutureBase;

    template <typename T>
    friend class FutureWatcher;
    friend class Concurrent;
};
}

#endif // WORKER_HPP
