#define TESTING

#include <gtest/gtest.h>

#include <cs/memory/shared_ptr.hpp>

class A {
public:
    explicit A(int& count):count_(count) {}
    ~A() {
        ++count_;
    }

private:
    int& count_;
};

TEST(SharedPtr, DefaultConstruct) {
    static int calledCount = 0;

    {
        cs::SharedPtr<A> ptr(new A(calledCount));
        ASSERT_EQ(ptr.useCount(), 1);
    }

    ASSERT_EQ(calledCount, 1);
}

TEST(SharedPtr, CopyConstruct) {
    static int calledCount = 0;

    cs::SharedPtr<A> ptr1(new A(calledCount));

    {
        cs::SharedPtr<A> ptr2 = ptr1;
        ASSERT_EQ(ptr2.useCount(), 2);
    }

    ASSERT_EQ(ptr1.useCount(), 1);
    ASSERT_EQ(calledCount, 0);
}

TEST(SharedPtr, MoveConstruct) {
    static int calledCount = 0;

    cs::SharedPtr<A> ptr1(new A(calledCount));

    {
        cs::SharedPtr<A> ptr2(std::move(ptr1));
        ASSERT_EQ(ptr2.useCount(), 1);
    }

    ASSERT_EQ(ptr1.useCount(), 0); // NOLINT
    ASSERT_EQ(calledCount, 1);
}

TEST(SharedPtr, CopyOperator) {
    static int calledCount = 0;

    {
        cs::SharedPtr<A> ptr1(new A(calledCount));
        cs::SharedPtr<A> ptr2(new A(calledCount));

        ptr2 = ptr1;

        ASSERT_EQ(calledCount, 1);
        ASSERT_EQ(ptr2.get(), ptr1.get());
        ASSERT_EQ(ptr2.useCount(), 2);
        ASSERT_EQ(ptr1.useCount(), 2);
    }

    ASSERT_EQ(calledCount, 2);
}

TEST(SharedPtr, MoveOperator) {
    static int calledCount = 0;

    {
        cs::SharedPtr<A> ptr1(new A(calledCount));
        cs::SharedPtr<A> ptr2(new A(calledCount));

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
