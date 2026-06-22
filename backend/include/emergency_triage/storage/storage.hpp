#pragma once

#include <cstdint>
#include <optional>
#include <unordered_map>
#include <vector>

#include "emergency_triage/storage/data_structures.hpp"
#include "emergency_triage/storage/patient.hpp"

namespace emergency_triage {

class PatientStorage {
public:
	uint32_t addPatient(const PatientClientData& patientClientData,
						std::chrono::system_clock::time_point request_receipt_time, PatientStatus status);

	std::optional<Patient> getPatient(uint32_t id) const;

	std::vector<Patient> getAllPatients() const;

private:
	std::unordered_map<uint32_t, Patient> patientsStorage_;
	uint32_t nextId_ = 1;
};

}  // namespace emergency_triage
