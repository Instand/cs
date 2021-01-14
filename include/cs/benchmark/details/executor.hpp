#ifndef CS_BENCHMARK_EXECUTOR_HPP
#define CS_BENCHMARK_EXECUTOR_HPP

#include <cs/benchmark/details/executor_base.hpp>

namespace cs::details {
template<typename T>
class Executor : public ExecutorBase {
    using Duration = typename ExecutorBase::Duration;
    using Future = std::future<std::pair<Duration, T>>;
    using FuturePtr = std::shared_ptr<Future>;

public:
    template <typename Func>
    T run(Func f, std::chrono::seconds max = std::chrono::seconds(10)) {
        ExecutorBase::started();

        auto lambda = [=] {
            auto now = std::chrono::steady_clock::now();
            auto result = f();

            auto difference = std::chrono::steady_clock::now() - now;
            return std::make_pair(difference, result);
        };

        future_ = std::make_shared<Future>(std::async(std::launch::async, lambda));
        ExecutorBase::monitor(future_, max);

        const auto [difference, result] = future_->get();
        ExecutorBase::finished(difference, future_);

        return result;
    }

private:
    FuturePtr future_;
};

template <>
class Executor<void> : public ExecutorBase {
    using Duration = typename ExecutorBase::Duration;
    using Future = std::future<Duration>;
    using FuturePtr = std::shared_ptr<Future>;

public:
    template <typename Func>
    void run(Func f, std::chrono::seconds max = std::chrono::seconds(10)) {
        static_assert (std::is_same_v<std::invoke_result_t<Func>, void>, "Benchmark<void> should use only functors with void result type");

        ExecutorBase::started();

        auto lambda = [=] {
            auto now = std::chrono::steady_clock::now();
            f();
            return std::chrono::steady_clock::now() - now;
        };

        future_ = std::make_shared<Future>(std::async(std::launch::async, lambda));
        ExecutorBase::monitor(future_, max);

        const auto difference = future_->get();
        ExecutorBase::finished(difference, future_);
    }

private:
    inline static FuturePtr future_;
};
}

#endif // CS_BENCHMARK_EXECUTOR_HPP
