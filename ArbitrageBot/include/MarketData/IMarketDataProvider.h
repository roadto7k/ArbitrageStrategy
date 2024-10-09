#pragma once
#include "IPriceSubscriber.h"
#include "String.h"
#include "IAPI.h"

class IMarketDataProvider : public IPriceSubscriber {
public:
    virtual ~IMarketDataProvider() = default;
    virtual void subscribe(const std::string& symbol, IPriceSubscriber* subscriber, IAPI* api) = 0;
    virtual float getPrice(const std::string& symbol) = 0;
};