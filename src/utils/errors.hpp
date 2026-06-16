#pragma once

#include <stdexcept>
#include <string>

class ValidationError : public std::exception {
public:  
    ValidationError(const std::string &field, const std::string &message) {
        message_ = "Field '" + field + "': " + message;
    }

    const char* what() const noexcept override {
        return message_.c_str();
    }
private:
    std::string message_;
};
