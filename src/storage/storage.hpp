#ifndef STORAGE_HPP
#define STORAGE_HPP

#include <unordered_map>
#include <vector>
#include <chrono>   // for timestamp
#include <cstdint>  // for uint32_t and uint8_t fields  
#include <optional> // for 'age' and 'sex' fields

// Перечисление для указания пола пациента
enum class Gender : uint8_t {

    Male,
    Female
};

// Перечисление для отображения текущего статуса оказания помощи пациенту
enum class PatientStatus : uint8_t
{
    OnTheWay,      // находится в карете скорой помощи
    Waiting,       // находится в больнице, в очереди в операционную
    InSurgery,     // находится на операции
    IntensiveCare, // после операции переведён в реанимацию
    Died           // умер
};

struct Patient {
    std::chrono::system_clock::time_point timestamp_;   // время поступления запроса
    uint32_t mask_;                                     // битовая маска симптомов (бит 0 – отсутствует, бит 1 – присутствует)
    uint32_t id_;                                       // уникальный идентификатор пациента (автоинкрементный)
    uint8_t priority_;                                  // приоритет (чем меньше число, тем выше приоритет)
    PatientStatus status_;                              // статус пациента
    std::optional<uint8_t> age_;                        // возраст (опционально)
    std::optional<Gender> sex_;                         // пол (опционально)
    

    Patient(const std::chrono::system_clock::time_point &ts, uint32_t mask, uint32_t id, uint8_t priority, PatientStatus status,
            std::optional<uint8_t> age = std::nullopt, std::optional<Gender> sex = std::nullopt)
        : timestamp_(ts), mask_(mask), id_(id), priority_(priority), status_(status), age_(age), sex_(sex) {}
};

class PatientStorage {
public:
    // Добавление пациента при помощи информации, полученной от клиента "карета скорой помощи" (mask, age, sex)
    // и от бизнес-логики (priority). Возвращает присвоенный пациенту ID.
    uint32_t addPatient(uint32_t mask, uint8_t priority,
                        std::optional<uint8_t> age = std::nullopt,
                        std::optional<Gender> sex = std::nullopt);

    // Передача информации о конкретном пациенте
    std::optional<Patient> getPatient(uint32_t id) const;

    // Передача информации обо всех пациентах в очереди
    std::vector<Patient> getAllPatients() const;        

private:
    std::unordered_map<uint32_t, Patient> patientsStorage_;
    uint32_t nextId_ = 1;
};

#endif