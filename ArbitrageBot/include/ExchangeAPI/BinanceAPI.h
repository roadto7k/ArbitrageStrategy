#pragma once
#include <iostream>
#include "IExchangeAPI.h"
#include "NetworkManager.h"
#include "JsonParser.h"
#include "WebSocketPriceObserver.h"

class BinanceAPI : public IExchangeAPI {
public:
    BinanceAPI(NetworkManager& networkManager) : networkManager(networkManager) {}

    float getPrice(const std::string& symbol) override;


    bool supportsWebSocket() const override {
        return true;  // Binance supporte WebSocket
    }

    void sendOrder(const Order& order) override;

    void subscribeToWebSocket(const std::string& symbol, IPriceSubscriber* provider) {
        auto observer = std::make_unique<WebSocketPriceObserver>(symbol, provider);
        networkManager.connectWebSocket(symbol, observer.get());
        observers.push_back(std::move(observer));
    }

private:
    NetworkManager& networkManager;
    std::vector<std::unique_ptr<WebSocketPriceObserver>> observers;
    std::string buildUrl(const std::string& endpoint) const;
};