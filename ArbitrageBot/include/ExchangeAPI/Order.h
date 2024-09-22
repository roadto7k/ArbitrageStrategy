#pragma once
#include <string>

class Order {
public:
    enum class Type { Limit, Market };
    
    Order(const std::string& symbol, float amount, float price, Type type)
        : symbol(symbol), amount(amount), price(price), type(type) {}

    std::string getSymbol() const { return symbol; }
    float getAmount() const { return amount; }
    float getPrice() const { return price; }
    Type getType() const { return type; }

private:
    std::string symbol;
    float amount;
    float price;
    Type type;
};