#pragma once
#include "IExchangeAPI.h"
#include "NetworkManager.h"

class BinanceAPI : public IExchangeAPI {
public:
    BinanceAPI(NetworkManager& networkManager) : networkManager(networkManager) {}

    float getPrice(const std::string& symbol) override {
        std::string url = "https://api.binance.com/api/v3/ticker/price?symbol=" + symbol;
        HttpResponse response = networkManager.sendRequest(HttpRequest(url));
        return JsonParser::parse(response.getBody())["price"].get<float>();
    }

    bool supportsWebSocket() const override {
        return true;  // Binance supporte WebSocket
    }

    void subscribeToWebSocket(const std::string& symbol, MarketDataProvider* provider) override {
        networkManager.connectWebSocket(symbol, [provider, symbol](const std::string& message) {
            float price = JsonParser::parse(message)["price"].get<float>();
            provider->onPriceUpdate(symbol, price);
        });
    }

private:
    NetworkManager& networkManager;
};