#include "emergency_triage/api/handlers.hpp"

#include <optional>
#include <string>
#include <chrono>

#include <nlohmann/json.hpp>

#include "core/triage.hpp"
#include "emergency_triage/storage/data_structures.hpp"
#include "emergency_triage/utils/errors.hpp"
#include "spdlog/spdlog.h"
#include "utils/error_utils.hpp"
#include "utils/json_serialisation.hpp"

namespace emergency_triage {

void commonExceptionHandler(const httplib::Request& req, httplib::Response& res, std::exception_ptr ep) {
	[[maybe_unused]] req;
	try {
		std::rethrow_exception(ep);
	}
	catch (const emergency_triage::Exception& e) {
		res.status = e.httpStatus();
		nlohmann::json error;
		if (e.isShowToClient()) {
			error = {{"error", e.what()}};
			if (res.status == 409) {
				spdlog::warn(e.what());
			} else {
				spdlog::info(e.what());
			}
		} else {
			error = {{"error", "Internal server error. Please try again later."}};
			spdlog::error(e.what());
		}

		res.set_content(error.dump(), "application/json");
	}
	catch (const std::exception& e) {
		spdlog::error("Internal server error: {}", e.what());

		res.status = 500;
		nlohmann::json error = {{"error", "Internal server error. Please try again later."}};
		res.set_content(error.dump(), "application/json");
	}
}

void handlePostPatients(const httplib::Request& req, httplib::Response& res, PatientStorage& storage) {
	auto requestReceiptTime = std::chrono::system_clock::now();
	PatientStatus status = PatientStatus::OnTheWay;

	auto json = catchValidationErrors([&]() { return nlohmann::json::parse(req.body); }, "body");

	PatientClientData patientClientData = {.emergency_data = deserialiseEmergencyData(json["emergency_data"]),
										   .triage_data = deserialiseTriageData(json["triage_data"]),
										   .demographic_data = deserialiseDemographicData(json["demographic_data"])};

	uint32_t id = storage.addPatient(patientClientData, requestReceiptTime, status);

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

}
