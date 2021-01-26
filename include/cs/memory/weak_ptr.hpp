#ifndef CS_WEAK_PTR_HPP
#define CS_WEAK_PTR_HPP

#include <cs/memory/details/ptr_base.hpp>

namespace cs {
template <typename T>
class WeakPtr : public details::PtrBase<T> {
public:
    WeakPtr() = default;
    ~WeakPtr();

    WeakPtr(const WeakPtr& weakPtr);
    WeakPtr(WeakPtr&& weakPtr);

    template <typename Type>
    WeakPtr(const SharedPtr<Type>& sharedPtr);

    WeakPtr& operator=(const WeakPtr& weakPtr);
    WeakPtr& operator=(WeakPtr&& weakPtr);

    void swap(WeakPtr& weakPtr);

    bool expired() const;
    SharedPtr<T> lock() const;
};

template <typename T>
WeakPtr<T>::~WeakPtr() {
    this->decrementWeak();
}

template <typename T>
WeakPtr<T>::WeakPtr(const WeakPtr& weakPtr) {
    this->constructWeak(weakPtr);
}

template <typename T>
WeakPtr<T>::WeakPtr(WeakPtr&& weakPtr) {
    this->moveConstruct(std::move(weakPtr));
}

template <typename T> template <typename Type>
WeakPtr<T>::WeakPtr(const SharedPtr<Type>& sharedPtr) {
    this->constructWeak(sharedPtr);
}

template <typename T>
WeakPtr<T>& WeakPtr<T>::operator=(const WeakPtr& weakPtr) {
    WeakPtr<T>(weakPtr).swap(*this);
    return *this;
}

template <typename T>
WeakPtr<T>& WeakPtr<T>::operator=(WeakPtr&& weakPtr) {
    WeakPtr<T>(std::move(weakPtr)).swap(*this);
    return *this;
}

template <typename T>
void WeakPtr<T>::swap(WeakPtr& weakPtr) {
    this->swap(weakPtr);
}

template <typename T>
bool WeakPtr<T>::expired() const {
    return this->useCount() == 0;
}

template <typename T>
SharedPtr<T> WeakPtr<T>::lock() const {
    SharedPtr<T> sharedPtr;
    sharedPtr.constructFromWeak(*this);

    return sharedPtr;
}

}

#endif // CS_WEAK_PTR_HPP
