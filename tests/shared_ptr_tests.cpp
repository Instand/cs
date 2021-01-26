#define TESTING

#include <gtest/gtest.h>

#include <destructor_counter.hpp>
#include <cs/memory/shared_ptr.hpp>

using namespace cs::testing;

TEST(SharedPtr, DefaultConstruct) {
    {
        cs::SharedPtr<DestructorCounter> ptr(new DestructorCounter());
        ASSERT_EQ(ptr.useCount(), 1);
    }

    ASSERT_EQ(DestructorCounter::count(), 1);
}

TEST(SharedPtr, CopyConstruct) {
    cs::SharedPtr<DestructorCounter> ptr1(new DestructorCounter());

    {
        cs::SharedPtr<DestructorCounter> ptr2 = ptr1;
        ASSERT_EQ(ptr2.useCount(), 2);
    }

    ASSERT_EQ(ptr1.useCount(), 1);
    ASSERT_EQ(DestructorCounter::count(), 0);
}

TEST(SharedPtr, MoveConstruct) {
    cs::SharedPtr<DestructorCounter> ptr1(new DestructorCounter());

    {
        cs::SharedPtr<DestructorCounter> ptr2(std::move(ptr1));
        ASSERT_EQ(ptr2.useCount(), 1);
    }

    ASSERT_EQ(ptr1.useCount(), 0); // NOLINT
    ASSERT_EQ(DestructorCounter::count(), 1);
}

TEST(SharedPtr, CopyOperator) {
    {
        cs::SharedPtr<DestructorCounter> ptr1(new DestructorCounter());
        cs::SharedPtr<DestructorCounter> ptr2(new DestructorCounter());

        ptr2 = ptr1;

        ASSERT_EQ(DestructorCounter::count(), 1);
        ASSERT_EQ(ptr2.get(), ptr1.get());
        ASSERT_EQ(ptr2.useCount(), 2);
        ASSERT_EQ(ptr1.useCount(), 2);
    }

    ASSERT_EQ(DestructorCounter::count(), 2);
}

TEST(SharedPtr, MoveOperator) {
    {
        cs::SharedPtr<DestructorCounter> ptr1(new DestructorCounter());
        cs::SharedPtr<DestructorCounter> ptr2(new DestructorCounter());

        ptr2 = std::move(ptr1);

        ASSERT_EQ(DestructorCounter::count(), 1);
        ASSERT_EQ(ptr1.useCount(), 0); // NOLINT
        ASSERT_EQ(ptr2.useCount(), 1);
    }

    ASSERT_EQ(DestructorCounter::count(), 2);
}

TEST(SharedPtr, GetMethod) {
    int* value = new int{10};

    cs::SharedPtr<int> ptr(value);

    ASSERT_EQ(ptr.get(), value);
}

TEST(SharedPtr, ArrowOperator) {
    static bool called = false;

    struct Foo {
        void bar() {
            called = true;
        }
    };

    cs::SharedPtr<Foo> ptr(new Foo());
    ptr->bar();

    ASSERT_TRUE(called);
}

TEST(SharedPtr, ConstructFromWeakPtr) {

}
