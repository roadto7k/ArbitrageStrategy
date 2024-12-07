#include "Wallet.h"
#include "TriangleGenerator.h"

int main() {
    Wallet wallet({{"BTC", 1.0f}, {"ETH", 2.0f}, {"USD", 1000.0f}});
    
    std::vector<std::string> pairs = {"BTCETH", "ETHUSD", "USDBTC", "BTCSOL", "SOLETH"};

    TriangleGenerator generator(pairs, wallet);

    std::cout << "All Triangles:\n";
    for (const auto& triangle : generator.getAllTriangles()) {
        std::cout << " - " << triangle.pair1 << " (" << triangle.side1 << ") -> "
                  << triangle.pair2 << " (" << triangle.side2 << ") -> "
                  << triangle.pair3 << " (" << triangle.side3 << ")\n";
    }

    std::cout << "\nTriangles Possible with Wallet:\n";
    for (const auto& triangle : generator.getTrianglesByWallet()) {
        std::cout << " - " << triangle.pair1 << " (" << triangle.side1 << ") -> "
                  << triangle.pair2 << " (" << triangle.side2 << ") -> "
                  << triangle.pair3 << " (" << triangle.side3 << ")\n";
    }

    return 0;
}
