// #pragma once
// #include "IWebSocketClient.h"
// #include "JsonParser.h"
// #include "IPriceSubscriber.h"
// #include <iostream>

// class WebSocketPriceObserver : public IWebSocketObserver {
// public:
//     WebSocketPriceObserver(const std::string& symbol, IPriceSubscriber* subscriber)
//         : symbol(symbol), subscriber(subscriber) {}

//     void onMessageReceived(const std::string& message) override {
//         try {
//             float price = JsonParser::parse(message)["price"].get<float>();
//             subscriber->onPriceUpdate(symbol, price);
//         } catch (const std::exception& e) {
//             std::cerr << "Erreur lors du parsing du message WebSocket: " << e.what() << std::endl;
//         }
//     }

//     void onConnectionClosed() override {
//         std::cerr << "Connexion WebSocket fermée pour le symbole: " << symbol << std::endl;
//     }

// private:
//     std::string symbol;
//     IPriceSubscriber* subscriber;
// };