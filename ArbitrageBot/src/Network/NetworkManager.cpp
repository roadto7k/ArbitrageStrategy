#include "NetworkManager.h"

NetworkManager::NetworkManager(std::unique_ptr<HttpClient> httpClient, std::unique_ptr<IWebSocketClient> webSocketClient)
    : httpClient(std::move(httpClient)), webSocketClient(std::move(webSocketClient)) {}

HttpResponse NetworkManager::makeHttpRequest(const HttpRequest& request) {
    return httpClient->sendRequest(request);
}

void NetworkManager::connectWebSocket(const std::string& symbol, IWebSocketObserver* observer) {
    webSocketClient->addObserver(observer);
    webSocketClient->connect();
}

void NetworkManager::setHttpTimeout(int milliseconds) {
    httpClient->setTimeout(milliseconds);
}

void NetworkManager::setHttpRetryPolicy(int retries) {
    httpClient->setRetryPolicy(retries);
}

void NetworkManager::setWebSocketReconnectPolicy(bool enableAutoReconnect) {
    // A faire ici 
    if (enableAutoReconnect) {
        std::cout << "WebSocket auto-reconnect enabled" << std::endl;
    }
}