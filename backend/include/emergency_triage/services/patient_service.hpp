#pragma once

#include <chrono>
#include <cstdint>
#include <optional>
#include <vector>

#include "emergency_triage/core/i_priority_calculator.hpp"
#include "emergency_triage/storage/patient.hpp"
#include "emergency_triage/storage/storage.hpp"

namespace emergency_triage {

class PatientService {
public:
	PatientService(IPatientStorage& storage, const IPriorityCalculator& calculator)
		: storage_(storage), calculator_(calculator) {
	}
	uint32_t addPatient(std::chrono::system_clock::time_point requestReceiptTime, PatientClientData patientClientData);
	std::optional<Patient> getPatientById(uint32_t id) const;
	std::vector<Patient> getPatients() const;
private:
	IPatientStorage& storage_;
	const IPriorityCalculator& calculator_;
};
}
