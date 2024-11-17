#pragma once
#include <string>
#include <map>
#include <set>
#include <vector>
#include <thread>
#include <memory>

#include "IMarketDataProvider.h"
#include "Http.h"
#include "NetworkManager.h"
#include "JsonParser.h"
#include "IAPI.h"

class MarketDataProvider : public IMarketDataProvider {
public:
    static MarketDataProvider& getInstance() {
        static MarketDataProvider instance;
        return instance;
    }

    void subscribe(const std::string& symbol, IPriceSubscriber* subscriber, std::shared_ptr<IAPI> api) {
        subscribers[symbol].push_back(subscriber);
        if (api->supportsWebSocket()) {
            api->subscribeToWebSocket(symbol, this);
        } else {
            startHttpPolling(symbol, api);
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

    void startHttpPolling(const std::string& symbol, std::shared_ptr<IAPI> api) {
        std::thread([this, symbol, api]() {
            while (true) { 
                float price = api->getPrice(symbol); 
                this->onPriceUpdate(symbol, price);  
                std::this_thread::sleep_for(std::chrono::seconds(10)); 
            }
        }).detach();
    }
};