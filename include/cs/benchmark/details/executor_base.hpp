#ifndef CS_EXECUTOR_BASE_HPP
#define CS_EXECUTOR_BASE_HPP

#include <chrono>
#include <memory>
#include <future>

#include <cs/logger/logger.hpp>
#include <cs/benchmark/details/timeout_exception.hpp>

namespace cs::details {
class ExecutorBase {
protected:
    using Duration = decltype(std::chrono::steady_clock::now() - std::chrono::steady_clock::now());

    void started() {
        cslog() << "\nStart benchmark";
    }

    template<typename T, typename Future>
    void finished(const T& difference, Future& future) {
        static const char* title = "Benchmark finished, time in";
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(difference);

        if (duration.count() != 0) {
            cslog() << title << " ms " << duration.count();
        }
        else {
            cslog() << title << " ns " << std::chrono::duration_cast<std::chrono::nanoseconds>(difference).count();
        }

        future = Future{};
    }

    template <typename T>
    void monitor(std::shared_ptr<T> future, std::chrono::seconds max) {
        auto now = std::chrono::steady_clock::now();
        std::chrono::milliseconds ms(0);

        for(;;) {
            static const std::chrono::milliseconds waitTime(10);
            std::future_status status = future->wait_for(waitTime);

            if (status == std::future_status::ready) {
                break;
            }
            else {
                auto point = std::chrono::steady_clock::now();
                ms += std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - now);
                now = point;

                if (ms >= max) {
                    ms = std::chrono::milliseconds(0);
                    throw cs::details::TimeOutException();
                }
            }
        }
    }
};
}

#endif // CS_EXECUTOR_BASE_HPP
