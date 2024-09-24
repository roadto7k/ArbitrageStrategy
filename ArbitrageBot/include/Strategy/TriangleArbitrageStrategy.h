class TriangleArbitrageStrategy : public IStrategy {
public:
    TriangleArbitrageStrategy(const std::string& pair1, const std::string& pair2, const std::string& pair3)
        : pair1(pair1), pair2(pair2), pair3(pair3) {}

    bool evaluate(const std::map<std::string, float>& priceCache) override {
        if (priceCache.find(pair1) != priceCache.end() &&
            priceCache.find(pair2) != priceCache.end() &&
            priceCache.find(pair3) != priceCache.end()) {

            float price1 = priceCache.at(pair1);
            float price2 = priceCache.at(pair2);
            float price3 = priceCache.at(pair3);

            float profit = calculateTriangleArbitrage(price1, price2, price3);
            return profit > threshold;
        }
        return false;
    }

    std::vector<std::unique_ptr<OrderCommand>> generateOrders() override {
        return {
            std::make_unique<BuyOrderCommand>(exchangeAPI, pair1, amount1),
            std::make_unique<SellOrderCommand>(exchangeAPI, pair2, amount2),
            std::make_unique<SellOrderCommand>(exchangeAPI, pair3, amount3)
        };
    }

private:
    std::string pair1, pair2, pair3;
    float threshold = 0.01; 

    float calculateTriangleArbitrage(float price1, float price2, float price3) {
        return price1 * price2 * price3 - 1.0f;
    }
};