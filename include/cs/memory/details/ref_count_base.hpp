#ifndef CS_REF_COUNT_BASE_HPP
#define CS_REF_COUNT_BASE_HPP

#include <atomic>

namespace cs::details {
class RefCountBase {
public:
    RefCountBase() = default;
    virtual ~RefCountBase() {}

    virtual void deleteElement() noexcept = 0;
    virtual void deleteThis() noexcept = 0;

    void increment();
    void decrement();

    int useCount() const;

private:
    std::atomic<int> count_ = { 1 };
};

void RefCountBase::increment() {
    count_.fetch_add(1);
}

void RefCountBase::decrement() {
    count_.fetch_sub(1);

    if (useCount() == 0) {
        deleteElement();
        deleteThis();
    }
}

int RefCountBase::useCount() const {
    return count_.load();
}
}

#endif // CS_REF_COUNT_BASE_HPP
