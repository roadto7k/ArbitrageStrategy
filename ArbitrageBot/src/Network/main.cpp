#include "NetworkManager.h"
#include "HttpClient.h"
#include "WebSocketClientWebSocketPP.h"
#include <iostream>
#include <memory>

class TestWebSocketObserver : public IWebSocketObserver {
public:
    void onMessageReceived(const std::string& message) override {
        std::cout << "WebSocket message received: " << message << std::endl;
    }

    void onConnectionClosed() override {
        std::cout << "WebSocket connection closed" << std::endl;
    }
};

int main() {
    try {
        std::cout << "Client creation" << std::endl;
        auto httpClient = std::make_unique<CurlHttpClient>();
        auto webSocketClient = std::make_unique<WebSocketClientWebSocketPP>("wss://ws.ifelse.io");

        std::cout << "Manager creation" << std::endl;
        NetworkManager networkManager(std::move(httpClient), std::move(webSocketClient));

        std::cout << "Test get request";
        HttpRequest httpRequest;
        httpRequest.url = "https://httpbin.org/get";
        httpRequest.method = "GET";
        
        HttpResponse httpResponse = networkManager.makeHttpRequest(httpRequest);
        std::cout << "HTTP GET Response: " << httpResponse.statusCode << " " << httpResponse.body << std::endl;

        std::cout << "Test post request : " << std::endl;
        HttpRequest postRequest;
        postRequest.url = "https://httpbin.org/post";
        postRequest.method = "POST";
        postRequest.body = "{\"test\": \"data\"}";
        postRequest.headers["Content-Type"] = "application/json";

        HttpResponse postResponse = networkManager.makeHttpRequest(postRequest);
        std::cout << "HTTP POST Response: " << postResponse.statusCode << " " << postResponse.body << std::endl;

        std::cout << "http timout" << std::endl;
        networkManager.setHttpTimeout(5000);

        std::cout << "Retry policy" << std::endl;
        networkManager.setHttpRetryPolicy(3);

        std::cout << "websocket creation" << std::endl;
        TestWebSocketObserver wsObserver;
        networkManager.connectWebSocket("test-symbol", &wsObserver);

        std::this_thread::sleep_for(std::chrono::seconds(1));

 
        networkManager.setWebSocketReconnectPolicy(true);

        std::this_thread::sleep_for(std::chrono::seconds(10));
        int a;
        std::cin >> a;
    }
    catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}