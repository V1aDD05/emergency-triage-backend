#include "emergency_triage/storage/patient.hpp"

#include <optional>

#include "emergency_triage/utils/errors.hpp"
#include "core/triage.hpp"

namespace emergency_triage {

Patient::Patient(uint32_t id, std::chrono::system_clock::time_point request_receipt_time, PatientStatus status,
				 const PatientClientData& patient_client_data)
	: id_(id), request_receipt_time_(request_receipt_time) {
	setStatus(status);
	setEmergencyData(patient_client_data.emergency_data);
	setTriageData(patient_client_data.triage_data);
	setDemographicData(patient_client_data.demographic_data);
	setPriority(computePriority(getEmergencyData(), getTriageData()));
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

void Patient::setEmergencyData(const EmergencyData& emergency_data) {
	emergency_data_ = emergency_data;
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

void Patient::setDemographicData(const DemographicData& demographic_data) {
	if (demographic_data.age.has_value() && *demographic_data.age > 130) {
		throw ValidationError("age", "Must be in range [0..130]");
	}
	demographic_data_ = demographic_data;
}

void Patient::setPriority(uint8_t priority) {
	if (priority < 1 || priority > 4) {
		throw ValidationError("priority", "Must be in range [1..4]");
	}
	priority_ = priority;
}

}
