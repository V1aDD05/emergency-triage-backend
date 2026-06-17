#include "json_utils.hpp"

std::optional<Gender> deserialiseGender(const nlohmann::json &json, const std::string &key) {
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

nlohmann::json serialiseJSON(const Patient &patient) {
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

nlohmann::json serialiseJSON(const std::optional<Patient> &patient) {
    if (!patient.has_value()) {
        return nlohmann::json();
    }
    return serialiseJSON(*patient);
}
