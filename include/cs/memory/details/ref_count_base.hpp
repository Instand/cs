#ifndef REF_COUNT_BASE_HPP
#define REF_COUNT_BASE_HPP

#include <atomic>

namespace cs {
class RefCountBase {
public:
    RefCountBase();
    virtual ~RefCountBase() {}

    virtual void deleteElement() noexcept = 0;
    virtual void deleteThis() noexcept = 0;

    void increment();
    void decrement();

    int useCount() const;

private:
    std::atomic<int> count_ = { 0 };
};

RefCountBase::RefCountBase() {
    increment();
}

void RefCountBase::increment() {
    count_.fetch_add(1);
}

void RefCountBase::decrement() {
    if (count_.fetch_sub(1) == 0) {
        deleteElement();
        deleteThis();
    }
}

int RefCountBase::useCount() const {
    return count_.load();
}
}

#endif // REF_COUNT_BASE_HPP
