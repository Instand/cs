#ifndef CS_PTR_BASE_HPP
#define CS_PTR_BASE_HPP

#include <type_traits>

#include <cs/memory/details/ref_count_base.hpp>

namespace cs {
template <typename T>
class SharedPtr;

template <typename T>
class WeakPtr;

namespace details {
// represents base class for smart pointers with reference count
template <typename T>
class PtrBase {
protected:
    using ElementType = typename std::remove_extent<T>::type;

    PtrBase() = default;
    PtrBase(const PtrBase&) = delete;
    PtrBase& operator=(const PtrBase&) = delete;

public:
    int useCount() const;
    int weakCount() const;

protected:

    template <typename Type>
    void moveConstruct(PtrBase<Type>&& ptrBase);

    template <typename Type>
    void copyConstruct(const PtrBase<Type>& ptrBase);

    void increment() const;
    void decrement() const;
    void swap(PtrBase& ptr);

    void incrementWeak() const;
    void decrementWeak() const;

    template <typename Type>
    bool constructFromWeak(const PtrBase<Type>& ptrBase);

    template <typename Type>
    void constructWeak(const PtrBase<Type>& ptrBase);

private:
    ElementType* ptr_ = nullptr;
    RefCountBase* refCountBase_ = nullptr;

    friend SharedPtr<T>;
};

template <typename T>
int PtrBase<T>::useCount() const {
    return refCountBase_ ? refCountBase_->useCount() : 0;
}

template <typename T>
int PtrBase<T>::weakCount() const {
    return refCountBase_ ? refCountBase_->weakCount() : 0;
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

template <typename T>
void PtrBase<T>::incrementWeak() const {
    if (refCountBase_) {
        refCountBase_->incrementWeak();
    }
}

template <typename T>
void PtrBase<T>::decrementWeak() const {
    if (refCountBase_) {
        refCountBase_->decrementWeak();
    }
}

template <typename T> template <typename Type>
bool PtrBase<T>::constructFromWeak(const PtrBase<Type>& ptrBase) {
    if (ptrBase.refCountBase_ && ptrBase.refCountBase_->incremenNotZero()) {
        ptr_ = ptrBase.ptr_;
        refCountBase_ = ptrBase.refCountBase_;

        return true;
    }

    return false;
}

template <typename T> template <typename Type>
void PtrBase<T>::constructWeak(const PtrBase<Type>& ptrBase) {
    if (ptrBase.refCountBase_) {
        ptr_ = ptrBase.ptr_;
        refCountBase_ = ptrBase.refCountBase_;
        refCountBase_->incrementWeak();
    }
}

}
}

#endif // CS_PTR_BASE_HPP
