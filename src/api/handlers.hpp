#pragma once

#include <httplib.h>                    
#include <nlohmann/json.hpp>            

#include "storage/storage.hpp"          
#include "core/triage.hpp"              
#include "utils/errors.hpp"             
#include "utils/json_utils.hpp"

void setupHandlers(httplib::Server &server, PatientStorage &storage);

void handlePostPatients(const httplib::Request& req, httplib::Response& res, PatientStorage& storage);

void handleGetPatientById(const httplib::Request &req, httplib::Response &res, const PatientStorage &storage);

void handleGetAllPatients(const httplib::Request &req, httplib::Response &res, const PatientStorage &storage);
