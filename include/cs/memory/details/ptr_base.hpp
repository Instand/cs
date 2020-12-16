#ifndef PTR_BASE_HPP
#define PTR_BASE_HPP

#include <type_traits>

#include <cs/memory/details/ref_count_base.hpp>

namespace cs {
template <typename T>
class SharedPtr;

// represents base class for smart pointers with reference count
template <typename T>
class PtrBase {
protected:
    using ElementType = std::remove_extent<T>;

    PtrBase() = default;
    PtrBase(const PtrBase&) = delete;
    PtrBase& operator=(const PtrBase&) = delete;

public:
    int useCount() const;

protected:

    template<typename Type>
    void moveConstruct(PtrBase<Type>&& ptrBase);

    template<typename Type>
    void copyConstruct(const PtrBase<Type>& ptrBase);

    void increment() const;
    void decrement() const;
    void swap(PtrBase& ptr);

private:
    T* ptr_ = nullptr;
    RefCountBase* refCountBase_ = nullptr;

    friend SharedPtr<T>;
};

template <typename T>
int PtrBase<T>::useCount() const {
    return refCountBase_ ? refCountBase_->useCount() : 0;
}

template <typename T> template<typename Type>
void PtrBase<T>::moveConstruct(PtrBase<Type>&& ptr) {
    ptr_ = ptr.ptr_;
    refCountBase_ = ptr.refCountBase_;

    ptr.ptr_ = nullptr;
    ptr.refCountBase_ = nullptr;
}

template <typename T> template <typename Type>
void PtrBase<T>::copyConstruct(const PtrBase<Type>& ptr) {
    ptr.increment();

    ptr_ = ptr.ptr_;
    refCountBase_ = ptr.refCountBase_;
}

template <typename T>
void PtrBase<T>::increment() const {
    if (refCountBase_) {
        refCountBase_->increment();
    }
}

template <typename T>
void PtrBase<T>::decrement() const {
    if (refCountBase_) {
        refCountBase_->decrement();
    }
}

template <typename T>
void PtrBase<T>::swap(PtrBase& ptr) {
    std::swap(ptr_, ptr.ptr_);
    std::swap(refCountBase_, ptr.refCountBase_);
}
}

#endif // PTR_BASE_HPP
