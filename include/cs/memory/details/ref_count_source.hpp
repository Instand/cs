#ifndef CS_REF_COUNT_SOURCE
#define CS_REF_COUNT_SOURCE

#include <cs/memory/details/ref_count_base.hpp>

namespace cs::details {
template <typename T, typename Deleter>
class RefCountSource : public RefCountBase {
public:
    explicit RefCountSource(T* ptr, Deleter deleter):RefCountBase(), ptr_(ptr), deleter_(std::move(deleter)) {}

    virtual void deleteElement() noexcept override;
    virtual void deleteThis() noexcept override;

private:
    T* ptr_;
    Deleter deleter_;
};

template <typename T, typename Deleter>
void RefCountSource<T, Deleter>::deleteElement() noexcept {
    deleter_(ptr_);
}

template <typename T, typename Deleter>
void RefCountSource<T, Deleter>::deleteThis() noexcept {
    delete this;
}
}

#endif // CS_REF_COUNT_SOURCE
