#pragma once

#include <chrono>
#include <cstdint>

#include "emergency_triage/storage/data_structures.hpp"

namespace emergency_triage {

class Patient {
public:
	Patient(uint32_t id, std::chrono::system_clock::time_point timestamp, PatientStatus status,
			const PatientClientData& patient_client_data);

	// getters
	std::chrono::system_clock::time_point getTimestamp() const noexcept {
		return timestamp_;
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

	const EmergencyData& getEmergencyData() const noexcept {
		return emergency_data_;
	}

	const TriageData& getTriageData() const noexcept {
		return triage_data_;
	}

	const DemographicData& getDemographicData() const noexcept {
		return demographic_data_;
	}

	// setters
	void setPriority(uint8_t priority);
	void setStatus(PatientStatus status);
	void setEmergencyData(const EmergencyData& emergency_data);
	void setTriageData(const TriageData& triage_data);
	void setDemographicData(const DemographicData& demographic_data);

private:
	std::chrono::system_clock::time_point timestamp_;
	uint32_t id_;

	uint8_t priority_;
	PatientStatus status_;

	EmergencyData emergency_data_;
	TriageData triage_data_;
	DemographicData demographic_data_;
};

}
