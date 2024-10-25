#pragma once
#include "IPriceSubscriber.h"
#include "MarketDataCache.h"
#include <iostream>

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
