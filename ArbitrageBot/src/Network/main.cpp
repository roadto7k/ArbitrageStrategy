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
        // Création des objets HttpClient et WebSocketClient
        auto httpClient = std::make_unique<CurlHttpClient>();
        auto webSocketClient = std::make_unique<WebSocketClientWebSocketPP>("wss://ws.ifelse.io");

        // Création du NetworkManager avec les clients
        NetworkManager networkManager(std::move(httpClient), std::move(webSocketClient));

        // Tester une requête HTTP GET vers httpbin.org
        HttpRequest httpRequest;
        httpRequest.url = "https://httpbin.org/get";
        httpRequest.method = "GET";
        
        HttpResponse httpResponse = networkManager.makeHttpRequest(httpRequest);
        std::cout << "HTTP GET Response: " << httpResponse.statusCode << " " << httpResponse.body << std::endl;

        // Tester une requête HTTP POST vers httpbin.org
        HttpRequest postRequest;
        postRequest.url = "https://httpbin.org/post";
        postRequest.method = "POST";
        postRequest.body = "{\"test\": \"data\"}";
        postRequest.headers["Content-Type"] = "application/json";

        HttpResponse postResponse = networkManager.makeHttpRequest(postRequest);
        std::cout << "HTTP POST Response: " << postResponse.statusCode << " " << postResponse.body << std::endl;

        // Configuration du timeout HTTP
        networkManager.setHttpTimeout(5000); // Timeout à 5 secondes

        // Configuration du retry policy HTTP
        networkManager.setHttpRetryPolicy(3); // 3 tentatives en cas d'échec

        // Connexion WebSocket et envoi de messages
        TestWebSocketObserver wsObserver;
        networkManager.connectWebSocket("test-symbol", &wsObserver);

        std::this_thread::sleep_for(std::chrono::seconds(1));  // Attendre un moment que la connexion WebSocket se fasse

        // Envoyer un message sur WebSocket
        networkManager.setWebSocketReconnectPolicy(true); // Option auto-reconnect activée

        std::this_thread::sleep_for(std::chrono::seconds(10));  // Garder la connexion ouverte un peu plus longtemps
    }
    catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}