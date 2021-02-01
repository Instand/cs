#define TESTING

#include <gtest/gtest.h>

#include <cs/containers/forward_list.hpp>

TEST(ForwardList, PushFront) {
    cs::ForwardList<int> forwardList;

    forwardList.pushFront(2);
    forwardList.pushFront(1);

    ASSERT_EQ(forwardList.front(), 1);
    ASSERT_EQ(forwardList.size(), 2);
}
