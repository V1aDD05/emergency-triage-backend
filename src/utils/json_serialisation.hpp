#pragma once

#include <cstdint>
#include <optional>

#include <httplib.h>
#include <nlohmann/json.hpp>

#include "emergency_triage/storage/data_structures.hpp"
#include "emergency_triage/storage/patient.hpp"
#include "emergency_triage/storage/storage.hpp"

namespace emergency_triage {

EmergencyParams deserialiseEmergencyParams(const nlohmann::json& json);

TriageData deserialiseTriageData(const nlohmann::json& json);

DemographyData deserialiseDemographyData(const nlohmann::json& json);

uint32_t deserialiseID(const httplib::Request& req);

nlohmann::json serialiseJSON(const Patient& patient);

nlohmann::json serialiseJSON(const std::optional<Patient>& patient);

}
