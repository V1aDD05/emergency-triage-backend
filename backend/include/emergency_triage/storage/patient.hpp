#pragma once

#include <chrono>
#include <cstdint>

#include "emergency_triage/storage/data_structures.hpp"

namespace emergency_triage {

class Patient {
public:
	Patient(std::chrono::system_clock::time_point request_receipt_time, uint32_t id,
			const PatientClientData& patient_client_data, uint8_t priority, PatientStatus status);

	// getters
	std::chrono::system_clock::time_point getRequestReceiptTime() const {
		return request_receipt_time_;
	}

	uint32_t getId() const noexcept {
		return id_;
	}

	uint8_t getPriority() const noexcept {
		return priority_;
	}

	PatientStatus getStatus() const noexcept {
		return status_;
	}

	EmergencyData getEmergencyData() const noexcept {
		return emergency_data_;
	}

	TriageData getTriageData() const noexcept {
		return triage_data_;
	}

	const DemographicData& getDemographicData() const noexcept {
		return demographic_data_;
	}

	// setters
	void setStatus(PatientStatus status);

private:
	void validateStatus(PatientStatus status);
	void validatePriority(uint8_t priority);
	void validateAll();

	std::chrono::system_clock::time_point request_receipt_time_;
	uint32_t id_;

	uint8_t priority_;
	PatientStatus status_;

	EmergencyData emergency_data_;
	TriageData triage_data_;
	DemographicData demographic_data_;
};

}
