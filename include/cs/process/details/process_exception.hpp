#ifndef CS_PROCESSEXCEPTION_HPP
#define CS_PROCESSEXCEPTION_HPP

#include <string>
#include <exception>

namespace cs {
namespace details {
class ProcessException : public std::exception {
public:
    explicit ProcessException(const std::string& message, int code = 0)
    : message_(message)
    , code_(code) {
    }

    ProcessException(const ProcessException&) = default;
    ProcessException(ProcessException&&) = default;

    virtual const char* what() const noexcept override {
        return message_.c_str();
    }

    int code() const {
        return code_;
    }

private:
    std::string message_;
    int code_ = 0;
};
} // namespace details
}

#endif // CS_PROCESSEXCEPTION_HPP
