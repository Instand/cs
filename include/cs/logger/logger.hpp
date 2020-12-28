#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <iostream>
#include <sstream>

namespace cs {
// implements simple logging needs
class Logger {
public:
    explicit Logger(const std::string& header = {}) {
        if (!header.empty()) {
            stream_ << header << ": ";
        }
    }

    ~Logger() {
        stream_ << '\n';
        std::cout << stream_.str();
    }

    Logger& operator()() {
        return *this;
    }

private:
    std::stringstream stream_;

    template<typename T>
    friend Logger& operator<<(Logger& logger, const T& value);
};

template<typename T>
inline Logger& operator<<(Logger& logger, const T& value) {
    logger.stream_ << value;
    return logger;
}

#define cslog cs::Logger()
}

#endif // LOGGER_HPP
