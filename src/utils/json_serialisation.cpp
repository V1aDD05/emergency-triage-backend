#include "json_serialisation.hpp"

#include <chrono>
#include <format>
#include <limits>
#include <string>
#include <type_traits>

#include "utils/errors.hpp"

namespace {

    bool deserialiseBoolField(const nlohmann::json& json, const std::string& key) {
        bool defaultValue = false;

        if (!json.contains(key)) {
            throw ValidationError(key, "Missing required field");
        }
        const auto& value = json[key];
        if (value.is_null()) {
            throw ValidationError(key, "Must be either `true` or `false`");
        }
        if (!value.is_boolean()) {
            throw ValidationError(key, "Must be a boolean");
        }
        return value.get<bool>();
    }

    template <typename T>

    T deserialiseUnsignedField(const nlohmann::json& json, const std::string& key) {
        static_assert(std::is_unsigned<T>::value, "T must be an unsigned integer type");

        if (!json.contains(key)) {
            throw ValidationError(key, "Missing required field");
        }

        if (!json[key].is_number_unsigned()) {
            throw ValidationError(key, "Must be a non-negative integer");
        }

        uint64_t raw = json[key].get<uint64_t>();
        if (raw > std::numeric_limits<T>::max()) {
            throw ValidationError(key, std::format("Must be in range of (0..{})", std::numeric_limits<T>::max()));
        }

        return static_cast<T>(raw);
    }

    std::optional<uint8_t> deserialiseAge(const nlohmann::json& json, const std::string& key) {
        std::optional<uint8_t> result = std::nullopt;
        if (json.contains(key) && !json[key].is_null()) {
            if (!json[key].is_number_unsigned()) {
                throw ValidationError(key, "Must be a non-negative integer");
            }

            uint64_t age_raw = json[key].get<uint64_t>();
            if (age_raw > std::numeric_limits<uint8_t>::max()) {
                throw ValidationError(key, "Must be in range of <uint8_t> (0..255)");
            }

            result = static_cast<uint8_t>(age_raw);
        }
        return result;
    }

    std::optional<Gender> deserialiseGender(const nlohmann::json& json, const std::string& key) {
        if (!json.contains(key) || json[key].is_null()) {
            return std::nullopt;
        }

        if (!json[key].is_string()) {
            throw ValidationError(key, "Must be a string");
        }

        std::string str = json[key];
        if (str == "male") {
            return Gender::Male;
        }
        if (str == "female") {
            return Gender::Female;
        }

        throw ValidationError(key, "Invalid gender value: " + str);
    }
}

EmergencyParams deserialiseEmergencyParams(const nlohmann::json& json){
    EmergencyParams result;
    result.is_bleeding = deserialiseBoolField(json, "is_bleeding");
    result.is_consciousness_depression = deserialiseBoolField(json, "is_consciousness_depression");
    result.is_extensive_wounds = deserialiseBoolField(json, "is_extensive_wounds");
    result.is_hemodynamic_depression = deserialiseBoolField(json, "is_hemodynamic_depression");
    result.is_penetrating_wounds = deserialiseBoolField(json, "is_penetrating_wounds");
    result.is_respiratory_depression = deserialiseBoolField(json, "is_respiratory_depression");
    result.is_severe_combined_injury = deserialiseBoolField(json, "is_severe_combined_injury");
    return result;
}

TriageData deserialiseTriageData(const nlohmann::json& json){
    TriageData result;
    result.eye_response = deserialiseUnsignedField<uint8_t>(json, "eye_response");
    result.motor_response = deserialiseUnsignedField<uint8_t>(json, "motor_response");
    result.verbal_response = deserialiseUnsignedField<uint8_t>(json, "verbal_response");
    result.respiratory_rate = deserialiseUnsignedField<uint8_t>(json, "respiratory_rate");
    result.systolic_bp = deserialiseUnsignedField<uint16_t>(json, "systolic_bp");
    return result;
}

DemographyData deserialiseDemographyData(const nlohmann::json& json){
    DemographyData result;
    result.age = deserialiseAge(json, "age");
    result.sex = deserialiseGender(json, "sex");
    return result;
}

uint32_t deserialiseID(const httplib::Request& req) {
    std::string id_str = req.matches[1];

    if (id_str.empty() || id_str[0] == '-') {
        throw ValidationError("id", "Must be a non-negative integer");
    }

    size_t pos;
    auto id_raw = std::stoull(id_str, &pos);
    if (pos != id_str.size()) {
        throw ValidationError("id", "Must contain only digits (no trailing characters)");
    }

    if (id_raw > std::numeric_limits<uint32_t>::max()) {
        throw ValidationError("id", "Must be in range of <uint32_t> (0..4294967295)");
    }
    return static_cast<uint32_t>(id_raw);
}

namespace {

    nlohmann::json serialiseEmergencyParams(const EmergencyParams& emeregencyParams){
        nlohmann::json json;
        json["is_bleeding"] = emeregencyParams.is_bleeding;
        json["is_extensive_wounds"] = emeregencyParams.is_extensive_wounds;
        json["is_penetrating_wounds"] = emeregencyParams.is_penetrating_wounds;
        json["is_consciousness_depression"] = emeregencyParams.is_consciousness_depression;
        json["is_respiratory_depression"] = emeregencyParams.is_respiratory_depression;
        json["is_hemodynamic_depression"] = emeregencyParams.is_hemodynamic_depression;
        json["is_severe_combined_injury"] = emeregencyParams.is_severe_combined_injury;
        return json;
    }

    nlohmann::json serialiseTriageData(const TriageData& triageData) {
        nlohmann::json json;
        json["eye_response"] = triageData.eye_response;
        json["verbal_response"] = triageData.verbal_response;
        json["motor_response"] = triageData.motor_response;
        json["respiratory_rate"] = triageData.respiratory_rate;
        json["systolic_bp"] = triageData.systolic_bp;
        return json;
    }

    nlohmann::json serialiseDemographyData(const DemographyData& demographyData) {
        nlohmann::json json;
        if (demographyData.age.has_value()) {
            json["age"] = *demographyData.age;
        }
        if (demographyData.sex.has_value()) {
            json["sex"] = (*demographyData.sex == Gender::Male) ? "male" : "female";
        }
        return json;
    }
}

nlohmann::json serialiseJSON(const Patient& patient) {
    nlohmann::json json;
    json["id"] = patient.getId();
    json["priority"] = patient.getPriority();
    json["timestamp"] =
        std::chrono::duration_cast<std::chrono::milliseconds>(patient.getTimestamp().time_since_epoch()).count();

    switch (patient.getStatus()) {
        case PatientStatus::OnTheWay:
            json["status"] = "on_the_way";
            break;
        case PatientStatus::Waiting:
            json["status"] = "waiting";
            break;
        case PatientStatus::InSurgery:
            json["status"] = "in_surgery";
            break;
        case PatientStatus::IntensiveCare:
            json["status"] = "intensive_care";
            break;
        case PatientStatus::Died:
            json["status"] = "died";
            break;
    }

    json["medical_data"]["emeregency_params"] = serialiseEmergencyParams(patient.getEmergencyParams());
    json["medical_data"]["triage_data"] = serialiseTriageData(patient.getTriageData());
    json["medical_data"]["demography_data"] = serialiseDemographyData(patient.getDemographyData());

    return json;
}

nlohmann::json serialiseJSON(const std::optional<Patient>& patient) {
    if (!patient.has_value()) {
        return nlohmann::json();
    }
    return serialiseJSON(*patient);
}
