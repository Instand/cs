#ifndef CS_UNIQUE_PTR
#define CS_UNIQUE_PTR

#include <utility>

#include <cs/utils/default_delete.hpp>

namespace cs {
template <typename T, typename Deleter = cs::DefaultDelete<T>>
class UniquePtr {
public:
    using ElementType = std::remove_all_extents_t<T>;

    UniquePtr() = default;

    template <typename Type>
    explicit UniquePtr(Type* ptr);
    ~UniquePtr();

    UniquePtr(const UniquePtr&) = delete;
    UniquePtr& operator=(const UniquePtr&) = delete;

    UniquePtr(UniquePtr&& ptr);
    UniquePtr& operator=(UniquePtr&& ptr);

    void swap(UniquePtr& ptr);

private:
    ElementType* value_ = nullptr;
};

template <typename T, typename Deleter> template <typename Type>
UniquePtr<T, Deleter>::UniquePtr(Type* ptr):value_(ptr) {}

template <typename T, typename Deleter>
UniquePtr<T, Deleter>::~UniquePtr() {
    Deleter deleter;
    deleter(value_);
}

template <typename T, typename Deleter>
UniquePtr<T, Deleter>::UniquePtr(UniquePtr&& ptr): value_(ptr.value_) {
    ptr.value_ = nullptr;
}

template <typename T, typename Deleter>
UniquePtr<T, Deleter>& UniquePtr<T, Deleter>::operator=(UniquePtr&& ptr) {
    UniquePtr<T>(std::move(ptr)).swap(*this);
    return *this;
}

template <typename T, typename Deleter>
void UniquePtr<T, Deleter>::swap(UniquePtr& ptr) {
    std::swap(value_, ptr.value_);
}
}

#endif // CS_UNIQUE_PTR
