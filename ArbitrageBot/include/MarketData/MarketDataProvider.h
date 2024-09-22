#include <string>
#include <map>
#include <set>
#include <vector>

#include "IPriceSubscriber.h"
#include "Http.h"
#include "NetworkManager.h"
#include "JsonParser.h"

class MarketDataProvider {
public:
    static MarketDataProvider& getInstance() {
        static MarketDataProvider instance;
        return instance;
    }

    void subscribe(const std::string& symbol, IPriceSubscriber* subscriber, IExchangeAPI* exchangeApi) {
        subscribers[symbol].push_back(subscriber);
        
        if (exchangeApi->supportsWebSocket()) {
            exchangeApi->subscribeToWebSocket(symbol, this);
        } else {
            startHttpPolling(symbol, exchangeApi);
        }
    }

    void onPriceUpdate(const std::string& symbol, float price) {
        prices[symbol] = price;
        notifySubscribers(symbol, price);
    }

    float getPrice(const std::string& symbol) {
        return prices[symbol];
    }

private:
    MarketDataProvider() {}

    std::map<std::string, float> prices;
    std::map<std::string, std::vector<IPriceSubscriber*>> subscribers;

    void notifySubscribers(const std::string& symbol, float price) {
        for (auto subscriber : subscribers[symbol]) {
            subscriber->onPriceUpdate(symbol, price);
        }
    }

    void startHttpPolling(const std::string& symbol, IExchangeAPI* exchangeApi) {
        std::thread([this, symbol, exchangeApi]() {
            while (true) {
                float price = exchangeApi->getPrice(symbol); 
                this->onPriceUpdate(symbol, price);  
                std::this_thread::sleep_for(std::chrono::seconds(10)); 
            }
        }).detach();
    }
};