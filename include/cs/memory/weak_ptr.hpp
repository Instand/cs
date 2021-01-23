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

    template <typename Type>
    WeakPtr(const SharedPtr<Type>& sharedPtr);
};

template <typename T>
WeakPtr<T>::~WeakPtr() {
    this->decrementWeak();
}

template <typename T>
WeakPtr<T>::WeakPtr(const WeakPtr& weakPtr) {
    this->constructWeak(weakPtr);
}

template <typename T> template <typename Type>
WeakPtr<T>::WeakPtr(const SharedPtr<Type>& sharedPtr) {
    this->constructWeak(sharedPtr);
}

}

#endif // CS_WEAK_PTR_HPP
