#ifndef REF_COUNT_HPP
#define REF_COUNT_HPP

#include <cs/memory/details/ref_count_base.hpp>

namespace cs {
template<typename T>
class RefCount : public RefCountBase {
public:
    explicit RefCount(T* ptr):RefCountBase(), ptr_(ptr) {}

    virtual void deleteElement() noexcept override;
    virtual void deleteThis() noexcept override;

private:
    T* ptr_;
};

template<typename T>
void RefCount<T>::deleteElement() noexcept {
    delete ptr_;
}

template<typename T>
void RefCount<T>::deleteThis() noexcept {
    delete this;
}
}

#endif // REF_COUNT_HPP
