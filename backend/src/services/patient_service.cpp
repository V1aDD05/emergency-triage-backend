#include "emergency_triage/services/patient_service.hpp"

namespace emergency_triage {
uint32_t PatientService::addPatient(std::chrono::system_clock::time_point requestReceiptTime,
								  PatientClientData patientClientData) {
	PatientStatus status = PatientStatus::OnTheWay;
	uint8_t priority = calculator_->computePriority(patientClientData.emergency_data, patientClientData.triage_data);
	return storage_->addPatient(requestReceiptTime, patientClientData, priority, status);
}

std::optional<Patient> PatientService::getPatientById(uint32_t id) const {
	return storage_->getPatient(id);
}

std::vector<Patient> PatientService::getPatients() const {
	return storage_->getAllPatients();
}
}
