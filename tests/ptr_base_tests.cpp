#define TESTING

#include <gtest/gtest.h>

#include <cs/memory/details/ptr_base.hpp>

TEST(PtrBase, DefaultConstruct) {
    cs::PtrBase base;
}
