#pragma once
#include <string>
#include <fstream>

class FileHandler {
public:
    static std::string readFile(const std::string& filePath) {
        std::ifstream file(filePath);
        if (!file.is_open()) {
            throw std::runtime_error("Impossible d'ouvrir le fichier : " + filePath);
        }

        return std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    }

    static void writeFile(const std::string& filePath, const std::string& content) {
        std::ofstream file(filePath);
        if (!file.is_open()) {
            throw std::runtime_error("Impossible d'ouvrir le fichier pour écriture : " + filePath);
        }

        file << content;
    }
};