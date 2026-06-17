#include "handlers.hpp"

#include <optional>
#include <string>

#include <nlohmann/json.hpp>

#include "core/triage.hpp"
#include "utils/errors.hpp"
#include "utils/json_serialisation.hpp"

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
        res.set_content("{\"error\":\"Invalid argument\"}", "application/json");
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

    uint32_t mask = deserialiseMask(json, "mask");

    std::optional<uint8_t> age = deserialiseAge(json, "age");

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
    uint32_t id = deserialiseID(req);

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
