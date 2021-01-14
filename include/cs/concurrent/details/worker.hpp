#ifndef CS_WORKER_HPP
#define CS_WORKER_HPP

#include <thread>

#include <cs/concurrent/details/common.hpp>
#include <cs/concurrent/thread_pool.hpp>

namespace cs {
template <typename T>
class FutureWatcher;
class Concurrent;

namespace details {
template <typename T>
class FutureBase;

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
            runThread(std::forward<Func>(function));
        }
        else {
            runThreadPool(std::forward<Func>(function));
        }
    }

    template <typename T>
    friend class FutureBase;

    template <typename T>
    friend class cs::FutureWatcher;
    friend class cs::Concurrent;
};
} // details
}

#endif // CS_WORKER_HPP
