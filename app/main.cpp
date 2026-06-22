#include <iostream>

#include <httplib.h>

#include "spdlog/spdlog.h"
#include "emergency_triage/api/handlers.hpp"
#include "emergency_triage/storage/storage.hpp"

int main() {
	const char* host = std::getenv("SERVER_HOST") ? std::getenv("SERVER_HOST") : "127.0.0.1";
	int port = std::getenv("SERVER_PORT") ? std::stoi(std::getenv("SERVER_PORT")) : 8080;
	emergency_triage::PatientStorage storage;
	httplib::Server server;
    setupHandlers(server, storage);
	spdlog::info("Server started on {}:{}", host, port);
	server.listen(host, port);
	return 0;
}
