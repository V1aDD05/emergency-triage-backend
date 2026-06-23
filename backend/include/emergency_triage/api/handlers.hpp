#pragma once

#include <exception>

#include <httplib.h>

#include "emergency_triage/storage/storage.hpp"

namespace emergency_triage {

void setupHandlers(httplib::Server &server, IPatientStorage &storage);

void commonExceptionHandler(const httplib::Request &req, httplib::Response &res, std::exception_ptr ep);

void handlePostPatients(const httplib::Request &req, httplib::Response &res, IPatientStorage &storage);

void handleGetPatientById(const httplib::Request &req, httplib::Response &res, const IPatientStorage &storage);

void handleGetPatients(const httplib::Request &req, httplib::Response &res, const IPatientStorage &storage);

}
