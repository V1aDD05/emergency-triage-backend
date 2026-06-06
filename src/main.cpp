#include <iostream>
#include <httplib.h>
#include "api/handlers.hpp"
#include "storage/storage.hpp"

int main()
{
    PatientStorage storage;
    httplib::Server svr;
    setupHandlers(svr, storage);
    std::cout << "Server started on http://localhost:8080" << std::endl;
    svr.listen("0.0.0.0", 8080);
    return 0;
}