#pragma once
#include "IWebSocketClient.h"
#include "IPriceSubscriber.h"
#include "JsonParser.h"
#include <string>

class WebSocketObserver : public IWebSocketObserver {
public:
    WebSocketObserver(const std::string& symbol, IPriceSubscriber* provider)
        : symbol(symbol), provider(provider) {}

    void onMessageReceived(const std::string& message) override {
        auto json = JsonParser::parse(message);
        float price = JsonParser::getValue<float>(json, "p", 0.0f); 
        provider->onPriceUpdate(symbol, price);
    }

    void onConnectionClosed() override {
        std::cout << "Connection closed for symbol: " << symbol << std::endl;
    }

private:
    std::string symbol;
    IPriceSubscriber* provider;
};