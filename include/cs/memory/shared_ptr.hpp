#ifndef SHARED_PTR_HPP
#define SHARED_PTR_HPP

#include <cs/memory/details/ptr_base.hpp>
#include <cs/memory/details/ref_count.hpp>

namespace cs {
// represents smart pointer with reference count
template <typename T>
class SharedPtr : public PtrBase<T> {
    using SharedPtrBase = PtrBase<T>;

public:
    explicit SharedPtr(T* value);
};

template <typename T>
SharedPtr<T>::SharedPtr(T* value) {
    this->ptr_ = value;
    this->refCountBase_ = new RefCount(value);
}
}

#endif // SHARED_PTR_HPP
