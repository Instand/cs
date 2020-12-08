#ifndef CONCURRENT_HPP
#define CONCURRENT_HPP

#include <list>

#include <cs/concurrent/future_watcher.hpp>

namespace cs {
class Concurrent {
    template<typename T>
    using Executions = std::list<T>;

public:
    // runs function in another thread, returns future watcher
    // than generates finished signal by run policy
    // you should not store watcher, it does by run method, just use finished/failed signal to subscribe
    template <typename Func, typename... Args>
    static FutureWatcherPtr<std::invoke_result_t<std::decay_t<Func>, std::decay_t<Args>...>> run(RunPolicy policy, Func&& function, Args&&... args) {
        using ReturnType = std::invoke_result_t<std::decay_t<Func>, std::decay_t<Args>...>;
        using WatcherType = FutureWatcher<ReturnType>;

        // running executions
        static Executions<FutureWatcherPtr<ReturnType>> executions;
        using ExecutionsIterator = typename decltype(executions)::iterator;

        // run async and store it at executions
        auto watcher = std::make_shared<WatcherType>(policy, std::async(std::launch::async, std::forward<Func>(function), std::forward<Args>(args)...));

        {
            std::lock_guard lock(executionsMutex_);
            executions.push_back(watcher);
        }

        // watcher will be removed after lambda called
        cs::Connector::connect(&watcher->completed, [storage = watcher->shared_from_this()](typename FutureWatcher<ReturnType>::Id id) {
            ExecutionsIterator iter;

            {
                std::lock_guard lock(executionsMutex_);
                iter = std::find_if(executions.begin(), executions.end(), [=](const auto& watcher) {
                    return (watcher->id() == id) && (watcher->state() == WatcherState::Compeleted);
                });
            }

            if (iter != executions.end()) {
                std::lock_guard lock(executionsMutex_);
                executions.erase(iter);
            }
        });

        return watcher;
    }

    // calls std::function after ms time in another thread
    // its better to do not use thread pool run policy, it can block all cs runtime pools
    template<typename Func>
    static void runAfter(const std::chrono::milliseconds& ms, Func&& callBack, cs::RunPolicy policy = cs::RunPolicy::Thread) {
        const auto timePoint = std::chrono::steady_clock::now() + ms;
        auto binder = std::bind(&Concurrent::runAfterHelper<Func>, timePoint, std::forward(callBack));

        Worker::execute(policy, std::move(binder));
    }

    template <typename Func>
    static void execute(cs::RunPolicy policy, Func&& function) {
        Worker::execute(policy, std::forward<Func>(function));
    }

private:
    template<typename Func>
    static void runAfterHelper(const std::chrono::steady_clock::time_point& timePoint, Func&& callBack) {
        std::this_thread::sleep_until(timePoint);
        callBack();
    }

    inline static std::mutex executionsMutex_;
};
}

#endif // CONCURRRENT_HPP
