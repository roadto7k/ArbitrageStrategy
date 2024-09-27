#include <iostream>
#include <memory>
#include "ExchangeFactory.h"
#include "MockExchangeAPI.h"
#include "NetworkManager.h"
#include "Order.h"

class MockMarketDataProvider : public MarketDataProvider {
public:
    void onPriceUpdate(const std::string& symbol, float price) override {
        std::cout << "Price update for " << symbol << ": " << price << std::endl;
    }
};

int main() {
    NetworkManager networkManager;

    std::unique_ptr<IExchangeAPI> exchange = ExchangeFactory::createExchange("Binance", networkManager);

    if (!exchange) {
        std::cerr << "Error: Unsupported exchange type!" << std::endl;
        return 1;
    }

    std::string symbol = "BTCUSDT";
    try {
        float price = exchange->getPrice(symbol);
        std::cout << "The price of " << symbol << " is " << price << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Failed to get price: " << e.what() << std::endl;
    }

    Order order(symbol, 0.1f, price, Order::Type::Market);
    try {
        exchange->sendOrder(order);
        std::cout << "Order placed successfully!" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Failed to send order: " << e.what() << std::endl;
    }

    MockMarketDataProvider provider;
    if (exchange->supportsWebSocket()) {
        exchange->subscribeToWebSocket(symbol, &provider);
        std::cout << "Subscribed to WebSocket for " << symbol << std::endl;
    }

    return 0;
}
