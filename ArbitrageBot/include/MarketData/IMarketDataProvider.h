#pragma once
#include "IPriceSubscriber.h"
#include <string>
#include "IAPI.h"
#include <memory>

class IMarketDataProvider : public IPriceSubscriber {
public:
    virtual ~IMarketDataProvider() = default;
    virtual void subscribe(const std::string& symbol, IPriceSubscriber* subscriber, std::shared_ptr<IAPI> api) = 0;
    virtual float getPrice(const std::string& symbol) = 0;
};