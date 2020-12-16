#define TESTING

#include <gtest/gtest.h>

#include <cs/memory/shared_ptr.hpp>

TEST(SharedPtr, DefaultConstruct) {
    static bool called = false;

    class A {
    public:
        ~A() {
            called = true;
        }
    };

    {
        cs::SharedPtr<A> ptr(new A());
        ASSERT_EQ(ptr.useCount(), 1);
    }

    ASSERT_TRUE(called);
}
