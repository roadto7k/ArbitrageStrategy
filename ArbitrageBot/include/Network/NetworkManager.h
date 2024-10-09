#pragma once
#include <memory>
#include <string>
#include <iostream>
#include "IWebSocketClient.h"
#include "HttpClient.h"
#include "MockClients.h" 
class NetworkManager {
public:
    NetworkManager() 
        : httpClient(std::make_unique<MockHttpClient>()),
          webSocketClient(std::make_unique<MockWebSocketClient>()) {
        std::cout << "NetworkManager created with mocks (MockHttpClient and MockWebSocketClient)" << std::endl;
    }

    NetworkManager(std::unique_ptr<HttpClient> httpClient, std::unique_ptr<IWebSocketClient> webSocketClient);

    HttpResponse makeHttpRequest(const HttpRequest& request);

    void connectWebSocket(const std::string& symbol, IWebSocketObserver* observer);

    void setHttpTimeout(int milliseconds);
    void setHttpRetryPolicy(int retries);
    void setWebSocketReconnectPolicy(bool enableAutoReconnect);

private:
    std::unique_ptr<HttpClient> httpClient; 
    std::unique_ptr<IWebSocketClient> webSocketClient;
};