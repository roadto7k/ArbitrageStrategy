#include "Wallet.h"

int main() {
    Wallet wallet({{"BTC", 1.0f}, {"ETH", 5.0f}, {"USD", 1000.0f}});
    wallet.printBalances();
    wallet.updateBalance("BTC", -0.5f);
    wallet.printBalances();
    return 0;
}