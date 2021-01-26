#define TESTING

#include <gtest/gtest.h>

#include <destructor_counter.hpp>

#include <cs/memory/shared_ptr.hpp>
#include <cs/memory/weak_ptr.hpp>

using namespace cs::testing;

TEST(WeakPtr, DefaultConstruct) {
    cs::WeakPtr<DestructorCounter> ptr;

    ASSERT_EQ(ptr.useCount(), 0);
    ASSERT_EQ(ptr.weakCount(), 0);
    ASSERT_EQ(ptr.expired(), 0);
}

TEST(WeakPtr, ConstructFromSharedPtr) {
    {
        cs::SharedPtr<DestructorCounter> sharedPtr(new DestructorCounter());
        cs::WeakPtr<DestructorCounter> weakPtr(sharedPtr);

        ASSERT_EQ(weakPtr.useCount(), 1);
        ASSERT_EQ(weakPtr.weakCount(), 2);
    }

    ASSERT_EQ(DestructorCounter::count(), 1);
}
