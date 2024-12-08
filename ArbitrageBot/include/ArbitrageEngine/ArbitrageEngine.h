#ifndef ARBITRAGE_ENGINE_H
#define ARBITRAGE_ENGINE_H

#include "Istrategy.h"
#include "IAPI.h"
#include "MarketDataProvider.h"
#include "OrderManager.h"
#include "TriangleGenerator.h"
#include "TriangleArbitrageStrategyv2.h"
#include <map>
#include <vector>
#include <memory>
#include <set>

class ArbitrageEngine : public IPriceSubscriber {
public:
    ArbitrageEngine(std::unique_ptr<IStrategy> strategy, std::shared_ptr<IAPI> api, const Wallet& wallet)
        : strategy(std::move(strategy)), exchangeAPI(api), orderManager(api), wallet(wallet) {}

    void subscribeToSymbols(const std::vector<std::string>& symbols) {
        for (const auto& symbol : symbols) {
            MarketDataProvider::getInstance().subscribe(symbol, this, exchangeAPI);
        }
    }

    void onPriceUpdate(const std::string& symbol, float price) override {
        priceCache[symbol] = price;
        checkArbitrageOpportunity();
    }

    const std::map<std::string, float>& getPriceCache() const {
        return priceCache;
    }

    void set_symbols(std::vector<std::string> sym) {
        this->symbols = sym;
    }

private:
    std::map<std::string, float> priceCache;
    std::unique_ptr<IStrategy> strategy;
    std::shared_ptr<IAPI> exchangeAPI;
    OrderManager orderManager;
    Wallet wallet;

    void checkArbitrageOpportunity() {

        auto pairs = generatePairsFromSymbols(symbols);

        TriangleGenerator generator(pairs, wallet);
        auto triangles = generator.getTrianglesByWallet();

        static int count = 0;
        if (strategy->evaluate(priceCache, triangles)) {
            std::cout << "=== PRICE CACHE ===\n";
            for (const auto& [pair, price] : priceCache) {
                std::cout << " - " << pair << ": " << price << "\n";
            }

            std::cout << "=== PROFITABLE OPPORTUNITY ===\n";
            dynamic_cast<TriangleArbitrageStrategy*>(strategy.get())->printSelectedTriangle();

            std::string actionsJson = strategy->generateActions();
            std::cout << "Generated Actions (JSON):\n" << actionsJson << "\n";

            orderManager.processActions(actionsJson);
        } else {
            if (count % 1000 == 0) {
                std::cout << "No opportunity found." << std::endl;
                count = 0;
                for (const auto& triangle : triangles) {
                    std::cout << " - " << triangle.pair1 << " (" << triangle.side1 << ") -> "
                              << triangle.pair2 << " (" << triangle.side2 << ") -> "
                              << triangle.pair3 << " (" << triangle.side3 << ")\n";
                }
            }
            count++;
        }
    }

    std::vector<std::string> generatePairsFromSymbols(const std::vector<std::string>& symbols) const {
        std::set<std::string> uniquePairs;

        for (const auto& symbol : symbols) {
            if (symbol.size() < 6) continue; 
            std::string base = symbol.substr(0, 3); 
            std::string quote = symbol.substr(3);

            uniquePairs.insert(base + quote);
        }

        return std::vector<std::string>(uniquePairs.begin(), uniquePairs.end());
    }


    std::vector<std::string> symbols = {"BTCUSDT", "ETHUSDT", "BNBUSDT"};
};

#endif
