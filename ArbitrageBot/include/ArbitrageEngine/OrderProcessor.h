#ifndef ORDER_PROCESSOR_H
#define ORDER_PROCESSOR_H

#include <iostream>
#include <string>
#include "IAPI.h"
#include "Order.h"

class OrderProcessor {
public:
    static void processBuyOrder(std::shared_ptr<IAPI> api, const std::string& symbol, float amount, float price, Order::Type type) {
        Order order = Order::createBuyOrder(symbol, amount, price, type);
        std::cout << "Sending Buy Order: " << createOrderJson(order) << std::endl;

        // api->sendOrder(order);
    }

    static void processSellOrder(std::shared_ptr<IAPI> api, const std::string& symbol, float amount, float price, Order::Type type) {
        Order order = Order::createSellOrder(symbol, amount, price, type);
        std::cout << "Sending Sell Order: " << createOrderJson(order) << std::endl;

        // api->sendOrder(order);
    }

private:
    static std::string createOrderJson(const Order& order) {
        return "{ \"operation\": \"" + order.getOperation() + "\", "
               "\"symbol\": \"" + order.getSymbol() + "\", "
               "\"amount\": " + std::to_string(order.getAmount()) + ", "
               "\"price\": " + std::to_string(order.getPrice()) + ", "
               "\"type\": \"" + (order.getType() == Order::Type::Market ? "Market" : "Limit") + "\" }";
    }
};

#endif