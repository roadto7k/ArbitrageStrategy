class ArbitrageEngine : public IPriceSubscriber {
public:
    ArbitrageEngine(std::unique_ptr<IStrategy> strategy, std::unique_ptr<IExchangeAPI> api)
        : strategy(std::move(strategy)), exchangeAPI(std::move(api)) {}

    void subscribeToSymbols(const std::vector<std::string>& symbols) {
        for (const auto& symbol : symbols) {
            MarketDataProvider::getInstance().subscribe(symbol, this);
        }
    }

    void onPriceUpdate(const std::string& symbol, float price) override {
        priceCache[symbol] = price;
        checkArbitrageOpportunity();
    }

private:
    std::map<std::string, float> priceCache; 
    std::unique_ptr<IStrategy> strategy;
    std::unique_ptr<IExchangeAPI> exchangeAPI;

    void checkArbitrageOpportunity() {
        if (strategy->evaluate(priceCache)) {
            executeTrades();
        }
    }

    void executeTrades() {
        auto orders = strategy->generateOrders();
        for (auto& order : orders) {
            order->execute();
        }
    }
};