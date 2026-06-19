#include "storage.hpp"

#include <chrono>

uint32_t PatientStorage::addPatient(const AmbulanceData& ambulanceData) {
    auto now = std::chrono::system_clock::now();
    PatientStatus status = PatientStatus::OnTheWay;
    Patient patient(nextId_, now, status, ambulanceData);
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
