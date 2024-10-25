// WebSocketPriceObserver.h
#pragma once
#include "WebSocketObserver.h"
#include <string>

class WebSocketPriceObserver : public WebSocketObserver {
public:
    WebSocketPriceObserver(const std::string& symbol, IPriceSubscriber* provider)
        : WebSocketObserver(symbol, provider) {}

};