#pragma once

#include <chrono>
#include <cstdint>
#include <optional>

#include "nlohmann/json.hpp"

namespace emergency_triage {

enum class Gender : uint8_t { Male, Female };

enum class PatientStatus : uint8_t { OnTheWay, Waiting, InSurgery, IntensiveCare, Died };

struct EmergencyData {
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

struct DemographicData {
	std::optional<uint8_t> age;
	std::optional<Gender> sex;
};

struct PatientClientData {
	EmergencyData emergency_data;
	TriageData triage_data;
	DemographicData demographic_data;
};

}  // namespace emergency_triage
