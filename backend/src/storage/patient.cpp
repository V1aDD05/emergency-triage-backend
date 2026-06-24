#include "emergency_triage/storage/patient.hpp"

#include <optional>

#include "emergency_triage/utils/errors.hpp"
#include "core/triage.hpp"

namespace emergency_triage {

Patient::Patient(std::chrono::system_clock::time_point request_receipt_time, uint32_t id,
				 const PatientClientData& patient_client_data, uint8_t priority, PatientStatus status)
	: request_receipt_time_(request_receipt_time),
	  id_(id),
	  emergency_data_(std::move(patient_client_data.emergency_data)),
	  triage_data_(std::move(patient_client_data.triage_data)),
	  demographic_data_(std::move(patient_client_data.demographic_data)),
	  priority_(priority),
	  status_(status) {
	validateAll();
}

// validation
void Patient::validateStatus(PatientStatus status) {
	switch (status) {
		case PatientStatus::OnTheWay:
		case PatientStatus::Waiting:
		case PatientStatus::InSurgery:
		case PatientStatus::IntensiveCare:
		case PatientStatus::Died:
			return;
		default:
			throw ValidationError("status", "Invalid patient status");
	}
}

void Patient::validatePriority(uint8_t priority) {
	if (priority < 1 || priority > 4) {
		throw ValidationError("priority", "Must be in range [1..4]");
	}
}

void Patient::validateAll() {
	validateStatus(status_);
	validatePriority(priority_);
}

// setters
void Patient::setStatus(PatientStatus status) {
	validateStatus(status);
	status_ = status;
}

}
