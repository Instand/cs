#define TESTING

#include <gtest/gtest.h>

#include <destructor_counter.hpp>

#include <cs/memory/weak_ptr.hpp>
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

TEST(SharedPtr, ConstructFromWeakPtrBad) {
    bool exceptionCatched = false;

    {
        try {
            cs::WeakPtr<int> weakPtr;
            cs::SharedPtr<int> sharedPtr(weakPtr);
        }
        catch (const cs::BadWeakPtrException&) {
            exceptionCatched = true;
        }
    }

    ASSERT_TRUE(exceptionCatched);
}

TEST(SharedPtr, ConstructFromWeakPtrOk) {
    bool exceptionCatched = false;

    {
        try {
            cs::SharedPtr<int> sharedPtr(new int{});
            cs::WeakPtr<int> weakPtr(sharedPtr);

            cs::SharedPtr<int> sharedPtrOk(weakPtr);

            ASSERT_EQ(weakPtr.expired(), false);
            ASSERT_EQ(weakPtr.useCount(), 2);
            ASSERT_EQ(weakPtr.weakCount(), 2);
        }
        catch (const cs::BadWeakPtrException&) {
            exceptionCatched = true;
        }
    }

    ASSERT_FALSE(exceptionCatched);
}

TEST(SharedPtr, DefaultArray) {
    auto calledCount = 0;

    {
        const auto array = new DestructorCounter[3] {
                DestructorCounter(calledCount),
                DestructorCounter(calledCount),
                DestructorCounter(calledCount)
        };

        cs::SharedPtr<DestructorCounter[]> sharedPtr1(array);

        {
            const auto sharedPtr2 = sharedPtr1;
            ASSERT_EQ(sharedPtr2.useCount(), 2);
        }

        ASSERT_EQ(sharedPtr1.useCount(), 1);
        ASSERT_EQ(calledCount, 0);
    }

    ASSERT_EQ(calledCount, 3);
}

TEST(SharedPtr, CustomDeleter) {
    auto calledCount = 0;

    {
        cs::SharedPtr<DestructorCounter> sharedPtr(new DestructorCounter(calledCount), [](auto ptr) { delete ptr; });

        ASSERT_EQ(sharedPtr.useCount(), 1);
        ASSERT_EQ(sharedPtr.weakCount(), 1);
        ASSERT_EQ(calledCount, 0);
    }

    ASSERT_EQ(calledCount, 1);
}

TEST(SharedPtr, Reset) {
    cs::SharedPtr<int> ptr1(new int(1));

    {
        cs::SharedPtr<int> ptr2 = ptr1;
        ptr2.reset(new int(2));

        ASSERT_EQ(*ptr2.get(), 2);
    }

    ASSERT_EQ(*ptr1.get(), 1);
}

TEST(SharedPtr, OperatorBool) {
    cs::SharedPtr<int> ptr1;

    {
        cs::SharedPtr<int> ptr2(new int{});
        ASSERT_TRUE(static_cast<bool>(ptr2));
    }

    ASSERT_FALSE(static_cast<bool>(ptr1));
}
