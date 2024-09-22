class OrderCommand {
public:
    virtual void execute() = 0;
    virtual ~OrderCommand() = default;
};

class BuyOrderCommand : public OrderCommand {
public:
    BuyOrderCommand(IExchangeAPI& api, const std::string& symbol, float amount)
        : exchangeAPI(api), symbol(symbol), amount(amount) {}

    void execute() override {
        exchangeAPI.buy(symbol, amount);
    }

private:
    IExchangeAPI& exchangeAPI;
    std::string symbol;
    float amount;
};

class SellOrderCommand : public OrderCommand {
public:
    SellOrderCommand(IExchangeAPI& api, const std::string& symbol, float amount)
        : exchangeAPI(api), symbol(symbol), amount(amount) {}

    void execute() override {
        exchangeAPI.sell(symbol, amount);
    }

private:
    IExchangeAPI& exchangeAPI;
    std::string symbol;
    float amount;
};