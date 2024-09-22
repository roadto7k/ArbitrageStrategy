#pragma once
#include <nlohmann/json.hpp>
#include <string>

class JsonParser {
public:
    static nlohmann::json parse(const std::string& jsonString) {
        return nlohmann::json::parse(jsonString);
    }

    template<typename T>
    static T getValue(const nlohmann::json& json, const std::string& key, const T& defaultValue = T()) {
        if (json.contains(key)) {
            return json.at(key).get<T>();
        } else {
            return defaultValue;
        }
    }

    template<typename T>
    static std::string toJsonString(const T& object) {
        return nlohmann::json(object).dump();
    }
};