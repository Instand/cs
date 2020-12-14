#ifndef PTR_BASE_HPP
#define PTR_BASE_HPP

#include <cs/memory/details/ref_count_base.hpp>

namespace cs {
template <typename T>
class SharedPtr;

// represents base class for smart pointers with reference count
template <typename T>
class PtrBase {
private:
    T* ptr_ = nullptr;
    RefCountBase* refCountBase_ = nullptr;

    friend SharedPtr<T>;
};
}

#endif // PTR_BASE_HPP
