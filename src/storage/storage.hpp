#pragma once
#include <chrono>
#include <cstdint>
#include <optional>
#include <unordered_map>
#include <vector>

enum class Gender : uint8_t { Male, Female };

enum class PatientStatus : uint8_t { OnTheWay, Waiting, InSurgery, IntensiveCare, Died };

struct Patient {
    std::chrono::system_clock::time_point timestamp_;
    uint32_t mask_;
    uint32_t id_;
    uint8_t priority_;
    PatientStatus status_;
    std::optional<uint8_t> age_;
    std::optional<Gender> sex_;

    Patient(const std::chrono::system_clock::time_point &timestamp, uint32_t mask, uint32_t id, uint8_t priority,
            PatientStatus status, std::optional<uint8_t> age = std::nullopt, std::optional<Gender> sex = std::nullopt)
        : timestamp_(timestamp), mask_(mask), id_(id), priority_(priority), status_(status), age_(age), sex_(sex) {
    }
};

class PatientStorage {
public:
    uint32_t addPatient(uint32_t mask, uint8_t priority, std::optional<uint8_t> age = std::nullopt,
                        std::optional<Gender> sex = std::nullopt);

    std::optional<Patient> getPatient(uint32_t id) const;

    std::vector<Patient> getAllPatients() const;

private:
    std::unordered_map<uint32_t, Patient> patientsStorage_;
    uint32_t nextId_ = 1;
};
