#include <iostream>
#include "ConfigLoader.h"
#include "FileHandler.h"
#include "Timer.h"
#include "JsonParser.h"

int main() {
    try {
        Timer timer;

        nlohmann::json configToWrite = {
            {"app_name", "MyTestApp"},
            {"version", 2},
            {"features", {"feature1", "feature2", "feature3"}}
        };

        std::string configFilePath = "test_config.json";

        std::cout << "Écriture du fichier de configuration : " << configFilePath << std::endl;
        FileHandler::writeFile(configFilePath, JsonParser::toJsonString(configToWrite));

        std::cout << "Temps écoulé pour écrire le fichier : " << timer.elapsedMilliseconds() << " ms" << std::endl;

        timer.reset();

        std::cout << "Chargement de la configuration à partir de : " << configFilePath << std::endl;
        auto config = ConfigLoader::loadConfig(configFilePath);

        std::string appName = ConfigLoader::getConfigValue(config, "app_name", std::string("DefaultAppName"));
        std::cout << "Nom de l'application : " << appName << std::endl;

        int version = ConfigLoader::getConfigValue(config, "version", 1);
        std::cout << "Version de l'application : " << version << std::endl;

        auto features = ConfigLoader::getConfigValue(config, "features", nlohmann::json::array());
        std::cout << "Fonctionnalités disponibles : ";
        for (const auto& feature : features) {
            std::cout << feature.get<std::string>() << " ";
        }
        std::cout << std::endl;

        std::cout << "Temps écoulé pour charger le fichier : " << timer.elapsedMilliseconds() << " ms" << std::endl;

    } catch (const std::exception& ex) {
        std::cerr << "Erreur : " << ex.what() << std::endl;
        return 1;
    }

    int a;
    std::cin >> a;
    return 0;
}
