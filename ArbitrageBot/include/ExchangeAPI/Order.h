#pragma once
#include <string>

class Order {
public:
    enum class Type { Limit, Market };

    Order(const std::string& symbol, float amount, float price, Type type, const std::string& operation)
        : symbol(symbol), amount(amount), price(price), type(type), operation(operation) {}

    // Getters
    std::string getSymbol() const { return symbol; }
    float getAmount() const { return amount; }
    float getPrice() const { return price; }
    Type getType() const { return type; }
    std::string getOperation() const { return operation; }

    static Order createBuyOrder(const std::string& symbol, float amount, float price, Type type) {
        return Order(symbol, amount, price, type, "buy");
    }

    static Order createSellOrder(const std::string& symbol, float amount, float price, Type type) {
        return Order(symbol, amount, price, type, "sell");
    }

private:
    std::string symbol;
    float amount;
    float price;
    Type type;
    std::string operation; // "buy" or "sell"
};