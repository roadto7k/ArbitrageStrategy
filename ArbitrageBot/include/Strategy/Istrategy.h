class IStrategy {
public:
    virtual bool evaluate(const std::map<std::string, float>& priceCache) = 0; 
    virtual std::vector<std::unique_ptr<OrderCommand>> generateOrders() = 0;
    virtual ~IStrategy() = default;
};