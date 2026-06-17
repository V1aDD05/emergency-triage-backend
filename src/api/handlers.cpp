#include "handlers.hpp"

#include <limits>
#include <optional>
#include <stdexcept>
#include <string>

void commonExceptionHandler(const httplib::Request& req, httplib::Response& res, std::exception_ptr ep) {
    [[maybe_unused]] req;
    try {
        std::rethrow_exception(ep);
    }
    // mask, age, sex processing exceptions
    catch (const ValidationError& e) {
        res.status = 400;
        res.set_content("{\"error\":\"" + std::string(e.what()) + "\"}", "application/json");
    }
    catch (const nlohmann::json::parse_error& e) {
        res.status = 400;
        res.set_content("{\"error\":\"Invalid JSON format: " + std::string(e.what()) + "\"}", "application/json");
    }
    catch (const std::invalid_argument& e) {
        res.status = 400;
        res.set_content("{\"error\":\"Invalid patient ID\"}", "application/json");
    }
    // priority computing exception
    catch (const std::logic_error& e) {
        res.status = 500;
        res.set_content("{\"error\":\"Internal server error (business logic): " + std::string(e.what()) + "\"}",
                        "application/json");
    }
    catch (const std::exception& e) {
        res.status = 500;
        res.set_content("{\"error\":\"Internal server error\"}", "application/json");
    }

}

void handlePostPatients(const httplib::Request& req, httplib::Response& res, PatientStorage& storage) {
    auto json = nlohmann::json::parse(req.body);

    // mask extraction
    if (!json.contains("mask")) {
        throw ValidationError("mask", "Missing required field");
    }

    if (!json["mask"].is_number_unsigned()) {
        throw ValidationError("mask", "Must be a non-negative integer");
    }

    uint64_t mask_raw = json["mask"].get<uint64_t>();
    if (mask_raw > std::numeric_limits<uint32_t>::max()) {
        throw ValidationError("mask", "Must be in range of <uint32_t> (0..4294967295)");
    }

    uint32_t mask = static_cast<uint32_t>(mask_raw);

    // age extraction
    std::optional<uint8_t> age = std::nullopt;
    if (json.contains("age") && !json["age"].is_null()) {
        if (!json["age"].is_number_unsigned()) {
            throw ValidationError("age", "Must be a non-negative integer");
        }

        uint64_t age_raw = json["age"].get<uint64_t>();
        if (age_raw > std::numeric_limits<uint8_t>::max()) {
            throw ValidationError("age", "Must be in range of <uint8_t>  (0..255)");
        }

        age = json["age"].get<uint8_t>();
    }

    std::optional<Gender> sex = deserialiseGender(json, "sex");

    uint8_t priority = computePriority(mask);

    uint32_t id = storage.addPatient(mask, priority, age, sex);

    // TODO: remove stub for `estimated_wait_time` at the
    // stage 3
    nlohmann::json resp = {{"id", id}, {"estimated_wait_time", 0}};

    res.set_content(resp.dump(), "application/json");
    res.status = 201;
}

void handleGetPatientById(const httplib::Request& req, httplib::Response& res, const PatientStorage& storage) {
    std::string id_str = req.matches[1];

    size_t pos;

    if (id_str.empty() || id_str[0] == '-') {
        throw ValidationError("id", "Must be a non-negative integer");
    }

    uint64_t id_raw = std::stoull(id_str, &pos);
    if (pos != id_str.size()) {
        throw ValidationError("id", "Must contain only digits(no trailing characters)");
    }

    if (id_raw > std::numeric_limits<uint32_t>::max()) {
        throw ValidationError("id", "Must be in range of <uint32_t> (0..4294967295)");
    }
    uint32_t id = static_cast<uint32_t>(id_raw);

    const auto patient = storage.getPatient(id);
    if (!patient) {
        res.status = 404;
        res.set_content("{\"error\":\"Patient not found\"}", "application/json");
        return;
    }

    nlohmann::json json = serialiseJSON(patient);

    res.set_content(json.dump(), "application/json");
    res.status = 200;
}

void handleGetPatients(const httplib::Request& req, httplib::Response& res, const PatientStorage& storage) {
    const std::vector<Patient> patients = storage.getAllPatients();

    if (patients.empty()) {
        res.set_content("[]", "application/json");
        res.status = 200;
        return;
    }

    nlohmann::json array = nlohmann::json::array();
    for (const auto& patient : patients) {
        array.push_back(serialiseJSON(patient));
    }

    res.set_content(array.dump(), "application/json");
    res.status = 200;
}

void setupHandlers(httplib::Server& server, PatientStorage& storage) {
    server.set_exception_handler(commonExceptionHandler);
    server.Post("/patients", [&storage](const httplib::Request& req, httplib::Response& res) {
        handlePostPatients(req, res, storage);
    });
    server.Get(R"(/patients/([^/]+))", [&storage](const httplib::Request& req, httplib::Response& res) {
        handleGetPatientById(req, res, storage);
    });
    server.Get("/patients", [&storage](const httplib::Request& req, httplib::Response& res) {
        handleGetPatients(req, res, storage);
    });
}
