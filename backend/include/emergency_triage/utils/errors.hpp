#pragma once

#include <format>
#include <stdexcept>
#include <string>

namespace emergency_triage {

class Exception : public std::runtime_error {
public:
	explicit Exception(const std::string& message) : std::runtime_error(message) {}
	virtual int httpStatus() const = 0;
	virtual bool isShowToClient() const = 0;
};

class ClientError : public Exception {
public:
	explicit ClientError(const std::string& message) : Exception(message) {}
	int httpStatus() const override {
		return 400;
	}
	bool isShowToClient() const override {
		return true;
	}
};

class ValidationError : public ClientError {
public:
	ValidationError(const std::string &field, const std::string &message)
		: ClientError(std::format("Field '{}': {}", field, message)) {}
	int httpStatus() const override {
		return 400;
	}
};

class NotFoundError : public ClientError {
public:
	NotFoundError(const std::string& resourse, const std::string& id)
		: ClientError(std::format("{} with id '{}' not found", resourse, id)) {}
	int httpStatus() const override {
		return 404;
	}
};

class ConflictError : public ClientError {
public:
	explicit ConflictError(const std::string& message) : ClientError(std::format("Conflict: {}", message)) {
	}
	int httpStatus() const override {
		return 409;
	}
};

class ServerError : public Exception {
public:
	explicit ServerError(const std::string& message) : Exception(message) {}
	int httpStatus() const override {
		return 500;
	}
	bool isShowToClient() const override {
		return false;
	}
};

class BusinessLogicError : public ServerError {
public:
	explicit BusinessLogicError(const std::string& message) : ServerError(std::format("Business logic error: {}", message)) {
	}
	int httpStatus() const override {
		return 500;
	}
};
}  // namespace emergency_triage
