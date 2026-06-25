#pragma once

#include <exception>
#include <memory>

#include <httplib.h>

#include "emergency_triage/services/patient_service.hpp"

namespace emergency_triage {

class Router {
public:
	explicit Router(std::shared_ptr<PatientService> patientService);

	void setup(httplib::Server &server);

private:
	void commonExceptionHandler(const httplib::Request &req, httplib::Response &res, std::exception_ptr ep);

	void handlePostPatients(const httplib::Request &req, httplib::Response &res);

	void handleGetPatientById(const httplib::Request &req, httplib::Response &res);

	void handleGetPatients(const httplib::Request &req, httplib::Response &res);

	std::shared_ptr<PatientService> patientService_;
}; 

}
