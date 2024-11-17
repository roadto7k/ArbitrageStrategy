#include "NetworkManager.h"

void NetworkManager::addHttpClient(const std::string& key, std::unique_ptr<HttpClient> client) {
    httpClients[key] = std::move(client);
    std::cout << "Added HttpClient for key: " << key << std::endl;
}

void NetworkManager::addWebSocketClient(const std::string& key, std::unique_ptr<IWebSocketClient> client) {
    webSocketClients[key] = std::move(client);
    std::cout << "Added WebSocketClient for key: " << key << std::endl;
}

void NetworkManager::connectWebSocket(const std::string& symbol, IWebSocketObserver* observer) {
    if (webSocketClients.find(symbol) == webSocketClients.end()) {
        throw std::runtime_error("WebSocketClient not found for symbol: " + symbol);
    }
    auto& webSocketClient = webSocketClients[symbol];
    webSocketClient->addObserver(observer);
    webSocketClient->connect();
    std::cout << "Connected WebSocket for symbol: " << symbol << std::endl;
}

void NetworkManager::setHttpTimeout(const std::string& key, int milliseconds) {
    if (httpClients.find(key) == httpClients.end()) {
        throw std::runtime_error("HttpClient not found for key: " + key);
    }
    httpClients[key]->setTimeout(milliseconds);
}

void NetworkManager::setHttpRetryPolicy(const std::string& key, int retries) {
    if (httpClients.find(key) == httpClients.end()) {
        throw std::runtime_error("HttpClient not found for key: " + key);
    }
    httpClients[key]->setRetryPolicy(retries);
}


void NetworkManager::setWebSocketReconnectPolicy(const std::string& key, bool enableAutoReconnect) {
    if (webSocketClients.find(key) == webSocketClients.end()) {
        throw std::runtime_error("WebSocketClient not found for key: " + key);
    }
    if (enableAutoReconnect) {
        std::cout << "WebSocket auto-reconnect enabled for key: " << key << std::endl;
    } else {
        std::cout << "WebSocket auto-reconnect disabled for key: " << key << std::endl;
    }
}

// NetworkManager::NetworkManager(std::unique_ptr<HttpClient> httpClient, std::unique_ptr<IWebSocketClient> webSocketClient)
//     : httpClient(std::move(httpClient)), webSocketClient(std::move(webSocketClient)) {}

HttpResponse NetworkManager::makeHttpRequest(const std::string& key, const HttpRequest& request) {
    if (httpClients.find(key) == httpClients.end()) {
        throw std::runtime_error("WebSocketClient not found for key: " + key);
    }
    return httpClients[key]->sendRequest(request);
}

// void NetworkManager::connectWebSocket(const std::string& symbol, IWebSocketObserver* observer) {
//     webSocketClient->addObserver(observer);
//     webSocketClient->connect();
// }

// void NetworkManager::setHttpTimeout(int milliseconds) {
//     httpClient->setTimeout(milliseconds);
// }

// void NetworkManager::setHttpRetryPolicy(int retries) {
//     httpClient->setRetryPolicy(retries);
// }

// void NetworkManager::setWebSocketReconnectPolicy(bool enableAutoReconnect) {
//     // A faire ici 
//     if (enableAutoReconnect) {
//         std::cout << "WebSocket auto-reconnect enabled" << std::endl;
//     }
// }