#ifndef JSON_UTILS_HPP
#define JSON_UTILS_HPP

#include <nlohmann/json.hpp>
#include <optional>
#include <cstdint>
#include <chrono>                   // for timestamp
#include "storage/storage.hpp"      // для работы с PatientStorage (хранилищем)
#include "utils/errors.hpp"         // собственное исключение с передачей имени поля

// Десериализация Gender
std::optional<Gender> deserialiseGender(const nlohmann::json& json, const std::string &key);

// Сериализация существующего пациента
nlohmann::json serialiseJSON(const Patient& patient);

// Сериализация опционального пациента (если нет значения, возвращает null)
nlohmann::json serialiseJSON(const std::optional<Patient>& patient);

#endif