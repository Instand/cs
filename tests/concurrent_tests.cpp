#define TESTING

#include <gtest/gtest.h>

#include <cs/utils/waiter.hpp>
#include <cs/concurrent/concurrent.hpp>

struct A {
    std::atomic<bool> called = { false };
};

static void concurrentBaseUsage(cs::RunPolicy policy) {
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

static void concurrentBaseUsageWithResult(cs::RunPolicy policy) {
    A object;

    {
        auto watcher = cs::Concurrent::run(policy, [=] {
            return policy;
        });

        cs::Connector::connect(&watcher->finished, [&](const cs::RunPolicy arg) {
            object.called = true;
            ASSERT_EQ(policy, arg);
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

TEST(Concurrent, RunBaseUsageWithResultThreadPolicy) {
    concurrentBaseUsageWithResult(cs::RunPolicy::Thread);
}

TEST(Concurrent, RunBaseUsageWithResultThreadPoolPolicy) {
    concurrentBaseUsageWithResult(cs::RunPolicy::ThreadPool);
}
