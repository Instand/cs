#ifndef CS_THREAD_POOL_HPP
#define CS_THREAD_POOL_HPP

#include <vector>
#include <queue>
#include <future>
#include <mutex>
#include <functional>

namespace cs {
class ThreadPool {
public:
   explicit ThreadPool(size_t threadsCount);
    ThreadPool();
    ~ThreadPool();

    // adds new task to queue and returns future to get task result
    template<class F, class... Args>
    std::future<std::invoke_result_t<F, Args...>> enqueueFuture(F&& f, Args&&... args);

    // adds new task to queue without future result
    template<class F, class... Args>
    void enqueue(F&& f, Args&&... args);

    // checks running state
    bool isRunning() const;

    // get instace of thread pool
    // warn: be aware it creates static thread pool
    static ThreadPool& instance();

private:
    using Task = std::function<void()>;

    void enqueueImpl(Task&& task);

    // every thread loop
    void routine();

    std::queue<Task> tasks_;
    std::vector<std::thread> workers_;

    mutable std::mutex mutex_;
    std::condition_variable notifier_;

    bool stop_ = false;
};
 
inline ThreadPool::ThreadPool(size_t threads) {
    for (size_t i = 0; i < threads; ++i) {
        workers_.emplace_back(&ThreadPool::routine, this);
    }
}

inline ThreadPool::ThreadPool():ThreadPool(std::thread::hardware_concurrency()) {}

inline ThreadPool::~ThreadPool() {
    {
        std::lock_guard lock(mutex_);
        stop_ = true;
    }

    notifier_.notify_all();

    for (auto& worker : workers_) {
        worker.join();
    }
}

template<class F, class... Args>
inline std::future<std::invoke_result_t<F, Args...>> ThreadPool::enqueueFuture(F&& f, Args&&... args) {
    using ResultType = std::invoke_result_t<F, Args...>;

    const auto task = std::make_shared<std::packaged_task<ResultType()>>(std::bind(std::forward<F>(f), std::forward<Args>(args)...));
    auto res = task->get_future();

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
    std::lock_guard lock(mutex_);
    return stop_;
}

inline ThreadPool& ThreadPool::instance() {
    static ThreadPool pool;
    return pool;
}

inline void ThreadPool::enqueueImpl(ThreadPool::Task&& task) {
    {
        if (stop_) {
            throw std::runtime_error("enqueue on stopped ThreadPool");
        }

        std::lock_guard lock(mutex_);

        tasks_.emplace(std::move(task));
    }

    notifier_.notify_one();
}

inline void ThreadPool::routine() {
    for (;;) {
        Task task;

        {
            std::unique_lock lock(mutex_);

            notifier_.wait(lock, [pool = this] {
                return pool->stop_ || !pool->tasks_.empty();
            });

            if (stop_) {
                break;
            }

            if (!tasks_.empty()) {
                task = std::move(tasks_.front());
                tasks_.pop();
            }
        }

        task();
    }
}
}

#endif  // CS_THREAD_POOL_HPP
