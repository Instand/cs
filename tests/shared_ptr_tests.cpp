#define TESTING

#include <gtest/gtest.h>

#include <destructor_counter.hpp>
#include <cs/memory/shared_ptr.hpp>

using namespace cs::testing;

TEST(SharedPtr, DefaultConstruct) {
    auto calledCount = 0;

    {
        cs::SharedPtr<DestructorCounter> ptr(new DestructorCounter(calledCount));
        ASSERT_EQ(ptr.useCount(), 1);
    }

    ASSERT_EQ(calledCount, 1);
}

TEST(SharedPtr, CopyConstruct) {
    auto calledCount = 0;
    cs::SharedPtr<DestructorCounter> ptr1(new DestructorCounter(calledCount));

    {
        cs::SharedPtr<DestructorCounter> ptr2 = ptr1;
        ASSERT_EQ(ptr2.useCount(), 2);
    }

    ASSERT_EQ(ptr1.useCount(), 1);
    ASSERT_EQ(calledCount, 0);
}

TEST(SharedPtr, MoveConstruct) {
    auto calledCount = 0;
    cs::SharedPtr<DestructorCounter> ptr1(new DestructorCounter(calledCount));

    {
        cs::SharedPtr<DestructorCounter> ptr2(std::move(ptr1));
        ASSERT_EQ(ptr2.useCount(), 1);
    }

    ASSERT_EQ(ptr1.useCount(), 0); // NOLINT
    ASSERT_EQ(calledCount, 1);
}

TEST(SharedPtr, CopyOperator) {
    auto calledCount = 0;

    {
        cs::SharedPtr<DestructorCounter> ptr1(new DestructorCounter(calledCount));
        cs::SharedPtr<DestructorCounter> ptr2(new DestructorCounter(calledCount));

        ptr2 = ptr1;

        ASSERT_EQ(calledCount, 1);
        ASSERT_EQ(ptr2.get(), ptr1.get());
        ASSERT_EQ(ptr2.useCount(), 2);
        ASSERT_EQ(ptr1.useCount(), 2);
    }

    ASSERT_EQ(calledCount, 2);
}

TEST(SharedPtr, MoveOperator) {
    auto calledCount = 0;

    {
        cs::SharedPtr<DestructorCounter> ptr1(new DestructorCounter(calledCount));
        cs::SharedPtr<DestructorCounter> ptr2(new DestructorCounter(calledCount));

        ptr2 = std::move(ptr1);

        ASSERT_EQ(calledCount, 1);
        ASSERT_EQ(ptr1.useCount(), 0); // NOLINT
        ASSERT_EQ(ptr2.useCount(), 1);
    }

    ASSERT_EQ(calledCount, 2);
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
