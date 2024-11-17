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
        try {
            auto json = JsonParser::parse(message);
            
            if (json.contains("stream") && json.contains("data")) {
                auto data = json["data"];
                processMessage(data);
            }
            else {
                processMessage(json);
            }
        } catch (const std::exception& e) {
            std::cerr << "Error parsing JSON message: " << e.what() << std::endl;
        }
    }

    void onConnectionClosed() override {
        std::cout << "Connection closed for symbol: " << symbol << std::endl;
    }

private:
    std::string symbol;
    IPriceSubscriber* provider;

    void processMessage(const nlohmann::json& json) {
        if (json.contains("b")) {
            auto truc = JsonParser::getValue<std::string>(json, "b", "");
            float price = std::stod(JsonParser::getValue<std::string>(json, "b", ""));
            provider->onPriceUpdate(symbol, price);
        } else {
            std::cerr << "Price field 'p' not found in the message." << std::endl;
        }
    }
};