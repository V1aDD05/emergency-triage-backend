#include "handlers.hpp"

#include <optional>
#include <string>
#include <stdexcept>    // для std::invalid_argument
#include <limits>       // для std::numeric_limits

// Обработчик POST /patients
void handlePostPatients(const httplib::Request &req, httplib::Response &res, PatientStorage &storage) {
    try {
        // 1. Десериализация JSON. Возможное исключение: "nlohmann::json::parse_error"
        auto json = nlohmann::json::parse(req.body);

        // 2. Извлечение маски
        // 2.1 Проверка наличия поля маски
        if (!json.contains("mask")) {
            throw ValidationError("mask", "Missing required field");
        }
        // 2.2 Проверка соответствия маски заданному диапазону значений `uint32_t`
        // 2.2.1 Проверка беззнаковости (0...std::numeric_limits<uint64_t>::max())
        if (!json["mask"].is_number_unsigned()){
            throw ValidationError("mask", "Must be a non-negative integer");
        }
        // 2.2.2 Проверка попадания в диапазон значений `uint32_t`
        uint64_t mask_raw = json["mask"].get<uint64_t>();
        if (mask_raw > std::numeric_limits<uint32_t>::max())
        {
            throw ValidationError("mask", "Must be in range of <uint32_t> (0..4294967295)");
        }

        uint32_t mask = static_cast<uint32_t>(mask_raw);

        // 3. Извлечение возраста (опционально)
        std::optional<uint8_t> age = std::nullopt;
        if (json.contains("age") && !json["age"].is_null()){
            // 3.1 Проверка соответствия возраста заданному диапазону значений `uint8_t`
            // 3.1.1 Проверка беззнаковости (0...std::numeric_limits<uint64_t>::max())
            if (!json["age"].is_number_unsigned())
            {
                throw ValidationError("age", "Must be a non-negative integer");
            }
            // 3.1.2 Проверка попадания в диапазон значений `uint8_t`
            uint64_t age_raw = json["age"].get<uint64_t>();
            if (age_raw > std::numeric_limits<uint8_t>::max())
            {
                throw ValidationError("age", "Must be in range of <uint8_t> (0..255)");
            }

            age = json["age"].get<uint8_t>();
        }

        // 4. Извлечение пола (опционально). В случае исключений бросает "ValidationError"
        std::optional<Gender> sex = deserialiseGender(json, "sex");

        // 5. Вычисление приоритета. Возможное исключение: "std::logic_error"
        uint8_t priority = computePriority(mask);

        // Добавление пациента: вызываем метод `addPatient` класса `PatientStorage`. 
        // Метод вызывает конструктор структуры `Patient` и добавляет структуру в `storage`
        uint32_t id = storage.addPatient(mask, priority, age, sex);

        // TODO: На этапе 2 добавим здесь оценку времени пациента до операции,
        // пока используем заглушку ниже

        // Формирование ответа. Случай успешного добавления записи о пациенте
        nlohmann::json resp = {{"id", id}, {"estimated_wait_time", 0}}; // заглушка применительно ко времени ожидания
        res.set_content(resp.dump(), "application/json");
        res.status = 201;
    }
    // ОБРАБОТКА ИСКЛЮЧЕНИЙ

    // обработка исключения, которое может быть выброшено на шаге 1
    catch (const nlohmann::json::parse_error& e) {
        res.status = 400;
        res.set_content("{\"error\":\"Invalid JSON format: " + std::string(e.what()) + "\"}", "application/json");
    }

    // обработка исключений, которые могут быть выброшены на шагах 2 - 4
    catch (const ValidationError &e) {
        res.status = 400;
        res.set_content("{\"error\":\"" + std::string(e.what()) + "\"}", "application/json");
    }

    // обработка исключения, которое может быть выброшено на шаге 5
    catch (const std::logic_error &e) {
        res.status = 500;
        res.set_content("{\"error\":\"Internal server error (business logic): " + std::string(e.what()) + "\"}", "application/json");
    }

    catch (const std::exception& e) {
        res.status = 500;
        res.set_content("{\"error\":\"Internal server error\"}", "application/json");
    }
}

// Обработчик GET patients/{id}
void handleGetPatientById(const httplib::Request &req, httplib::Response &res, const PatientStorage &storage) {
    try{
        // Извлечение ID из URL
        std::string id_str = req.matches[1];

        // 1. Преобразование строки в число с проверкой
        size_t pos;
        // 1.1 Проверка беззнаковости (0...std::numeric_limits<uint64_t>::max())
        uint64_t id_raw = std::stoull(id_str, &pos);
        if (pos != id_str.size())
        {
            throw ValidationError("id", "Must be a non-negative integer (no trailing characters)");
        }
        // 1.2 Проверка попадания в диапазон значений `uint32_t`
        if (id_raw > std::numeric_limits<uint32_t>::max())
        {
            throw ValidationError("id", "Must be in range of <uint32_t> (0..4294967295)");
        }
        uint32_t id = static_cast<uint32_t>(id_raw);

        // Поиск пациента с заданным id
        auto patient = storage.getPatient(id);
        if (!patient) {
            res.status = 404;
            res.set_content("{\"error\":\"Patient not found\"}", "application/json");
            return;
        }

        // Формирование JSON для найденного пациента
        nlohmann::json json = serialiseJSON(patient);
        
        // Формирование ответа. Случай успешного обнаружения пациента
        res.set_content(json.dump(), "application/json");
        res.status = 200;
    }
    // ОБРАБОТКА ИСКЛЮЧЕНИЙ

    catch (const std::invalid_argument &e)
    {
        res.status = 400;
        res.set_content("{\"error\":\"Invalid patient ID\"}", "application/json");
    }

    catch (const ValidationError &e)
    {
        res.status = 400;
        res.set_content("{\"error\":\"" + std::string(e.what()) + "\"}", "application/json");
    }

    catch (const std::exception &e)
    {
        res.status = 500;
        res.set_content("{\"error\":\"Internal server error\"}", "application/json");
    }
}

// Обработчик GET /patients
void handleGetAllPatients(const httplib::Request &req, httplib::Response &res, const PatientStorage &storage) {
    try{
        std::vector<Patient> patients = storage.getAllPatients();

        // Формирование ответа. Случай пустого массива
        if (patients.empty())
        {
            res.set_content("[]", "application/json");
            res.status = 200;
            return;
        }

        // Формирование ответа. Случай непустого массива

        // Формирование JSON из массива записей `Patient`
        nlohmann::json array = nlohmann::json::array();
        for (const auto &patient : patients)
        {
            array.push_back(serialiseJSON(patient));
        }

        res.set_content(array.dump(), "application/json");
        res.status = 200;
    }

    catch (const std::exception &e)
    {
        res.status = 500;
        res.set_content("{\"error\":\"Internal server error\"}", "application/json");
    }
}

// Функция, регистрирующая обработчики на сервере
void setupHandlers(httplib::Server &svr, PatientStorage &storage) {
    svr.Post("/patients", [&storage](const httplib::Request &req, httplib::Response &res) {
        handlePostPatients(req,res,storage);
    });
    svr.Get(R"(/patients/(\d+))", [&storage](const httplib::Request &req, httplib::Response &res) {
        handleGetPatientById(req,res,storage);
    });
    svr.Get("/patients", [&storage](const httplib::Request &req, httplib::Response &res) {
        handleGetAllPatients(req,res,storage);
    });
}
