#include <iostream>
#include <memory>
#include "ArbitrageEngine.h"
#include "Istrategy.h"
#include "BinanceAPI.h"
#include "NetworkManager.h"
#include "OrderProcessor.h"
#include "TriangleArbitrageStrategy.h"
#include "Order.h"
#include <cctype>

std::string toLower(const std::string& input) {
    std::string result = input;
    for (char& c : result) {
        c = std::tolower(c);
    }
    return result;
}


int main() {
    // try {
    //     // Initialiser le NetworkManager
    //     NetworkManager networkManager;

    //     // Liste des symboles à surveiller
    //     std::vector<std::string> symbols = {"BTCUSDT", "ETHUSDT", "BNBUSDT"};

    //     // Configurer les clients WebSocket pour chaque symbole
    //     for (const auto& symbol : symbols) {
    //         auto webSocketClient = std::make_unique<WebSocketClientWebSocketPP>(
    //             "wss://stream.binance.com/ws/" + toLower(symbol) + "@bookTicker"
    //         );
    //         networkManager.addWebSocketClient(symbol, std::move(webSocketClient));
    //     }

    //     // Initialiser l'API Binance avec le gestionnaire de réseau
    //     auto exchangeAPI = std::make_shared<BinanceAPI>(networkManager);

    //     // Créer une stratégie d'arbitrage triangulaire
    //     auto strategy = std::make_unique<TriangleArbitrageStrategy>("BTCUSDT", "ETHUSDT", "BNBUSDT");

    //     // Créer le moteur d'arbitrage avec la stratégie et l'API
    //     ArbitrageEngine engine(std::move(strategy), exchangeAPI);

    //     // S'abonner aux symboles pour surveiller les mises à jour des prix
    //     engine.subscribeToSymbols(symbols);

    //     std::cout << "Arbitrage engine is running. Monitoring symbols for opportunities..." << std::endl;

    //     // Simuler un fonctionnement continu avec un mécanisme de contrôle
    //     while (true) {
    //         // Ajoutez un mécanisme pour interrompre ou redémarrer proprement si nécessaire
    //         std::this_thread::sleep_for(std::chrono::seconds(10));
    //     }

    // } catch (const std::exception& e) {
    //     std::cerr << "Error: " << e.what() << std::endl;
    //     return 1;
    // }

    return 0;
}