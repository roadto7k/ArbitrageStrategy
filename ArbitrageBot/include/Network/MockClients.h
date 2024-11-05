#pragma once
#include <iostream>
#include <vector>
#include <algorithm>
#include "Http.h"
#include "HttpClient.h"
#include "IWebSocketClient.h"

class MockHttpClient : public HttpClient {
public:
    MockHttpClient() : timeout(0), retryCount(0) {}

    HttpResponse sendRequest(const HttpRequest& request) override {
        std::cout << "MockHttpClient: Sending HTTP request to URL: " << request.url 
                  << " with method: " << request.method 
                  << " and body: " << request.body << std::endl;

        HttpResponse response;
        response.statusCode = 200;
        response.body = "{\"mock\": \"response\"}";
        return response;
    }

    void setTimeout(int milliseconds) override {
        timeout = milliseconds;
        std::cout << "MockHttpClient: Setting timeout to " << milliseconds << " milliseconds." << std::endl;
    }

    void setRetryPolicy(int retries) override {
        retryCount = retries;
        std::cout << "MockHttpClient: Setting retry policy to " << retries << " retries." << std::endl;
    }

private:
    int timeout;
    int retryCount;
};

class MockWebSocketClient : public IWebSocketClient {
public:
    MockWebSocketClient() : isConnected(false) {}

    void connect() override {
        isConnected = true;
        std::cout << "MockWebSocketClient: WebSocket connected." << std::endl;
        notifyObservers("Mock message: connected");
    }

    void disconnect() override {
        isConnected = false;
        std::cout << "MockWebSocketClient: WebSocket disconnected." << std::endl;
        notifyConnectionClosed();
    }

    void sendMessage(const std::string& message) override {
        std::cout << "MockWebSocketClient: Sending WebSocket message: " << message << std::endl;
    }

    void addObserver(IWebSocketObserver* observer) override {
        std::cout << "MockWebSocketClient: Adding observer." << std::endl;
        observers.push_back(observer);
    }

    void removeObserver(IWebSocketObserver* observer) override {
        std::cout << "MockWebSocketClient: Removing observer." << std::endl;
        observers.erase(std::remove(observers.begin(), observers.end(), observer), observers.end());
    }

private:
    bool isConnected;
    std::vector<IWebSocketObserver*> observers;

    void notifyObservers(const std::string& message) {
        for (auto observer : observers) {
            observer->onMessageReceived(message);
        }
    }

    void notifyConnectionClosed() {
        for (auto observer : observers) {
            observer->onConnectionClosed();
        }
    }
};