#include <iostream>

#include <httplib.h>

#include "emergency_triage/api/handlers.hpp"
#include "emergency_triage/storage/storage.hpp"

int main() {
	emergency_triage::PatientStorage storage;
	httplib::Server server;
    setupHandlers(server, storage);
    std::cout << "Server started on http://localhost:8080" << std::endl;
    server.listen("0.0.0.0", 8080);
    return 0;
}
