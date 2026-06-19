#pragma once

#include <exception>

#include <httplib.h>

#include "emergency_triage/storage/storage.hpp"

namespace emergency_triage {

void setupHandlers(httplib::Server &server, PatientStorage &storage);

void commonExceptionHandler(const httplib::Request &req, httplib::Response &res, std::exception_ptr ep);

void handlePostPatients(const httplib::Request &req, httplib::Response &res, PatientStorage &storage);

void handleGetPatientById(const httplib::Request &req, httplib::Response &res, const PatientStorage &storage);

void handleGetPatients(const httplib::Request &req, httplib::Response &res, const PatientStorage &storage);

}
