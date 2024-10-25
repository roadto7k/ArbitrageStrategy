// IAPI.h
#pragma once
#include <string>

class IPriceSubscriber;

class IAPI {
public:
    virtual ~IAPI() = default;
    virtual float getPrice(const std::string& symbol) = 0;
    virtual bool supportsWebSocket() const = 0;
    virtual void subscribeToWebSocket(const std::string& symbol, IPriceSubscriber* subscriber) = 0;
};