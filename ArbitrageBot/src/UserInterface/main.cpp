#include <iostream>
#include <memory>
#include "ArbitrageEngine.h"
#include "MarketDataProvider.h"
#include "MockExchangeAPI.h"

int main() {
    std::unique_ptr<IExchangeAPI> mockExchangeAPI = std::make_unique<MockExchangeAPI>();

    std::unique_ptr<IStrategy> triangleArbStrategy = std::make_unique<TriangleArbitrageStrategy>("BTC/USD", "ETH/BTC", "ETH/USD");

    ArbitrageEngine arbEngine(std::move(triangleArbStrategy), std::move(mockExchangeAPI));

    MarketDataProvider& market = MarketDataProvider::getInstance();

    market.subscribe("BTC/USD", &arbEngine, arbEngine.getExchangeAPI());
    market.subscribe("ETH/BTC", &arbEngine, arbEngine.getExchangeAPI());
    market.subscribe("ETH/USD", &arbEngine, arbEngine.getExchangeAPI());

    market.onPriceUpdate("BTC/USD", 50000.0f);
    market.onPriceUpdate("ETH/BTC", 0.07f);
    market.onPriceUpdate("ETH/USD", 3500.0f);

    market.onPriceUpdate("BTC/USD", 49000.0f);
    market.onPriceUpdate("ETH/BTC", 0.06f);
    market.onPriceUpdate("ETH/USD", 3200.0f);

    return 0;
}