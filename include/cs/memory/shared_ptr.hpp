#ifndef SHARED_PTR_HPP
#define SHARED_PTR_HPP

#include <cs/memory/details/ptr_base.hpp>

namespace cs {
template<typename T>
class SharedPtr {
public:

private:
    T* data_;
    PtrBase<T>* controlBlock_;
};
}

#endif // SHARED_PTR_HPP
