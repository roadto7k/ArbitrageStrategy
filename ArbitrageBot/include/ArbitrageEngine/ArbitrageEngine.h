class ArbitrageEngine : public IPriceSubscriber {
public:
    ArbitrageEngine(std::unique_ptr<IStrategy> strategy, std::unique_ptr<IExchangeAPI> api)
        : strategy(std::move(strategy)), exchangeAPI(std::move(api)) {}

    // Abonnement aux symboles pour recevoir des mises à jour de prix
    void subscribeToSymbols(const std::vector<std::string>& symbols) {
        for (const auto& symbol : symbols) {
            MarketDataProvider::getInstance().subscribe(symbol, this);
        }
    }

    // Implémentation de l'interface IPriceSubscriber
    void onPriceUpdate(const std::string& symbol, float price) override {
        priceCache[symbol] = price;
        checkArbitrageOpportunity();
    }

private:
    std::map<std::string, float> priceCache;  // Cache local des prix
    std::unique_ptr<IStrategy> strategy;  // Stratégie d'arbitrage utilisée (e.g. triangulaire)
    std::unique_ptr<IExchangeAPI> exchangeAPI;  // API pour passer des ordres

    // Vérification des opportunités d'arbitrage avec la stratégie
    void checkArbitrageOpportunity() {
        if (strategy->evaluate(priceCache)) {
            executeTrades();
        }
    }

    // Exécuter les ordres d'achat/vente
    void executeTrades() {
        auto orders = strategy->generateOrders();
        for (auto& order : orders) {
            order->execute();
        }
    }
};