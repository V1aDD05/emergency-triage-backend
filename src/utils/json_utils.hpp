#pragma once

#include <chrono>
#include <cstdint>
#include <optional>

#include <nlohmann/json.hpp>

#include "storage/storage.hpp"
#include "utils/errors.hpp"

std::optional<Gender> deserialiseGender(const nlohmann::json& json, const std::string& key);

nlohmann::json serialiseJSON(const Patient& patient);

nlohmann::json serialiseJSON(const std::optional<Patient>& patient);
