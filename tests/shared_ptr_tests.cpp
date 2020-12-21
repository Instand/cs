#define TESTING

#include <gtest/gtest.h>

#include <cs/memory/shared_ptr.hpp>

class A {
public:
    explicit A(bool& b):called_(b) {}
    ~A() {
        called_ = true;
    }

private:
    bool& called_;
};

TEST(SharedPtr, DefaultConstruct) {
    static bool called = false;

    {
        cs::SharedPtr<A> ptr(new A(called));
        ASSERT_EQ(ptr.useCount(), 1);
    }

    ASSERT_TRUE(called);
}

TEST(SharedPtr, CopyConstruct) {
    static bool called = false;

    cs::SharedPtr<A> ptr1(new A(called));

    {
        cs::SharedPtr<A> ptr2 = ptr1;
        ASSERT_EQ(ptr2.useCount(), 2);
    }

    ASSERT_EQ(ptr1.useCount(), 1);
    ASSERT_FALSE(called);
}

TEST(SharedPtr, MoveConstruct) {
    static bool called = false;

    cs::SharedPtr<A> ptr1(new A(called));

    {
        cs::SharedPtr<A> ptr2(std::move(ptr1));
        ASSERT_EQ(ptr2.useCount(), 1);
    }

    ASSERT_EQ(ptr1.useCount(), 0); // NOLINT
    ASSERT_TRUE(called);
}
