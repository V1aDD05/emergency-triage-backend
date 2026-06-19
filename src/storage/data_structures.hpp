#pragma once

#include <chrono>
#include <cstdint>
#include <optional>

#include "nlohmann/json.hpp"

enum class Gender : uint8_t { Male, Female };

enum class PatientStatus : uint8_t { OnTheWay, Waiting, InSurgery, IntensiveCare, Died };

struct EmergencyParams {
    bool is_bleeding = false;
    bool is_extensive_wounds = false;
    bool is_penetrating_wounds = false;
    bool is_consciousness_depression = false;
    bool is_respiratory_depression = false;
    bool is_hemodynamic_depression = false;
    bool is_severe_combined_injury = false;
};

struct TriageData {
    uint8_t eye_response = 0;
    uint8_t verbal_response = 0;
    uint8_t motor_response = 0;
    uint8_t respiratory_rate = 0;
    uint16_t systolic_bp = 0;
};

struct DemographyData {
    std::optional<uint8_t> age;
    std::optional<Gender> sex;
};

struct AmbulanceData {
    EmergencyParams emergency_params;
    TriageData triage_data;
    DemographyData demography_data;
};

/*
NLOHMANN_JSON_SERIALIZE_ENUM_STRICT(Gender, {{Gender::Male, "male"}, {Gender::Female, "female"}})

NLOHMANN_JSON_SERIALIZE_ENUM_STRICT(PatientStatus, {{PatientStatus::OnTheWay, "on_the_way"},
                                                    {PatientStatus::Waiting, "waiting"},
                                                    {PatientStatus::InSurgery, "in_surgery"},
                                                    {PatientStatus::IntensiveCare, "intensive_care"},
                                                    {PatientStatus::Died, "died"}})
*/
