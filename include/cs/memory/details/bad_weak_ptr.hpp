#ifndef CS_BAD_WEAK_PTR_HPP
#define CS_BAD_WEAK_PTR_HPP

#include <exception>

namespace cs::details {
class BadWeakPtrException : public std::exception {
public:
    virtual const char* what() const noexcept override {
        return "Bad weak ptr";
    }
};
}

#endif
