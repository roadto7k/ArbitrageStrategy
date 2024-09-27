#include <iostream>
#include "MarketDataProvider.h"
#include "MarketDataCache.h"
#include "IPriceSubscriber.h"
#include "IAPI.h"

class PriceDisplaySubscriber : public IPriceSubscriber {
public:
    PriceDisplaySubscriber(MarketDataCache& cache) : cache(cache) {}

    void onPriceUpdate(const std::string& symbol, float price) override {
        std::cout << "Price update for " << symbol << ": " << price << std::endl;
        cache.storePrice(symbol, price); 
    }

private:
    MarketDataCache& cache;
};

class MockAPI : public IAPI {

    float getPrice(const std::string& symbol) override {return 1.;};
    bool supportsWebSocket() const override {return false;};
    void subscribeToWebSocket(const std::string& symbol, IPriceSubscriber* subscriber) override {};
};
void simulatePriceUpdates(MarketDataProvider& provider) {
    provider.onPriceUpdate("BTCUSDT", 45000.0f);
    provider.onPriceUpdate("ETHUSDT", 3000.0f);
}

int main() {
    std::cout << "Getting cache" << std::endl;
    MarketDataCache cache;

    std::cout << "Getting PriceDisplaySubscriber" << std::endl;
    PriceDisplaySubscriber displaySubscriber(cache);

    std::cout << "Accessing MarketDataProviderInstance" << std::endl;
    MarketDataProvider& provider = MarketDataProvider::getInstance();

    std::string symbol1 = "BTCUSDT";
    std::string symbol2 = "ETHUSDT";
    MockAPI api;
    IAPI *fakeapi = &api;
    std::cout << "Subscribing to BTC and ETH" << std::endl;
    provider.subscribe(symbol1, &displaySubscriber, fakeapi);
    provider.subscribe(symbol2, &displaySubscriber, fakeapi);

    simulatePriceUpdates(provider);

    try {
        std::cout << "Cached price for " << symbol1 << ": " << cache.getPrice(symbol1) << std::endl;
        std::cout << "Cached price for " << symbol2 << ": " << cache.getPrice(symbol2) << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    int a;
    std::cin >> a;
    return 0;
}
