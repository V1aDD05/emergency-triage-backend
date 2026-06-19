#pragma once

#include <cstdint>
#include <optional>
#include <unordered_map>
#include <vector>

#include "data_structures.hpp"
#include "patient.hpp"

class PatientStorage {
public:
    uint32_t addPatient(const AmbulanceData& ambulanceData);

    std::optional<Patient> getPatient(uint32_t id) const;

    std::vector<Patient> getAllPatients() const;

private:
    std::unordered_map<uint32_t, Patient> patientsStorage_;
    uint32_t nextId_ = 1;
};
