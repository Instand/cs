#define TESTING

#include <gtest/gtest.h>

#include <cs/utils/waiter.hpp>
#include <cs/concurrent/concurrent.hpp>

static void concurrentBaseUsage(cs::RunPolicy policy) {
    struct A {
        std::atomic<bool> called = { false };
    };

    A object;

    {
        auto watcher = cs::Concurrent::run(policy, [] {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        });

        cs::Connector::connect(&watcher->finished,[&] {
            object.called = true;
        });
    }

    cs::Waiter::wait([&] { return !object.called; }, 2000);

    ASSERT_TRUE(object.called);
}

TEST(Concurrent, RunBaseUsageThreadPolicy) {
    concurrentBaseUsage(cs::RunPolicy::Thread);
}

TEST(Concurrent, RunBaseUsageThreadPoolPolicy) {
    concurrentBaseUsage(cs::RunPolicy::ThreadPool);
}
