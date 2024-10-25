#pragma once
#include <string>
#include "Order.h"
// #include "IPriceSubscriber.h"
#include "IAPI.h"

class IExchangeAPI : public IAPI {
public:
    // virtual float getPrice(const std::string& symbol) = 0;
    virtual void sendOrder(const Order& order) = 0;
    // virtual void subscribeToWebSocket(const std::string& symbol, IPriceSubscriber* provider) = 0;
    virtual ~IExchangeAPI() = default;
};