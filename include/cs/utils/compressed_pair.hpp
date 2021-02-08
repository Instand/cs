#ifndef CS_COMPRESSED_PAIR_HPP
#define CS_COMPRESSED_PAIR_HPP

#include <utility>
#include <type_traits>

#include <cs/utils/details/utils_tags.hpp>

namespace cs {
template <typename Type1, typename Type2, bool = std::is_empty_v<Type1>>
class CompressedPair : private Type1 {
public:
    template <typename... Args2>
    explicit CompressedPair(details::SecondArgTag, Args2&&... args2):
        Type1(), second_(std::forward<Args2>(args2)...) {}

    template <typename Args1, typename... Args2>
    explicit CompressedPair(details::BothArgsTag, Args1&& args1, Args2&&... args2):
        Type1(std::forward<Args1>(args1)), second_(std::forward<Args2>(args2)...) {}

    Type1& first() { return *this; }
    const Type1& first() const { return *this; }

    Type2& second() { return second_; }
    const Type2& second() const { return second_; }

private:
    Type2 second_;
};

template <typename Type1, typename Type2>
class CompressedPair<Type1, Type2, false> {
public:
    template <typename... Args2>
    explicit CompressedPair(details::SecondArgTag, Args2&&... args2):
        first_(), second_(std::forward<Args2>(args2)...) {}

    template <typename Args1, typename... Args2>
    explicit CompressedPair(details::BothArgsTag, Args1&& args1, Args2&&... args2):
        first_(std::forward<Args1>(args1)), second_(std::forward<Args2>(args2)...) {}

    Type1& first() { return first_; }
    const Type1& first() const { return first_; }

    Type2& second() { return second_; }
    const Type2& second() const { return second_; }

private:
    Type1 first_;
    Type2 second_;
};
}

#endif // CS_COMPRESSED_PAIR_HPP
