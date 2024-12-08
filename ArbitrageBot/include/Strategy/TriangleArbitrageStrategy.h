// #ifndef TRIANGLE_ARBITRAGE_STRATEGY_H
// #define TRIANGLE_ARBITRAGE_STRATEGY_H
// #include "Istrategy.h"
// #include <string>
// #include <vector>
// #include <map>
// #include <memory>
// #include "JsonParser.h"
// // #include "OrderCommand.h"

// class TriangleArbitrageStrategy : public IStrategy {
// public:
//     TriangleArbitrageStrategy(const std::string& pair1, const std::string& pair2, const std::string& pair3)
//         : pair1(pair1), pair2(pair2), pair3(pair3) {}

//     bool evaluate(const std::map<std::string, float>& priceCache) override {
//         if (priceCache.find(pair1) != priceCache.end() &&
//             priceCache.find(pair2) != priceCache.end() &&
//             priceCache.find(pair3) != priceCache.end()) {

//             float price1 = priceCache.at(pair1);
//             float price2 = priceCache.at(pair2);
//             float price3 = priceCache.at(pair3);

//             profit = calculateTriangleArbitrage(price1, price2, price3);
//             return profit > threshold;
//         }
//         return false;
//     }

//     std::string generateActions() const override {
//         nlohmann::json actions = {
//             {"type", "triangle_arbitrage"},
//             {"pairs", {pair1, pair2, pair3}},
//             {"expected_profit", profit},
//             {"orders", {
//                 {{"type", "buy"}, {"symbol", pair1}, {"amount", 100}},
//                 {{"type", "sell"}, {"symbol", pair2}, {"amount", 100}},
//                 {{"type", "sell"}, {"symbol", pair3}, {"amount", 100}}
//             }}
//         };

//         return JsonParser::toJsonString(actions);
//     }


// private:
//     std::string pair1, pair2, pair3;
//     float threshold = 0.01;
//     float profit = 0.0;

//     float calculateTriangleArbitrage(float price1, float price2, float price3) {
//         return price1 * price2 * price3 - 1.0f;
//     }
// };

// #endif