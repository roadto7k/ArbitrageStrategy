#ifndef TRIANGLE_ARBITRAGE_STRATEGY_V2_H
#define TRIANGLE_ARBITRAGE_STRATEGY_V2_H
#include <map>
#include <string>
#include <vector>
#include <optional>
#include "TriangleGenerator.h"

class TriangleArbitrageStrategy {
public:
    TriangleArbitrageStrategy(float threshold)
        : profitThreshold(threshold), profit(0.0f) {}

    bool evaluate(const std::map<std::string, float>& priceCache, const std::vector<TriangleGenerator::Triangle>& triangles) {
        for (const auto& triangle : triangles) {
            if (isTriangleValid(triangle, priceCache)) {
                float currentProfit = calculateProfit(triangle, priceCache);
                if (currentProfit > profitThreshold) {
                    profit = currentProfit;
                    selectedTriangle = triangle;
                    return true;
                }
            }
        }
        return false;
    }

    void printSelectedTriangle() const {
        if (selectedTriangle) {
            const auto& triangle = *selectedTriangle;
            std::cout << "Selected Triangle:\n"
                      << " - " << triangle.pair1 << " (" << triangle.side1 << ")\n"
                      << " - " << triangle.pair2 << " (" << triangle.side2 << ")\n"
                      << " - " << triangle.pair3 << " (" << triangle.side3 << ")\n"
                      << "Expected Profit: " << profit << "\n";
        } else {
            std::cout << "No profitable triangle found.\n";
        }
    }

private:
    float profitThreshold;
    float profit;
    std::optional<TriangleGenerator::Triangle> selectedTriangle;

    bool isTriangleValid(const TriangleGenerator::Triangle& triangle, const std::map<std::string, float>& priceCache) const {
        return priceCache.find(triangle.pair1) != priceCache.end() &&
               priceCache.find(triangle.pair2) != priceCache.end() &&
               priceCache.find(triangle.pair3) != priceCache.end();
    }

    float calculateProfit(const TriangleGenerator::Triangle& triangle, const std::map<std::string, float>& priceCache) const {
        float price1 = priceCache.at(triangle.pair1);
        float price2 = priceCache.at(triangle.pair2);
        float price3 = priceCache.at(triangle.pair3);

        return (triangle.side1 == "ask" ? 1.0f / price1 : price1) *
               (triangle.side2 == "ask" ? 1.0f / price2 : price2) *
               (triangle.side3 == "ask" ? 1.0f / price3 : price3) - 1.0f;
    }
};

#endif