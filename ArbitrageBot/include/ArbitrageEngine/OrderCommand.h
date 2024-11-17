#ifndef ORDER_COMMAND_H
#define ORDER_COMMAND_H

#include "OrderProcessor.h"
#include "IAPI.h"
#include <memory>

class OrderCommand {
public:
    virtual void execute() = 0;
    virtual ~OrderCommand() = default;
};

class BuyOrderCommand : public OrderCommand {
public:
    BuyOrderCommand(const std::string& symbol, float amount, float price, Order::Type type, std::shared_ptr<IAPI> api)
        : symbol(symbol), amount(amount), price(price), type(type), api(api) {}

    void execute() override {
        OrderProcessor::processBuyOrder(api, symbol, amount, price, type);
    }

private:
    std::string symbol;
    float amount;
    float price;
    Order::Type type;
    std::shared_ptr<IAPI> api;
};

class SellOrderCommand : public OrderCommand {
public:
    SellOrderCommand(const std::string& symbol, float amount, float price, Order::Type type, std::shared_ptr<IAPI> api)
        : symbol(symbol), amount(amount), price(price), type(type), api(api) {}

    void execute() override {
        OrderProcessor::processSellOrder(api, symbol, amount, price, type);
    }

private:
    std::string symbol;
    float amount;
    float price;
    Order::Type type;
    std::shared_ptr<IAPI> api;
};

#endif