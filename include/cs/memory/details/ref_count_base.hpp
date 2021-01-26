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

    void incrementWeak();
    void decrementWeak();

    int weakCount() const;

    bool incrementNotZero();

private:
    std::atomic<int> count_ = { 1 };
    std::atomic<int> weakCount_ = { 1 };
};

inline void RefCountBase::increment() {
    count_.fetch_add(1);
}

inline void RefCountBase::decrement() {
    count_.fetch_sub(1);

    if (useCount() == 0) {
        deleteElement();
        decrementWeak();
    }
}

inline int RefCountBase::useCount() const {
    return count_.load();
}

inline void RefCountBase::incrementWeak() {
    weakCount_.fetch_add(1);
}

inline void RefCountBase::decrementWeak() {
    weakCount_.fetch_sub(1);

    if (weakCount() == 0) {
        deleteThis();
    }
}

inline int RefCountBase::weakCount() const {
    return weakCount_.load();
}

inline bool RefCountBase::incrementNotZero() {
    auto current = count_.load(std::memory_order_seq_cst);

    if (current == 0) {
        return false;
    }

    while (!count_.compare_exchange_weak(current, current + 1,
                                         std::memory_order_release,
                                         std::memory_order_relaxed));
    return true;
}

}

#endif // CS_REF_COUNT_BASE_HPP
