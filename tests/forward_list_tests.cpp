#define TESTING

#include <gtest/gtest.h>

#include <deque>
#include <cs/containers/forward_list.hpp>

TEST(ForwardList, PushFront) {
    cs::ForwardList<int> forwardList;

    forwardList.pushFront(2);
    forwardList.pushFront(1);

    ASSERT_EQ(forwardList.front(), 1);
    ASSERT_EQ(forwardList.size(), 2);
}

TEST(ForwardList, PopFront) {
    cs::ForwardList<std::string> forwardList;

    forwardList.pushFront("1");
    forwardList.pushFront("2");

    forwardList.popFront();

    ASSERT_EQ(forwardList.front(), "1");
}

TEST(ForwardList, Iteration) {
    cs::ForwardList<int> forwardList;
    std::deque<int> deque;

    for (auto i = 0; i < 10; ++i) {
        forwardList.pushFront(i);
        deque.push_front(i);
    }

    for (auto iter = std::begin(forwardList); iter != std::end(forwardList); ++iter) {
        auto index = std::size(forwardList) - std::distance(iter, forwardList.end());

        ASSERT_EQ(*iter, deque[index]);
    }
}

TEST(ForwardList, Reverse) {
    cs::ForwardList<int> forwardList;
    std::deque<int> deque;

    for (auto i = 0; i < 10; ++i) {
        forwardList.pushFront(i);
        deque.push_back(i);
    }

    forwardList.reverse();

    ASSERT_TRUE(std::equal(std::begin(forwardList), std::end(forwardList),
                           std::begin(deque), std::end(deque)));
}

TEST(ForwardList, Clear) {
    cs::ForwardList<int> forwardList;

    forwardList.pushFront(1);
    forwardList.pushFront(2);

    forwardList.clear();

    ASSERT_TRUE(forwardList.isEmpty());
}
