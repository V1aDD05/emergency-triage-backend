#include "json_utils.hpp"

uint32_t deserialiseMask(const nlohmann::json& json, const std::string& key) {
    if (!json.contains(key)) {
        throw ValidationError(key, "Missing required field");
    }

    if (!json[key].is_number_unsigned()) {
        throw ValidationError(key, "Must be a non-negative integer");
    }

    uint64_t mask_raw = json[key].get<uint64_t>();
    if (mask_raw > std::numeric_limits<uint32_t>::max()) {
        throw ValidationError(key, "Must be in range of <uint32_t> (0..4294967295)");
    }

    return static_cast<uint32_t>(mask_raw);
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
        throw ValidationError(key, "must be a string");
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

uint32_t deserialiseID(const httplib::Request& req) {
    std::string id_str = req.matches[1];

    if (id_str.empty() || id_str[0] == '-') {
        throw ValidationError("id", "Must be a non-negative integer");
    }

    size_t pos;
    auto id_raw = std::stoull(id_str, &pos);
    if (pos != id_str.size()) {
        throw ValidationError("id", "Must contain only digits(no trailing characters)");
    }

    if (id_raw > std::numeric_limits<uint32_t>::max()) {
        throw ValidationError("id", "Must be in range of <uint32_t> (0..4294967295)");
    }
    return static_cast<uint32_t>(id_raw);
}

nlohmann::json serialiseJSON(const Patient& patient) {
    nlohmann::json json;
    json["id"] = patient.id_;
    json["mask"] = patient.mask_;
    json["priority"] = patient.priority_;
    json["timestamp"] =
        std::chrono::duration_cast<std::chrono::milliseconds>(patient.timestamp_.time_since_epoch()).count();

    switch (patient.status_) {
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

    if (patient.age_.has_value()) {
        json["age"] = *patient.age_;
    }
    if (patient.sex_.has_value()) {
        json["sex"] = (*patient.sex_ == Gender::Male) ? "male" : "female";
    }

    return json;
}

nlohmann::json serialiseJSON(const std::optional<Patient>& patient) {
    if (!patient.has_value()) {
        return nlohmann::json();
    }
    return serialiseJSON(*patient);
}
