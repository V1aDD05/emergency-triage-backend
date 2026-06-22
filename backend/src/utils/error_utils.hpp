#pragma once

#include <stdexcept>
#include <string>
#include <format>

#include <nlohmann/json.hpp>

#include "emergency_triage/utils/errors.hpp"

namespace emergency_triage {
    
template <typename Func>
auto catchValidationErrors(Func&& func, const std::string& field)
try {
	return func();
}
catch (const nlohmann::json::parse_error& e) {
	throw ValidationError(field, std::format("Invalid JSON format: {}", e.what()));
}
catch (const nlohmann::json::type_error& e) {
	throw ValidationError(field, std::format("Type mismatch: {}", e.what()));
}
catch (const std::invalid_argument& e) {
	throw ValidationError(field, std::format("Invalid JSON format: {}", e.what()));
}
catch (const std::out_of_range& e) {
	throw ValidationError(field, std::format("Value out of range: {}", e.what()));
}
catch (const std::exception& e) {
	throw ValidationError(field, std::format("Unexpected client error: {}", e.what()));
}
}
