#pragma once

#include <stdexcept>
#include <string>

class ValidationError : public std::exception {
public:
    std::string field_;
    std::string message_;
    mutable std::string full_message_;   

    ValidationError(const std::string &field, const std::string &message)
        : field_(field), message_(message) {}

    const char* what() const noexcept override {
        full_message_ = "Field '" + field_ + "': " + message_;
        return full_message_.c_str();
    }
};
