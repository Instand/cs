#ifndef CS_DESTRUCTOR_COUNTER_HPP
#define CS_DESTRUCTOR_COUNTER_HPP

namespace cs::testing {
class DestructorCounter {
public:
    explicit DestructorCounter() = default;
    ~DestructorCounter() { ++count_; }

    static int count() { return count_; }

private:
    inline static int count_ = 0;
};
}

#endif
