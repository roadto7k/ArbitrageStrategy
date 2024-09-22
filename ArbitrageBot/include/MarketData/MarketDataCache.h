#include <string>
#include <map>
#include <stdexcept>

class MarketDataCache {
public:
    void storePrice(const std::string& symbol, float price) {
        priceCache[symbol] = price;
    }

    float getPrice(const std::string& symbol) const {
        auto it = priceCache.find(symbol);
        if (it != priceCache.end()) {
            return it->second;
        } else {
            throw std::runtime_error("Prix non disponible pour le symbole: " + symbol);
        }
    }

private:
    std::map<std::string, float> priceCache;
};