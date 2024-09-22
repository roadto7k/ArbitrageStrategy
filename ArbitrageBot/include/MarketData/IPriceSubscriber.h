#include <string>

class IPriceSubscriber {
public:
    virtual void onPriceUpdate(const std::string& symbol, float price) = 0;
    virtual ~IPriceSubscriber() = default;
};