#include "WebSocketClientWebSocketPP.h"

class MyWebSocketObserver : public IWebSocketObserver {
public:
    void onMessageReceived(const std::string& message) override {
        std::cout << "Received: " << message << std::endl;
    }

    void onConnectionClosed() override {
        std::cout << "WebSocket connection closed" << std::endl;
    }
};

int main() {
    std::cout << "waaaouw" << std::endl;
    MyWebSocketObserver observer;
    
    std::unique_ptr<IWebSocketClient> client = std::make_unique<WebSocketClientWebSocketPP>("ws://example.com/socket");

    client->addObserver(&observer);
    client->connect();

    // Simulate sending a message
    std::this_thread::sleep_for(std::chrono::seconds(1));
    client->sendMessage("Hello, WebSocket!");

    // Wait for some messages
    std::this_thread::sleep_for(std::chrono::seconds(5));

    client->disconnect();

    return 0;
}