#include "storage.hpp"

#include <iterator>
#include <vector>

// Добавление пациента при помощи информации, полученной от клиента "карета скорой помощи". 
// Возвращает присвоенный пациенту ID.
uint32_t PatientStorage::addPatient(uint32_t mask, uint8_t priority,
                                    std::optional<uint8_t> age, std::optional<Gender> sex)
{
    auto now = std::chrono::system_clock::now();
    PatientStatus status = PatientStatus::OnTheWay;
    Patient patient(now, mask, nextId_, priority, status, age, sex);
    patientsStorage_.insert({patient.id_, patient});
    ++nextId_;
    return patient.id_;
}

// Передача информации о конкретном пациенте
std::optional<Patient> PatientStorage::getPatient(uint32_t id) const
{
    auto it = patientsStorage_.find(id);
    if (it != patientsStorage_.end())
    {
        return it->second;
    }

    return std::nullopt;
}

// Передача информации обо всех пациентах в очереди
std::vector<Patient> PatientStorage::getAllPatients() const
{
    std::vector<Patient> result;
    result.reserve(patientsStorage_.size());

    for (const auto &[id, patient] : patientsStorage_)
    {
        result.push_back(patient);
    }

    return result;
}