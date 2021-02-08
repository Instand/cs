#ifndef CS_SHARED_PTR_HPP
#define CS_SHARED_PTR_HPP

#include <type_traits>

#include <cs/memory/details/bad_weak_ptr.hpp>
#include <cs/memory/details/ptr_base.hpp>
#include <cs/memory/details/ref_count.hpp>
#include <cs/memory/details/ref_count_source.hpp>

namespace cs {
using BadWeakPtrException = details::BadWeakPtrException;

// represents smart pointer with reference count
template <typename T>
class SharedPtr : public details::PtrBase<T> {
    using SharedPtrBase = details::PtrBase<T>;

public:
    using ElementType = typename SharedPtrBase::ElementType;

    SharedPtr() noexcept = default;

    template <typename Type>
    explicit SharedPtr(Type* value);

    template<typename Type, typename Deleter>
    explicit SharedPtr(Type* value, Deleter deleter);
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
    explicit SharedPtr(const WeakPtr<Type>& ptr);

    template <typename Type>
    SharedPtr& operator=(const SharedPtr<Type>& ptr);

    template <typename Type>
    SharedPtr& operator=(SharedPtr<Type>&& ptr);

    // returns element ptr
    ElementType* get() const;
    ElementType* operator->() const;
    operator bool() const;

    void reset(ElementType* value = nullptr);

    // swaps with other shared pointer
    void swap(SharedPtr& ptr);
};

template <typename T> template <typename Type>
SharedPtr<T>::SharedPtr(Type* value) {
    this->ptr_ = value;

    if constexpr (std::is_array_v<T>) {
        auto deleter = [](ElementType* ptr) { delete[] ptr; };
        this->refCountBase_ = new details::RefCountSource<ElementType, decltype(deleter)>(value, deleter);
    }
    else {
        this->refCountBase_ = new details::RefCount(value);
    }
}

template <typename T> template <typename Type, typename Deleter>
SharedPtr<T>::SharedPtr(Type* value, Deleter deleter) {
    this->ptr_ = value;
    this->refCountBase_ = new details::RefCountSource<T, decltype(deleter)>(value, deleter);
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
SharedPtr<T>::SharedPtr(const WeakPtr<Type>& ptr) {
    if (!this->constructFromWeak(ptr)) {
        throw BadWeakPtrException();
    }
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
SharedPtr<T>::operator bool() const {
    return static_cast<bool>(this->ptr_);
}

template <typename T>
void SharedPtr<T>::reset(ElementType* value) {
    SharedPtr<T> ptr;

    if (value != nullptr) {
        ptr = SharedPtr<T>(value);
    }

    ptr.swap(*this);
}

template <typename T>
void SharedPtr<T>::swap(SharedPtr& ptr) {
    SharedPtrBase::swap(ptr);
}
}

#endif // CS_SHARED_PTR_HPP
