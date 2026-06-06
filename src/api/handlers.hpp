#ifndef HANDLERS_HPP
#define HANDLERS_HPP

#include <httplib.h>            // для HTTP сервера
#include <nlohmann/json.hpp>    // для работы с JSON

#include "storage/storage.hpp"  // для работы с PatientStorage (хранилищем)
#include "core/triage.hpp"      // для вызова бизнес-логики
#include "utils/errors.hpp"     // собственное исключение с передачей имени поля
#include "utils/json_utils.hpp"       // функции сериализации и десераилизации при работе с JSON

// Регистрация обработчиков на сервере
void setupHandlers(httplib::Server &svr, PatientStorage &storage);

// Обработчик POST /patients
void handlePostPatients(const httplib::Request& req, httplib::Response& res, PatientStorage& storage);

// Обработчик GET /patients/{id}
void handleGetPatientById(const httplib::Request &req, httplib::Response &res, const PatientStorage &storage);

// Обработчик GET /patients
void handleGetAllPatients(const httplib::Request &req, httplib::Response &res, const PatientStorage &storage);

#endif