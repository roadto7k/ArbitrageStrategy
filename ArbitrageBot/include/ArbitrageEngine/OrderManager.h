#ifndef ORDER_MANAGER_H
#define ORDER_MANAGER_H

#include "OrderProcessor.h"
#include "OrderCommand.h"
#include <memory>
#include <vector>
#include "JsonParser.h"

class OrderManager {
public:

    explicit OrderManager()  {}
    explicit OrderManager(std::shared_ptr<IAPI> api) : api(api) {}
    void executeOrders() {
        for (auto& order : orders) {
            order->execute();
        }
        orders.clear();
    }


    void processActions(const std::string& actionsJson) {
        nlohmann::json actions = JsonParser::parse(actionsJson);

        if (JsonParser::getValue<std::string>(actions, "type", "") == "triangle_arbitrage") {
            auto ordersArray = actions["orders"];
            for (const auto& order : ordersArray) {
                std::string type = JsonParser::getValue<std::string>(order, "type", "");
                std::string symbol = JsonParser::getValue<std::string>(order, "symbol", "");
                float amount = JsonParser::getValue<float>(order, "amount", 0.0f);
                float price = JsonParser::getValue<float>(order, "price", 0.0f);
                std::cout << order << std::endl;
                std::cout << "type : " << type << std::endl;
                if (type == "buy") {
                    orders.emplace_back(std::make_unique<BuyOrderCommand>(symbol, amount, price, Order::Type::Limit, api));
                } else if (type == "sell") {
                    orders.emplace_back(std::make_unique<SellOrderCommand>(symbol, amount, price, Order::Type::Limit, api));
                } else {
                    std::cerr << "Unknown order type: " << type << std::endl;
                }
            }
        }

        executeOrders();
    }


private:
    std::vector<std::unique_ptr<OrderCommand>> orders;
    std::shared_ptr<IAPI> api;
};

#endif