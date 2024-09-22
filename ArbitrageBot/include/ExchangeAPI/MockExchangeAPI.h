#include <iostream>
#include <string>

#include "IExchangeAPI.h"

class MockExchangeAPI : public IExchangeAPI {
public:
    void buy(const std::string& symbol, float amount) override {
        std::cout << "Executed BUY order for " << amount << " of " << symbol << std::endl;
    }

    void sell(const std::string& symbol, float amount) override {
        std::cout << "Executed SELL order for " << amount << " of " << symbol << std::endl;
    }
};