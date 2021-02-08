#ifndef CS_UNIQUE_PTR
#define CS_UNIQUE_PTR

#include <cs/utils/default_delete.hpp>
#include <cs/utils/compressed_pair.hpp>

namespace cs {
template <typename T, typename Deleter = cs::DefaultDelete<T>>
class UniquePtr {
public:
    using ElementType = std::remove_all_extents_t<T>;

    UniquePtr();

    template <typename Type>
    explicit UniquePtr(Type* ptr);

    template <typename Type, typename UserDeleter>
    explicit UniquePtr(Type* ptr, UserDeleter&& deleter);

    ~UniquePtr();

    UniquePtr(const UniquePtr&) = delete;
    UniquePtr& operator=(const UniquePtr&) = delete;

    UniquePtr(UniquePtr&& ptr);
    UniquePtr& operator=(UniquePtr&& ptr);

    void swap(UniquePtr& ptr);
    void reset(ElementType* value = nullptr);
    ElementType* release();

    operator bool() const;

private:
    CompressedPair<Deleter, ElementType*> compressedPair_;
};

template <typename T, typename Deleter>
UniquePtr<T, Deleter>::UniquePtr(): compressedPair_(details::SecondArgTag{}, nullptr) {}

template <typename T, typename Deleter> template <typename Type>
UniquePtr<T, Deleter>::UniquePtr(Type* ptr): compressedPair_(details::SecondArgTag{}, ptr) {}

template <typename T, typename Deleter> template <typename Type, typename UserDeleter>
UniquePtr<T, Deleter>::UniquePtr(Type* ptr, UserDeleter&& deleter):
    compressedPair_(details::BothArgsTag{}, std::forward<UserDeleter>(deleter), ptr) {}

template <typename T, typename Deleter>
UniquePtr<T, Deleter>::~UniquePtr() {
    compressedPair_.first()(compressedPair_.second());
}

template <typename T, typename Deleter>
UniquePtr<T, Deleter>::UniquePtr(UniquePtr&& ptr): compressedPair_(std::move(ptr.compressedPair_)) {
    ptr.compressedPair_.second() = nullptr;
}

template <typename T, typename Deleter>
UniquePtr<T, Deleter>& UniquePtr<T, Deleter>::operator=(UniquePtr&& ptr) {
    UniquePtr<T>(std::move(ptr)).swap(*this);
    return *this;
}

template <typename T, typename Deleter>
void UniquePtr<T, Deleter>::swap(UniquePtr& ptr) {
    std::swap(compressedPair_, ptr.compressedPair_);
}

template <typename T, typename Deleter>
void UniquePtr<T, Deleter>::reset(ElementType* value) {
    cs::UniquePtr<T, Deleter> ptr(value);
    swap(ptr);
}

template <typename T, typename Deleter>
typename UniquePtr<T, Deleter>::ElementType* UniquePtr<T, Deleter>::release() {
    auto value = compressedPair_.second();

    compressedPair_.first() = {};
    compressedPair_.second() = nullptr;

    return value;
}

template <typename T, typename Deleter>
UniquePtr<T, Deleter>::operator bool() const {
    return static_cast<bool>(compressedPair_.second());
}
}

#endif // CS_UNIQUE_PTR
