#pragma once
#include <string>
#include "JsonParser.hpp"
#include <fstream>

class ConfigLoader {
public:
    static nlohmann::json loadConfig(const std::string& filePath) {
        std::ifstream file(filePath);
        if (!file.is_open()) {
            throw std::runtime_error("Impossible d'ouvrir le fichier de configuration : " + filePath);
        }

        std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        return JsonParser::parse(content);
    }

    template<typename T>
    static T getConfigValue(const nlohmann::json& config, const std::string& key, const T& defaultValue = T()) {
        return JsonParser::getValue(config, key, defaultValue);
    }
};