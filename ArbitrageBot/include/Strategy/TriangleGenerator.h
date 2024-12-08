#ifndef TRIANGLE_GENERATOR_H
#define TRIANGLE_GENERATOR_H
#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <string>
#include <tuple>
#include "Wallet.h"

#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <string>
#include <tuple>
#include "Wallet.h"

class TriangleGenerator {
public:
    struct Triangle {
        std::string pair1, pair2, pair3;
        std::string side1, side2, side3;
    };

    TriangleGenerator(const std::vector<std::string>& pairs, const Wallet& wallet)
        : wallet(wallet) {
        generateTriangles(pairs);
    }

    const std::vector<Triangle>& getAllTriangles() const {
        return triangles;
    }

    std::vector<Triangle> getTrianglesByWallet() const {
        std::vector<Triangle> filteredTriangles;

        for (const auto& triangle : triangles) {
            auto [A, _] = parsePair(triangle.pair1);
            if (wallet.hasCurrency(A)) {
                filteredTriangles.push_back(triangle);
            }
        }

        return filteredTriangles;
    }

private:
    Wallet wallet;
    std::vector<Triangle> triangles;

    void generateTriangles(const std::vector<std::string>& pairs) {
        std::set<std::string> uniquePairs(pairs.begin(), pairs.end());
        std::vector<Edge> edges;

        for (const auto& pair : uniquePairs) {
            auto [base, quote] = parsePair(pair);
            edges.push_back({base, quote, pair, "ask"});
            edges.push_back({quote, base, pair, "bid"});
        }

        for (const auto& edge1 : edges) {
            const std::string& A = edge1.from;
            const std::string& B = edge1.to;
            for (const auto& edge2 : edges) {
                if (edge2.pair == edge1.pair || edge2.from != B) continue;
                const std::string& C = edge2.to;
                for (const auto& edge3 : edges) {
                    if (edge3.pair == edge1.pair || edge3.pair == edge2.pair || edge3.from != C || edge3.to != A) continue;
                    triangles.push_back({edge1.pair, edge2.pair, edge3.pair, edge1.side, edge2.side, edge3.side});
                }
            }
        }
    }

    struct Edge {
        std::string from, to, pair, side;
    };

    std::pair<std::string, std::string> parsePair(const std::string& pair) const {
        static const std::set<std::string> knownCurrencies = {"BTC", "ETH", "BNB", "USDT", "SOL", "ADA", "DOT", "DOGE"};

        for (const auto& base : knownCurrencies) {
            if (pair.rfind(base, 0) == 0) {
                std::string quote = pair.substr(base.size());
                if (knownCurrencies.count(quote)) {
                    return {base, quote};
                }
            }
        }

        throw std::invalid_argument("Unknown pair format: " + pair);
    }
};


#endif