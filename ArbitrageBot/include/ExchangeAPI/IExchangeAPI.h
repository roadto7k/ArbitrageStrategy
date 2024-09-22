#pragma once
#include <string>
#include "Order.h"

class IExchangeAPI {
public:
    virtual float getPrice(const std::string& symbol) = 0;
    virtual void sendOrder(const Order& order) = 0;
    virtual void subscribeToWebSocket(const std::string& symbol, MarketDataProvider* provider) = 0;
    virtual ~IExchangeAPI() = default;
};