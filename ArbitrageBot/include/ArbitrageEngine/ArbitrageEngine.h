#include "Istrategy.h"
#include "IAPI.h"
#include "BinanceAPI.h"
#include "WebSocketClientWebSocketPP.h"
#include "HttpClient.h"
#include "MarketDataProvider.h"
#include "NetworkManager.h"
#include "OrderManager.h"

class ArbitrageEngine : public IPriceSubscriber {
public:
    ArbitrageEngine(std::unique_ptr<IStrategy> strategy, std::shared_ptr<IAPI> api)
        : strategy(std::move(strategy)), exchangeAPI(api), orderManager(api) {}

    void subscribeToSymbols(const std::vector<std::string>& symbols) {
        for (const auto& symbol : symbols) {
            MarketDataProvider::getInstance().subscribe(symbol, this, exchangeAPI);
        }
    }

    void onPriceUpdate(const std::string& symbol, float price) override {
        priceCache[symbol] = price;
        checkArbitrageOpportunity();
    }

private:
    std::map<std::string, float> priceCache; 
    std::unique_ptr<IStrategy> strategy;
    std::shared_ptr<IAPI> exchangeAPI;
    OrderManager orderManager;

    void checkArbitrageOpportunity() {
        if (strategy->evaluate(priceCache)) {
            std::string actionsJson = strategy->generateActions();
            orderManager.processActions(actionsJson);
        }
    }


};