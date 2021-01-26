#ifndef CS_DESTRUCTOR_COUNTER_HPP
#define CS_DESTRUCTOR_COUNTER_HPP

namespace cs::testing {
class DestructorCounter {
public:
    explicit DestructorCounter(int& value):count_(value) {};
    ~DestructorCounter() { ++count_; }

    int count() const { return count_; }

private:
    int& count_;
};
}

#endif
