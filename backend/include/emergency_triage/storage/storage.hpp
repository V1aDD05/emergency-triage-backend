#pragma once

#include <cstdint>
#include <optional>
#include <unordered_map>
#include <vector>

#include "emergency_triage/storage/data_structures.hpp"
#include "emergency_triage/storage/patient.hpp"

namespace emergency_triage {

class IPatientStorage {
public:
	virtual ~IPatientStorage() = default;
	virtual uint32_t addPatient(std::chrono::system_clock::time_point requestReceiptTime,
								const PatientClientData& patientClientData, uint8_t priority, PatientStatus status) = 0;
	virtual std::optional<Patient> getPatient(uint32_t id) const = 0;
	virtual std::vector<Patient> getAllPatients() const = 0;
};
class PatientStorage : public IPatientStorage {
public:
	uint32_t addPatient(std::chrono::system_clock::time_point requestReceiptTime,
						const PatientClientData& patientClientData, uint8_t priority, PatientStatus status) override;

	std::optional<Patient> getPatient(uint32_t id) const override;

	std::vector<Patient> getAllPatients() const override;

private:
	std::unordered_map<uint32_t, Patient> patientsStorage_;
	uint32_t nextId_ = 0;
};

}  // namespace emergency_triage
