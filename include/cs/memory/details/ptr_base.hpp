#ifndef PTR_BASE_HPP
#define PTR_BASE_HPP

#include <atomic>
#include <string>
#include <exception>

namespace cs {
class PtrBaseException : public std::exception {
public:
    PtrBaseException() = default;
    explicit PtrBaseException(const char* message);

    virtual const char* what() const noexcept override;

private:
    std::string_view view_;
};

// represents base class for smart pointers with reference counts
class PtrBase {
public:
    PtrBase();

    // returns current used references
    int useCount() const;

protected:
    void addReference();

private:
    std::atomic<int> referenceCount_ = { 0 };
};

// impementation

PtrBaseException::PtrBaseException(const char* message) : view_(message) {}

const char* PtrBaseException::what() const noexcept {
    return view_.data();
}

PtrBase::PtrBase() {
    addReference();
}

int PtrBase::useCount() const {
    return referenceCount_.load();
}

void PtrBase::addReference() {
    referenceCount_.fetch_add(1);
}
}

#endif // PTR_BASE_HPP
