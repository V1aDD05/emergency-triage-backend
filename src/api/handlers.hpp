#pragma once

#include <httplib.h>
#include <nlohmann/json.hpp>

#include "core/triage.hpp"
#include "storage/storage.hpp"
#include "utils/errors.hpp"
#include "utils/json_serialisation.hpp"

void setupHandlers(httplib::Server &server, PatientStorage &storage);

void commonExceptionHandler(const httplib::Request &req, httplib::Response &res, std::exception_ptr ep);

void handlePostPatients(const httplib::Request &req, httplib::Response &res, PatientStorage &storage);

void handleGetPatientById(const httplib::Request &req, httplib::Response &res, const PatientStorage &storage);

void handleGetPatients(const httplib::Request &req, httplib::Response &res, const PatientStorage &storage);
