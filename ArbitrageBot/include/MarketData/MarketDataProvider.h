class MarketDataProvider {
public:
    static MarketDataProvider& getInstance() {
        static MarketDataProvider instance;
        return instance;
    }

    void subscribe(const std::string& symbol, IPriceSubscriber* subscriber) {
        subscribers[symbol].push_back(subscriber);
        
        if (websocketSubscriptions.find(symbol) == websocketSubscriptions.end()) {
            startWebSocketSubscription(symbol);
        }
    }

    void updatePrice(const std::string& symbol) {
        HttpRequest request{"https://api.exchange.com/v1/ticker/price?symbol=" + symbol, {}, "", "GET"};
        
        HttpResponse response = networkManager.makeHttpRequest(request);
        
        float price = JsonParser::parse(response.body)["price"].get<float>();
        prices[symbol] = price;
        notifySubscribers(symbol, price);
    }

    float getPrice(const std::string& symbol) {
        return prices[symbol];
    }

    void startWebSocketSubscription(const std::string& symbol) {
        networkManager.connectWebSocket(symbol, this);
        websocketSubscriptions.insert(symbol);
    }

    void onMessageReceived(const std::string& symbol, const std::string& message) {
        float price = JsonParser::parse(message)["price"].get<float>();
        
        prices[symbol] = price;
        notifySubscribers(symbol, price);
    }

private:
    MarketDataProvider() : networkManager(NetworkManager::getInstance()) {}

    std::map<std::string, float> prices; 
    std::map<std::string, std::vector<IPriceSubscriber*>> subscribers; 
    std::set<std::string> websocketSubscriptions;
    NetworkManager& networkManager;

    void notifySubscribers(const std::string& symbol, float price) {
        for (auto subscriber : subscribers[symbol]) {
            subscriber->onPriceUpdate(symbol, price);
        }
    }
};