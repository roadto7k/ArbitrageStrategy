#pragma once
#include <iostream>
#include "IExchangeAPI.h"
#include "NetworkManager.h"
#include "JsonParser.h"

class BinanceAPI : public IExchangeAPI {
public:
    BinanceAPI(NetworkManager& networkManager) : networkManager(networkManager) {}

    float getPrice(const std::string& symbol) override;
    // {
    //     std::string url = "https://api.binance.com/api/v3/ticker/price?symbol=" + symbol;
    //     HttpResponse response = networkManager.makeHttpRequest(HttpRequest(url));
    //     return JsonParser::parse(response.getBody())["price"].get<float>();
    // }

    bool supportsWebSocket() const override {
        return true;  // Binance supporte WebSocket
    }

    void sendOrder(const Order& order) override;

    void subscribeToWebSocket(const std::string& symbol, IPriceSubscriber* provider) override {
        networkManager.connectWebSocket(symbol, [provider, symbol](const std::string& message) {
            float price = JsonParser::parse(message)["price"].get<float>();
            provider->onPriceUpdate(symbol, price);
        });
    }

private:
    NetworkManager& networkManager;

    std::string buildUrl(const std::string& endpoint) const;
};