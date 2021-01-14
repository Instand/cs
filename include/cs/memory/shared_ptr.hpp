#ifndef SHARED_PTR_HPP
#define SHARED_PTR_HPP

#include <cs/memory/details/ptr_base.hpp>
#include <cs/memory/details/ref_count.hpp>

namespace cs {
// represents smart pointer with reference count
template <typename T>
class SharedPtr : public details::PtrBase<T> {
    using SharedPtrBase = details::PtrBase<T>;

public:
    using ElementType = typename SharedPtrBase::ElementType;

    SharedPtr() noexcept = default;
    explicit SharedPtr(T* value);
    ~SharedPtr();

    SharedPtr(const SharedPtr& ptr);
    SharedPtr(SharedPtr&& ptr);

    SharedPtr& operator=(const SharedPtr& ptr);
    SharedPtr& operator=(SharedPtr&& ptr);

    template <typename Type>
    SharedPtr(const SharedPtr<Type>& ptr);

    template <typename Type>
    SharedPtr(SharedPtr<Type>&& ptr);

    template <typename Type>
    SharedPtr& operator=(const SharedPtr<Type>& ptr);

    template <typename Type>
    SharedPtr& operator=(SharedPtr<Type>&& type);

    // returns element ptr
    ElementType* get() const;
    ElementType* operator->() const;

    // swaps with other shared pointer
    void swap(SharedPtr& ptr);
};

template <typename T>
SharedPtr<T>::SharedPtr(T* value) {
    this->ptr_ = value;
    this->refCountBase_ = new details::RefCount(value);
}

template <typename T>
SharedPtr<T>::~SharedPtr() {
    this->decrement();
}

template <typename T>
SharedPtr<T>::SharedPtr(const SharedPtr& ptr):SharedPtr() {
    this->copyConstruct(ptr);
}

template <typename T>
SharedPtr<T>::SharedPtr(SharedPtr&& ptr) {
    this->moveConstruct(std::move(ptr));
}

template <typename T>
SharedPtr<T>& SharedPtr<T>::operator=(const SharedPtr& ptr) {
    SharedPtr<T>(ptr).swap(*this);
    return *this;
}

template <typename T>
SharedPtr<T>& SharedPtr<T>::operator=(SharedPtr&& ptr) {
    SharedPtr<T>(std::move(ptr)).swap(*this);
    return *this;
}

template <typename T> template <typename Type>
SharedPtr<T>::SharedPtr(const SharedPtr<Type>& ptr) {
    this->copyConstruct(ptr);
}

template <typename T> template <typename Type>
SharedPtr<T>::SharedPtr(SharedPtr<Type>&& ptr) {
    this->moveConstruct(std::move(ptr));
}

template <typename T> template <typename Type>
SharedPtr<T>& SharedPtr<T>::operator=(const SharedPtr<Type>& ptr) {
    SharedPtr<T>(ptr).swap(*this);
    return *this;
}

template <typename T> template <typename Type>
SharedPtr<T>& SharedPtr<T>::operator=(SharedPtr<Type>&& ptr) {
    SharedPtr<T>(std::move(ptr)).swap(*this);
    return *this;
}

template <typename T>
typename SharedPtr<T>::ElementType* SharedPtr<T>::get() const {
    return this->ptr_;
}

template <typename T>
typename SharedPtr<T>::ElementType* SharedPtr<T>::operator->() const {
    return this->ptr_ ? this->ptr_ : nullptr;
}

template <typename T>
void SharedPtr<T>::swap(SharedPtr& ptr) {
    SharedPtrBase::swap(ptr);
}
}

#endif // SHARED_PTR_HPP
