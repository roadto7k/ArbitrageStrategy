#pragma once
#include "IExchangeAPI.h"
#include "BinanceAPI.h"
#include "NetworkManager.h"
#include <memory>

class ExchangeFactory {
public:
    static std::unique_ptr<IExchangeAPI> createExchange(const std::string& exchangeName, NetworkManager& networkManager) {
        if (exchangeName == "Binance") {
            return std::make_unique<BinanceAPI>(networkManager);
        }
        return nullptr;
    }
};