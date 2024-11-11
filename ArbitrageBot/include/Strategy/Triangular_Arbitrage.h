#include <iostream>
#include <unordered_map>
#include <vector>
#include <queue>
#include <set>
#include <string>
#include <algorithm>
#include <cmath>
#include <tuple>

// Global data structures to store market data
std::unordered_map<std::string, double> market_data;
std::vector<std::vector<std::pair<std::string, std::string>>> triangle_list;
std::unordered_map<std::string, std::vector<std::vector<std::pair<std::string, std::string>>>> pair_to_triangles;
std::unordered_map<std::vector<std::pair<std::string, std::string>>, double> triangle_profitability;
std::queue<std::string> update_queue;
double profit_threshold = 0.001;

// Pairs list and wallet currencies
std::vector<std::string> pairs_list = {"BTCETH", "ETHUSD", "USDBTC", "ETHSOL", "SOLBTC"};
std::vector<std::string> wallet_currencies = {"BTC", "ETH"};

void handle_market_data_update(const std::string& pair, double data) {
    market_data[pair] = data;
    update_queue.push(pair);
}

std::vector<std::vector<std::pair<std::string, std::string>>> generate_triangles(const std::vector<std::string>& pairs_list, const std::vector<std::string>& wallet_currencies) {
    std::set<std::string> cleaned_pairs;
    for (const auto& pair : pairs_list) {
        std::string base_currency = pair.substr(0, 3);
        std::string quote_currency = pair.substr(3);
        if (cleaned_pairs.find(quote_currency + base_currency) == cleaned_pairs.end()) {
            cleaned_pairs.insert(pair);
        }
    }

    std::vector<std::tuple<std::string, std::string, std::string>> edges;
    for (const auto& pair : cleaned_pairs) {
        std::string base_currency = pair.substr(0, 3);
        std::string quote_currency = pair.substr(3);
        edges.push_back(std::make_tuple(base_currency, quote_currency, "ask"));
        edges.push_back(std::make_tuple(quote_currency, base_currency, "bid"));
    }

    std::unordered_map<std::set<std::string>, std::vector<std::pair<std::string, std::string>>> unique_triangles;
    for (const auto& edge1 : edges) {
        std::string A = std::get<0>(edge1);
        std::string B = std::get<1>(edge1);
        std::string pair1 = A + B;
        std::string side1 = std::get<2>(edge1);
        for (const auto& edge2 : edges) {
            if (std::get<0>(edge2) != B) continue;
            std::string C = std::get<1>(edge2);
            std::string pair2 = B + C;
            std::string side2 = std::get<2>(edge2);
            for (const auto& edge3 : edges) {
                if (std::get<0>(edge3) != C || std::get<1>(edge3) != A) continue;
                std::string pair3 = C + A;
                std::string side3 = std::get<2>(edge3);
                std::vector<std::pair<std::string, std::string>> triangle = {
                    {pair1, side1},
                    {pair2, side2},
                    {pair3, side3}
                };
                std::set<std::string> triangle_set = {pair1, pair2, pair3};
                unique_triangles[triangle_set] = triangle;
            }
        }
    }

    std::vector<std::vector<std::pair<std::string, std::string>>> result_triangles;
    for (const auto& entry : unique_triangles) {
        result_triangles.push_back(entry.second);
    }
    return result_triangles;
}

void initialize_triangles(const std::vector<std::string>& pairs_list, const std::vector<std::string>& wallet_currencies) {
    triangle_list = generate_triangles(pairs_list, wallet_currencies);
    pair_to_triangles.clear();
    triangle_profitability.clear();
    for (const auto& triangle : triangle_list) {
        for (const auto& pair : triangle) {
            pair_to_triangles[pair.first].push_back(triangle);
        }
        // Initialize profitability for each triangle
        triangle_profitability[triangle] = 0.0;
    }
}

bool is_triangle_valid(const std::vector<std::pair<std::string, std::string>>& triangle) {
    for (const auto& pair : triangle) {
        if (market_data.find(pair.first) == market_data.end()) {
            return false;
        }
    }
    return true;
}

double calculate_arbitrage_opportunity(const std::vector<std::pair<std::string, std::string>>& triangle) {
    double profit = 1.0;
    for (const auto& pair : triangle) {
        double price = market_data[pair.first];
        if (pair.second == "ask") {
            profit /= price;
        } else {
            profit *= price;
        }
    }
    return profit;
}

std::vector<std::string> generate_trade_signal(const std::vector<std::pair<std::string, std::string>>& triangle) {
    std::vector<std::string> adjusted_pairs;
    for (const auto& pair : triangle) {
        if (pair.second == "bid") {
            std::string base_currency = pair.first.substr(0, 3);
            std::string quote_currency = pair.first.substr(3);
            adjusted_pairs.push_back(quote_currency + base_currency);
        } else {
            adjusted_pairs.push_back(pair.first);
        }
    }
    return adjusted_pairs;
}

void process_updates() {
    while (!update_queue.empty()) {
        std::string pair = update_queue.front();
        update_queue.pop();
        auto affected_triangles = pair_to_triangles.find(pair);
        if (affected_triangles != pair_to_triangles.end()) {
            for (const auto& triangle : affected_triangles->second) {
                if (is_triangle_valid(triangle)) {
                    double profit = calculate_arbitrage_opportunity(triangle);
                    triangle_profitability[triangle] = profit;  // Update profitability dictionary
                    if (profit > (1.0 + profit_threshold)) {
                        auto trade_signal = generate_trade_signal(triangle);
                        // Execute or log the trade signal here
                    }
                }
            }
        }
    }
}

int main() {
    initialize_triangles(pairs_list, wallet_currencies);
    // Simulate receiving data from a websocket
    handle_market_data_update("BTCETH", 0.025);
    handle_market_data_update("ETHUSD", 1700.0);
    handle_market_data_update("USDBTC", 40000.0);
    process_updates();
    return 0;
}
