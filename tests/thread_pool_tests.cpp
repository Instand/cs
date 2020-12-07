#define TESTING

#include <gtest/gtest.h>

#include <cs/concurrent/thread_pool.hpp>

TEST(ThreadPool, BaseUsage) {
    static std::atomic<bool> called = { false };

    cs::ThreadPool pool;
    pool.enqueue([] {
        called.store(true, std::memory_order_release);
    });

    size_t i = 0;

    while (!called.load(std::memory_order_acquire) || i++ > 99) {
        std::this_thread::yield();
    }

    ASSERT_TRUE(called.load(std::memory_order_acquire));
}

TEST(ThreadPool, BaseUsageFuture) {
    static std::atomic<bool> called = { false };

    cs::ThreadPool pool;
    auto future = pool.enqueueFuture([] {
        called.store(true, std::memory_order_release);
    });

    future.get();

    ASSERT_TRUE(called.load(std::memory_order_acquire));
}
