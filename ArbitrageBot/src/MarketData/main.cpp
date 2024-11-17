#include <iostream>
#include "MarketDataProvider.h"
#include "MarketDataCache.h"
#include "IPriceSubscriber.h"
#include "BinanceAPI.h"
#include "NetworkManager.h"
#include "PriceDisplaySubscriber.h"
#include "HttpClient.h"
#include "WebSocketClientWebSocketPP.h"

class MockAPI : public IAPI {

    float getPrice(const std::string& symbol) override {return 1.;};
    bool supportsWebSocket() const override {return false;};
    void subscribeToWebSocket(const std::string& symbol, IPriceSubscriber* subscriber) override {};
};
void simulatePriceUpdates(MarketDataProvider& provider) {
    provider.onPriceUpdate("BTCUSDT", 45000.0f);
    provider.onPriceUpdate("ETHUSDT", 3000.0f);
}

// int main() {
//     std::cout << "Getting cache" << std::endl;
//     MarketDataCache cache;

//     std::cout << "Getting PriceDisplaySubscriber" << std::endl;
//     PriceDisplaySubscriber displaySubscriber(cache);

//     std::cout << "Accessing MarketDataProviderInstance" << std::endl;
//     MarketDataProvider& provider = MarketDataProvider::getInstance();

//     std::string symbol1 = "BTCUSDT";
//     std::string symbol2 = "ETHUSDT";
//     MockAPI api;
//     IAPI *fakeapi = &api;
//     std::cout << "Subscribing to BTC and ETH" << std::endl;
//     provider.subscribe(symbol1, &displaySubscriber, fakeapi);
//     provider.subscribe(symbol2, &displaySubscriber, fakeapi);

//     simulatePriceUpdates(provider);

//     try {
//         std::cout << "Cached price for " << symbol1 << ": " << cache.getPrice(symbol1) << std::endl;
//         std::cout << "Cached price for " << symbol2 << ": " << cache.getPrice(symbol2) << std::endl;
//     } catch (const std::exception& e) {
//         std::cerr << "Error: " << e.what() << std::endl;
//     }
//     int a;
//     std::cin >> a;
//     return 0;
// }



int main() {
    try {
        auto httpClient = std::make_unique<CurlHttpClient>();
        // auto httpClient2 = std::make_unique<CurlHttpClient>();
        auto webSocketClient = std::make_unique<WebSocketClientWebSocketPP>("wss://stream.binance.com/ws/btcusdt@bookTicker");
        auto webSocketClient2 = std::make_unique<WebSocketClientWebSocketPP>("wss://stream.binance.com/ws/ethusdt@bookTicker");
        std::cout << "Initializing NetworkManager" << std::endl;
        NetworkManager networkManager("BTCUSDT", std::move(httpClient), std::move(webSocketClient));

        networkManager.addWebSocketClient("ETHUSDT", std::move(webSocketClient2));

        // NetworkManager networkManagerETHUSDT2("ETHUSDT", std::move(httpClient2), std::move(webSocketClient2));
        std::cout << "Initializing BinanceAPI" << std::endl;
        // BinanceAPI binanceAPI(networkManager);
        // BinanceAPI binanceAPI2(networkManager2);
        std::cout << "Getting cache" << std::endl;
        MarketDataCache cache;

        std::cout << "Getting PriceDisplaySubscriber" << std::endl;
        PriceDisplaySubscriber displaySubscriber(cache);

        std::cout << "Accessing MarketDataProviderInstance" << std::endl;
        MarketDataProvider& provider = MarketDataProvider::getInstance();

        std::string symbol1 = "BTCUSDT";
        std::string symbol2 = "ETHUSDT";
        
        std::cout << "Subscribing to BTC and ETH" << std::endl;
        std::shared_ptr<IAPI> ptr_binanceAPI = std::make_shared<BinanceAPI>(networkManager);

        provider.subscribe(symbol1, &displaySubscriber, ptr_binanceAPI);
        provider.subscribe(symbol2, &displaySubscriber, ptr_binanceAPI);

        std::this_thread::sleep_for(std::chrono::seconds(5));

        std::cout << "Fetching cached prices..." << std::endl;
        std::cout << "Cached price for " << symbol1 << ": " << cache.getPrice(symbol1) << std::endl;
        std::cout << "Cached price for " << symbol2 << ": " << cache.getPrice(symbol2) << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    
    std::cout << "wow" << std::endl;
    int a=0;
    while(a != 1) {
        std::cin >> a;
    }
    
    return 0;
}
