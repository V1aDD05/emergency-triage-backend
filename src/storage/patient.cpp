#include "emergency_triage/storage/patient.hpp"

#include <optional>

#include "core/triage.hpp"
#include "emergency_triage/utils/errors.hpp"

namespace emergency_triage {

Patient::Patient(uint32_t id, std::chrono::system_clock::time_point timestamp, PatientStatus status,
				 const AmbulanceData& ambulance_data)
	: id_(id), timestamp_(timestamp) {
	setStatus(status);
	setEmergencyParams(ambulance_data.emergency_params);
	setTriageData(ambulance_data.triage_data);
	setDemographyData(ambulance_data.demography_data);
	setPriority(computePriority(getEmergencyParams(), getTriageData()));
}

// setters
void Patient::setStatus(PatientStatus status) {
	switch (status) {
		case PatientStatus::OnTheWay:
		case PatientStatus::Waiting:
		case PatientStatus::InSurgery:
		case PatientStatus::IntensiveCare:
		case PatientStatus::Died:
			status_ = status;
			break;
		default:
			throw ValidationError("status", "Invalid patient status");
	}
}

void Patient::setEmergencyParams(const EmergencyParams& emergency_params) {
	emergency_params_ = emergency_params;
}

void Patient::setTriageData(const TriageData& triage_data) {
	if (triage_data.eye_response < 1 || triage_data.eye_response > 4) {
		throw ValidationError("eye_response", "Must be in range [1..4]");
	}
	if (triage_data.verbal_response < 1 || triage_data.verbal_response > 5) {
		throw ValidationError("verbal_response", "Must be in range [1..5]");
	}
	if (triage_data.motor_response < 1 || triage_data.motor_response > 6) {
		throw ValidationError("motor_response", "Must be in range [1..6]");
	}
	if (triage_data.respiratory_rate > 60) {
		throw ValidationError("respiratory_rate", "Must be in range [0..60]");
	}
	if (triage_data.systolic_bp > 300) {
		throw ValidationError("systolic_bp", "Must be in range [0..300]");
	}

	triage_data_ = triage_data;
}

void Patient::setDemographyData(const DemographyData& demography_data) {
	if (demography_data.age.has_value() && *demography_data.age > 130) {
		throw ValidationError("age", "Must be in range [0..130]");
	}
	demography_data_ = demography_data;
}

void Patient::setPriority(uint8_t priority) {
	if (priority < 1 || priority > 4) {
		throw ValidationError("priority", "Must be in range [1..4]");
	}
	priority_ = priority;
}

}
