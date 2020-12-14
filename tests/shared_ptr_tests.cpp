#define TESTING

#include <gtest/gtest.h>

#include <cs/memory/shared_ptr.hpp>

TEST(SharedPtr, DefaultConstruct) {
    cs::SharedPtr<int> ptr(nullptr);
}
