#include <iostream>

#include <httplib.h>

#include "api/handlers.hpp"
#include "storage/storage.hpp"

int main() {
    PatientStorage storage;
    httplib::Server server;
    setupHandlers(server, storage);
    std::cout << "Server started on http://localhost:8080" << std::endl;
    server.listen("0.0.0.0", 8080);
    return 0;
}
