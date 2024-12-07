#ifndef WALLET_H
#define WALLET_H
#include <iostream>
#include <map>
#include <string>

class Wallet {
public:
    Wallet(const std::map<std::string, float>& initialBalances)
        : balances(initialBalances) {}

    bool hasCurrency(const std::string& currency) const {
        return balances.find(currency) != balances.end() && balances.at(currency) > 0;
    }

    float getBalance(const std::string& currency) const {
        return hasCurrency(currency) ? balances.at(currency) : 0.0f;
    }

    void updateBalance(const std::string& currency, float amount) {
        balances[currency] += amount;
    }

    void printBalances() const {
        std::cout << "Wallet Balances:\n";
        for (const auto& [currency, balance] : balances) {
            std::cout << " - " << currency << ": " << balance << "\n";
        }
    }

private:
    std::map<std::string, float> balances;
};

#endif