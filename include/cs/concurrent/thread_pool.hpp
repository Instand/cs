#ifndef THREAD_POOL_HPP
#define THREAD_POOL_HPP

#include <vector>
#include <queue>
#include <future>
#include <mutex>
#include <functional>
#include <atomic>

namespace cs {
class ThreadPool {
public:
   explicit ThreadPool(size_t threadsCount);
    ThreadPool();
    ~ThreadPool();

    // adds new task to queue and returns future to get task result
    template<class F, class... Args>
    std::future<typename std::result_of<F(Args...)>::type> enqueueFuture(F&& f, Args&&... args);

    // adds new task to queue without future result
    template<class F, class... Args>
    void enqueue(F&& f, Args&&... args);

    // checks running state
    bool isRunning() const;

private:
    using Task = std::function<void()>;

    void enqueueImpl(Task&& task);

    std::queue<Task> tasks_;
    std::vector<std::thread> workers_;

    std::atomic<bool> stop_;

    std::mutex mutex_;
    std::condition_variable notifier_;
};
 
inline ThreadPool::ThreadPool(size_t threads):
    stop_(false)
{
    const auto routine = [pool = this] {
        for (;;) {
            Task task;

            {
                std::unique_lock lock(pool->mutex_);

                pool->notifier_.wait(lock, [pool] {
                    return pool->stop_.load(std::memory_order_acquire) || !pool->tasks_.empty();
                });

                if (pool->stop_.load(std::memory_order_acquire) && pool->tasks_.empty()) {
                    return;
                }

                task = std::move(pool->tasks_.front());
                pool->tasks_.pop();
            }

            task();
        }
    };

    for (size_t i = 0; i < threads; ++i) {
        workers_.emplace_back(routine);
    }
}

inline ThreadPool::ThreadPool():ThreadPool(std::thread::hardware_concurrency()) {}

inline ThreadPool::~ThreadPool() {
    stop_.store(true, std::memory_order_release);

    notifier_.notify_all();

    for (auto& worker : workers_) {
        worker.join();
    }
}

template<class F, class... Args>
inline std::future<typename std::result_of<F(Args...)>::type> ThreadPool::enqueueFuture(F&& f, Args&&... args) {
    using ResultType = typename std::result_of<F(Args...)>::type;

    const auto task = std::make_shared<std::packaged_task<ResultType()>>(std::bind(std::forward<F>(f), std::forward<Args>(args)...));
    const auto res = task->get_future();

    enqueueImpl([task] {
        (*task)();
    });

    return res;
}

template<class F, class... Args>
inline void ThreadPool::enqueue(F&& f, Args&&... args) {
    enqueueImpl(std::bind(std::forward<F>(f), std::forward<Args>(args)...));
}

inline bool ThreadPool::isRunning() const {
    return stop_.load(std::memory_order_acquire);
}

inline void ThreadPool::enqueueImpl(ThreadPool::Task&& task) {
    {
        // don't allow enqueueing after stopping the pool
        if (stop_.load(std::memory_order_acquire)) {
            throw std::runtime_error("enqueue on stopped ThreadPool");
        }

        std::unique_lock lock(mutex_);

        tasks_.emplace(std::move(task));
    }

    notifier_.notify_one();
}
}

#endif  // THREAD_POOL_HPP
