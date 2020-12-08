#ifndef WAITER_HPP
#define WAITER_HPP

#include <chrono>
#include <thread>

namespace cs {
// waits predicate or some time
class Waiter {
    template<typename Predicate>
    static void wait(Predicate&& predicate, const std::chrono::milliseconds ms) {
        const auto timePoint = std::chrono::steady_clock::now();

        while (predicate) {
            const auto now = std::chrono::steady_clock::now();
            const auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - timePoint);

            if (duration >= ms) {
                break;
            }

            std::this_thread::yield();
        }
    }
};
}

#endif // WAITER_HPP
