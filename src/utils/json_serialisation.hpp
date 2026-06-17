#pragma once

#include <chrono>
#include <cstdint>
#include <optional>

#include <httplib.h>
#include <nlohmann/json.hpp>

#include "storage/storage.hpp"
#include "utils/errors.hpp"

uint32_t deserialiseMask(const nlohmann::json& json, const std::string& key);

std::optional<uint8_t> deserialiseAge(const nlohmann::json& json, const std::string& key);

std::optional<Gender> deserialiseGender(const nlohmann::json& json, const std::string& key);

uint32_t deserialiseID(const httplib::Request& req);

nlohmann::json serialiseJSON(const Patient& patient);

nlohmann::json serialiseJSON(const std::optional<Patient>& patient);
