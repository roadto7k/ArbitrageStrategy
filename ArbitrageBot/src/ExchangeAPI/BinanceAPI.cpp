#include "BinanceAPI.h"
#include "JsonParser.h"

// BinanceAPI::BinanceAPI(NetworkManager& networkManager) : networkManager(networkManager) {}

std::string BinanceAPI::buildUrl(const std::string& endpoint) const {
    return "https://api.binance.com" + endpoint; 
}

float BinanceAPI::getPrice(const std::string& symbol) {
    std::string url = buildUrl("/api/v3/ticker/price?symbol=" + symbol);
    HttpResponse response = networkManager.makeHttpRequest(HttpRequest(url));

    nlohmann::json jsonResponse = JsonParser::parse(response.getBody());
    return JsonParser::getValue<float>(jsonResponse, "price");
}

void BinanceAPI::sendOrder(const Order& order) {
    std::string url = buildUrl("/api/v3/order");

    nlohmann::json orderJson = {
        {"symbol", order.getSymbol()},
        {"quantity", order.getAmount()},
        {"price", order.getPrice()},
        {"type", order.getType() == Order::Type::Market ? "MARKET" : "LIMIT"}
    };
    HttpRequest request(url, {}, JsonParser::toJsonString(orderJson), "POST"); 
    HttpResponse response = networkManager.makeHttpRequest(request);
}