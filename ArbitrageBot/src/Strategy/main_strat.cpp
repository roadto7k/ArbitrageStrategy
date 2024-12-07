#include "Wallet.h"
#include "TriangleGenerator.h"
#include "TriangleArbitrageStrategyv2.h"
#include <iostream>

int main() {
    Wallet wallet({{"BTC", 1.0f}, {"ETH", 2.0f}, {"USD", 1000.0f}});

    std::vector<std::string> pairs = {"BTCETH", "ETHUSD", "USDBTC"};

    TriangleGenerator generator(pairs, wallet);

    TriangleArbitrageStrategy strategy(0.01f); 

    std::map<std::string, float> priceCache = {
        {"BTCETH", 0.05f}, 
        {"ETHUSD", 3000.0f}, 
        {"USDBTC", 0.000033f} 
    };

    std::cout << "All Triangles Generated:\n";
    for (const auto& triangle : generator.getAllTriangles()) {
        std::cout << " - " << triangle.pair1 << " (" << triangle.side1 << ") -> "
                  << triangle.pair2 << " (" << triangle.side2 << ") -> "
                  << triangle.pair3 << " (" << triangle.side3 << ")\n";
    }

    std::cout << "\nTriangles Possible with Wallet:\n";
    auto possibleTriangles = generator.getTrianglesByWallet();
    for (const auto& triangle : possibleTriangles) {
        std::cout << " - " << triangle.pair1 << " (" << triangle.side1 << ") -> "
                  << triangle.pair2 << " (" << triangle.side2 << ") -> "
                  << triangle.pair3 << " (" << triangle.side3 << ")\n";
    }

    if (strategy.evaluate(priceCache, possibleTriangles)) {
        std::cout << "\nProfitable Triangle Found:\n";
        strategy.printSelectedTriangle();
    } else {
        std::cout << "\nNo profitable triangle found.\n";
    }

    return 0;
}
