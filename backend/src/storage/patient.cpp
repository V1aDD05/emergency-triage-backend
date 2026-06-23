#include "emergency_triage/storage/patient.hpp"

#include <optional>

#include "emergency_triage/utils/errors.hpp"
#include "core/triage.hpp"

namespace emergency_triage {

Patient::Patient(std::chrono::system_clock::time_point request_receipt_time, uint32_t id,
				 const PatientClientData& patient_client_data, uint8_t priority, PatientStatus status)
	: request_receipt_time_(request_receipt_time),
	  id_(id),
	  emergency_data_(patient_client_data.emergency_data),
	  triage_data_(patient_client_data.triage_data),
	  demographic_data_(patient_client_data.demographic_data) {
	setPriority(priority);
	setStatus(status);
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
	triage_data_ = triage_data;
}

void Patient::setDemographicData(const DemographicData& demographic_data) {
	demographic_data_ = demographic_data;
}

void Patient::setPriority(uint8_t priority) {
	if (priority < 1 || priority > 4) {
		throw ValidationError("priority", "Must be in range [1..4]");
	}
	priority_ = priority;
}

}
