#pragma once

#include <chrono>
#include <cstdint>

#include "emergency_triage/storage/data_structures.hpp"

namespace emergency_triage {

class Patient {
public:
	Patient(uint32_t id, std::chrono::system_clock::time_point timestamp, PatientStatus status,
			const AmbulanceData& ambulance_data);

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

	const EmergencyParams& getEmergencyParams() const noexcept {
		return emergency_params_;
	}

	const TriageData& getTriageData() const noexcept {
		return triage_data_;
	}

	const DemographyData& getDemographyData() const noexcept {
		return demography_data_;
	}

	// setters
	void setPriority(uint8_t priority);
	void setStatus(PatientStatus status);
	void setEmergencyParams(const EmergencyParams& emergency_params);
	void setTriageData(const TriageData& triage_data);
	void setDemographyData(const DemographyData& demography_data);

private:
	const std::chrono::system_clock::time_point timestamp_;
	const uint32_t id_;

	uint8_t priority_;
	PatientStatus status_;

	EmergencyParams emergency_params_;
	TriageData triage_data_;
	DemographyData demography_data_;
};

}
