#pragma once

#include <exception>

#include <httplib.h>

#include "emergency_triage/services/patient_service.hpp"
#include "emergency_triage/storage/storage.hpp"

namespace emergency_triage {

void setupHandlers(httplib::Server &server, PatientService &patientService);

void commonExceptionHandler(const httplib::Request &req, httplib::Response &res, std::exception_ptr ep);

void handlePostPatients(const httplib::Request &req, httplib::Response &res, PatientService& patientService);

void handleGetPatientById(const httplib::Request &req, httplib::Response &res, PatientService &patientService);

void handleGetPatients(const httplib::Request &req, httplib::Response &res, PatientService &patientService);
}
