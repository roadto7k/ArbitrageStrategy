#pragma once
#include <memory>
#include <string>
#include <iostream>
#include "IWebSocketClient.h"
#include "HttpClient.h"
#include "MockClients.h" 
class NetworkManager {
public:
    NetworkManager() {
        std::cout << "NetworkManager created with empty maps." << std::endl;
    }

    NetworkManager(const std::string& key, std::unique_ptr<HttpClient> httpClient, std::unique_ptr<IWebSocketClient> webSocketClient) {
        addHttpClient(key, std::move(httpClient));
        addWebSocketClient(key, std::move(webSocketClient));
    }

    void addHttpClient(const std::string& key, std::unique_ptr<HttpClient> client);

    void addWebSocketClient(const std::string& key, std::unique_ptr<IWebSocketClient> client);

    HttpResponse makeHttpRequest(const std::string& key, const HttpRequest& request);

    void connectWebSocket(const std::string& symbol, IWebSocketObserver* observer);

    void setHttpTimeout(const std::string& key, int milliseconds);

    void setHttpRetryPolicy(const std::string& key, int retries);

    void setWebSocketReconnectPolicy(const std::string& key, bool enableAutoReconnect);

private:
    std::map<std::string, std::unique_ptr<HttpClient>> httpClients;
    std::map<std::string, std::unique_ptr<IWebSocketClient>> webSocketClients;
};