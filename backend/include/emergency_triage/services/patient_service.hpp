#pragma once

#include <chrono>
#include <cstdint>
#include <optional>
#include <vector>
#include <memory>

#include "emergency_triage/core/i_priority_calculator.hpp"
#include "emergency_triage/storage/patient.hpp"
#include "emergency_triage/storage/storage.hpp"

namespace emergency_triage {

class PatientService {
public:
	PatientService(std::shared_ptr<IPatientStorage> storage, std::shared_ptr<const IPriorityCalculator> calculator)
		: storage_(std::move(storage)), calculator_(std::move(calculator)) {
	}
	uint32_t addPatient(std::chrono::system_clock::time_point requestReceiptTime, PatientClientData patientClientData);
	std::optional<Patient> getPatientById(uint32_t id) const;
	std::vector<Patient> getPatients() const;
private:
	std::shared_ptr<IPatientStorage> storage_;
	std::shared_ptr<const IPriorityCalculator> calculator_;
};
}
