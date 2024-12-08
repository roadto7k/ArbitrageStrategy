#include <iostream>
#include <memory>
#include <thread>
#include <vector>
#include <map>
#include <string>
#include "ArbitrageEngine.h"
#include "TriangleArbitrageStrategyv2.h"
#include "BinanceAPI.h"
#include "NetworkManager.h"
#include "MarketDataProvider.h"
#include "WebSocketClientWebSocketPP.h"
#include "Wallet.h"

std::string toLower(const std::string& input) {
    std::string result = input;
    for (char& c : result) {
        c = std::tolower(c);
    }
    return result;
}

int main() {
    try {
        NetworkManager networkManager;

        std::vector<std::string> symbols = {"BTCETH", "ETHUSDT", "USDTBTC"};

        for (const auto& symbol : symbols) {
            auto webSocketClient = std::make_unique<WebSocketClientWebSocketPP>(
                "wss://stream.binance.com/ws/" + toLower(symbol) + "@bookTicker"
            );
            networkManager.addWebSocketClient(symbol, std::move(webSocketClient));
        }

        auto exchangeAPI = std::make_shared<BinanceAPI>(networkManager);

        Wallet wallet({
            {"BTC", 1.0f},   
            {"ETH", 10.0f}, 
            {"USDT", 1000.0f} 
        });

        auto strategy = std::make_unique<TriangleArbitrageStrategy>(0.01f);

        ArbitrageEngine engine(std::move(strategy), exchangeAPI, wallet);
        engine.set_symbols(symbols);
        engine.subscribeToSymbols(symbols);

        std::cout << "Arbitrage engine is running. Monitoring symbols for opportunities...\n";

        while (true) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
