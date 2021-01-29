#define TESTING

#include <gtest/gtest.h>

#include <destructor_counter.hpp>
#include <cs/memory/unique_ptr.hpp>

using namespace cs::testing;

TEST(UniquePtr, DefaultConstructor) {
    auto calledCount = 0;

    {
        cs::UniquePtr<DestructorCounter> ptr;
    }

    ASSERT_EQ(calledCount, 0);
}

TEST(UniquePtr, DefaultValueUsage) {
    auto calledCount = 0;

    {
        cs::UniquePtr<DestructorCounter> ptr(new DestructorCounter(calledCount));
    }

    ASSERT_EQ(calledCount, 1);
}

TEST(UniquePtr, MoveConstructor) {
    auto calledCount = 0;

    {
        cs::UniquePtr<DestructorCounter> ptr1(new DestructorCounter(calledCount));
        cs::UniquePtr<DestructorCounter> ptr2(std::move(ptr1));

        ASSERT_EQ(calledCount, 0);
    }

    ASSERT_EQ(calledCount, 1);
}

TEST(UniquePtr, MoveOperator) {
    auto calledCount = 0;

    {
        cs::UniquePtr<DestructorCounter> ptr1(new DestructorCounter(calledCount));
        cs::UniquePtr<DestructorCounter> ptr2(new DestructorCounter(calledCount));

        ptr1 = std::move(ptr2);

        ASSERT_EQ(calledCount, 1);
    }

    ASSERT_EQ(calledCount, 2);
}

TEST(UniquePtr, DefaultArrayUsage) {
    auto calledCount = 0;

    {
        auto array = new DestructorCounter[2] {
            DestructorCounter(calledCount),
            DestructorCounter(calledCount)
        };

        cs::UniquePtr<DestructorCounter[]> ptr(array);
    }

    ASSERT_EQ(calledCount, 2);
}
