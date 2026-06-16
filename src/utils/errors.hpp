#pragma once

#include <stdexcept>
#include <format>
#include <string>

class ValidationError : public std::runtime_error {
public:  
    ValidationError(const std::string &field, const std::string &message) : 
    std::runtime_error(std::format("Field '{}': {}", field, message)) {}
};
