#ifndef CS_BENCHMARK_TIMEOUT_EXECTION_HPP
#define CS_BENCHMARK_TIMEOUT_EXECTION_HPP

#include <exception>

namespace cs::details {
class TimeOutException : public std::exception {
public:
    virtual const char* what() const noexcept override {
        return "Benchmark Time out";
    }
};
}

#endif // CS_BENCHMARK_TIMEOUT_EXECTION_HPP
