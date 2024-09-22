class OrderManager {
public:
    OrderManager(std::unique_ptr<IExchangeAPI> api) : exchangeAPI(std::move(api)) {}

    void executeOrder(std::unique_ptr<OrderCommand> order) {
        order->execute();
    }

private:
    std::unique_ptr<IExchangeAPI> exchangeAPI;
};