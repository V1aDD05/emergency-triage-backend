#pragma once

#include <chrono>
#include <cstdint>
#include <optional>

#include "emergency_triage/utils/errors.hpp"
#include "nlohmann/json.hpp"

namespace emergency_triage {

enum class Gender : uint8_t { Male, Female };

enum class PatientStatus : uint8_t { OnTheWay, Waiting, InSurgery, IntensiveCare, Died };

struct EmergencyData {
	bool is_bleeding = false;
	bool is_extensive_wounds = false;
	bool is_penetrating_wounds = false;
	bool is_consciousness_depression = false;
	bool is_respiratory_depression = false;
	bool is_hemodynamic_depression = false;
	bool is_severe_combined_injury = false;
};

class TriageData {
public:
	TriageData(uint8_t eye_response, uint8_t verbal_response, uint8_t motor_response, uint8_t respiratory_rate,
			   uint16_t systolic_bp)
		: eye_response_(eye_response),
		  verbal_response_(verbal_response),
		  motor_response_(motor_response),
		  respiratory_rate_(respiratory_rate),
		  systolic_bp_(systolic_bp) {
		validateAll();
	}

	uint8_t getEyeResponse() const noexcept {
		return eye_response_;
	}

	uint8_t getVerbalResponse() const noexcept {
		return verbal_response_;
	}

	uint8_t getMotorResponse() const noexcept {
		return motor_response_;
	}

	uint8_t getRespiratoryRate() const noexcept {
		return respiratory_rate_;
	}

	uint8_t getSystolicBP() const noexcept {
		return systolic_bp_;
	}

	void setEyeResponse(uint8_t eye_response) {
		validateEyeResponse(eye_response);
		eye_response_ = eye_response;
	}

	void setVerbalResponse(uint8_t verbal_response) {
		validateVerbalResponse(verbal_response);
		verbal_response_ = verbal_response;
	}

	void setMotorResponse(uint8_t motor_response) {
		validateMotorResponse(motor_response);
		motor_response_ = motor_response;
	}

	void setRespiratoryRate(uint8_t respiratory_rate) {
		validateRespiratoryRate(respiratory_rate);
		respiratory_rate_ = respiratory_rate;
	}

	void setSystolicBP(uint16_t systolic_bp) {
		validateSystolicBP(systolic_bp);
		systolic_bp_ = systolic_bp;
	}

private:
	void validateEyeResponse(uint8_t eye_response) const {
		if (eye_response < 1 || eye_response > 4) {
			throw ValidationError("eye_response", "Must be in range [1..4]");
		}
	}
	void validateVerbalResponse(uint8_t verbal_response) const {
		if (verbal_response < 1 || verbal_response > 5) {
			throw ValidationError("verbal_response", "Must be in range [1..5]");
		}
	}
	void validateMotorResponse(uint8_t motor_response) const {
		if (motor_response < 1 || motor_response > 6) {
			throw ValidationError("motor_response", "Must be in range [1..6]");
		}
	}
	void validateRespiratoryRate(uint8_t respiratory_rate) const {
		if (respiratory_rate > 60) {
			throw ValidationError("respiratory_rate", "Must be in range [0..60]");
		}
	}
	void validateSystolicBP(uint16_t systolic_bp) const {
		if (systolic_bp > 300) {
			throw ValidationError("systolic_bp", "Must be in range [0..300]");
		}
	}

	void validateAll() const {
		validateEyeResponse(eye_response_);
		validateVerbalResponse(verbal_response_);
		validateMotorResponse(motor_response_);
		validateRespiratoryRate(respiratory_rate_);
		validateSystolicBP(systolic_bp_);
	}

	uint8_t eye_response_;
	uint8_t verbal_response_;
	uint8_t motor_response_;
	uint8_t respiratory_rate_;
	uint16_t systolic_bp_;
};

class DemographicData {
public:
	DemographicData(std::optional<uint8_t> age, std::optional<Gender> sex) : age_(age), sex_(sex) {
		validateAll();
	}

	std::optional<uint8_t> getAge() const noexcept {
		return age_;
	}
	std::optional<Gender> getSex() const noexcept {
		return sex_;
	}

	void setAge(std::optional<uint8_t> age) {
		validateAge(age);
		age_ = age;
	}
	void setSex(std::optional<Gender> sex) {
		validateSex(sex);
		sex_ = sex;
	}

private:
	void validateAge(std::optional<uint8_t> age) const {
		if (age.has_value() && *age > 130) {
			throw ValidationError("age", "Must be in range [0..130]");
		}
	}
	void validateSex(std::optional<Gender> sex) const {
		if (sex.has_value() && *sex != Gender::Male && *sex != Gender::Female) {
			throw ValidationError("sex", "Invalid gender value" );
		}
	}
	void validateAll() const {
		validateAge(age_);
		validateSex(sex_);
	}

	std::optional<uint8_t> age_;
	std::optional<Gender> sex_;
};

struct PatientClientData {
	EmergencyData emergency_data;
	TriageData triage_data;
	DemographicData demographic_data;
};

}  // namespace emergency_triage
