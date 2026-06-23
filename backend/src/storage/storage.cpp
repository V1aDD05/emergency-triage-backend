#include "emergency_triage/storage/storage.hpp"

#include <chrono>

namespace emergency_triage {

uint32_t PatientStorage::addPatient(std::chrono::system_clock::time_point requestReceiptTime,
									const PatientClientData& patientClientData, uint8_t priority,
									PatientStatus status) {
	Patient patient(requestReceiptTime, nextId_, patientClientData, priority, status);
	patientsStorage_.insert({patient.getId(), patient});
	++nextId_;
	return patient.getId();
}

std::optional<Patient> PatientStorage::getPatient(uint32_t id) const {
	auto it = patientsStorage_.find(id);
	if (it != patientsStorage_.end()) {
		return it->second;
	}

	return std::nullopt;
}

std::vector<Patient> PatientStorage::getAllPatients() const {
	std::vector<Patient> result;
	result.reserve(patientsStorage_.size());

	for (const auto& [_, patient] : patientsStorage_) {
		result.push_back(patient);
	}

	return result;
}

}
