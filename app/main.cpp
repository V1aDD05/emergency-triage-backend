#include <iostream>
#include <memory>
#include <string>

#include <httplib.h>

#include "emergency_triage/core/i_priority_calculator.hpp"
#include "emergency_triage/api/handlers.hpp"
#include "emergency_triage/services/patient_service.hpp"
#include "emergency_triage/storage/storage.hpp"
#include "spdlog/spdlog.h"

int main() {
	const char* host = std::getenv("SERVER_HOST") ? std::getenv("SERVER_HOST") : "127.0.0.1";
	int port = std::getenv("SERVER_PORT") ? std::stoi(std::getenv("SERVER_PORT")) : 8080;
	std::string triageMethod = std::getenv("TRIAGE_METHOD") ? std::getenv("TRIAGE_METHOD") : "SORT";
	std::unique_ptr<emergency_triage::IPriorityCalculator> calculator =
		emergency_triage::createPriorityCalculator(triageMethod);

	emergency_triage::PatientStorage storage;
	emergency_triage::PatientService patientService(storage, *calculator);
	httplib::Server server;
	setupHandlers(server, patientService);
	spdlog::info("Server started on {}:{}", host, port);
	server.listen(host, port);
	return 0;
}
