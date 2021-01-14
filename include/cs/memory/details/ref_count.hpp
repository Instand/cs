#ifndef CS_REF_COUNT_HPP
#define CS_REF_COUNT_HPP

#include <cs/memory/details/ref_count_base.hpp>

namespace cs::details {
template<typename T>
class RefCount : public RefCountBase {
public:
    explicit RefCount(T* ptr):RefCountBase(), ptr_(ptr) {}

    virtual void deleteElement() noexcept override;
    virtual void deleteThis() noexcept override;

private:
    T* ptr_ = nullptr;
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

#endif // CS_REF_COUNT_HPP
