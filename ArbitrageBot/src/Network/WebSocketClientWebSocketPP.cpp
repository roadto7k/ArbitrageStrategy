
#include <iostream>
#include <thread>
#include <memory>
#include "WebSocketClientWebSocketPP.h"
#include <websocketpp/config/asio_client.hpp>
// #include <websocketpp/config/asio_tls_client.hpp>
#include <websocketpp/client.hpp>
#include <websocketpp/config/asio.hpp>
#include <boost/asio/ssl/context.hpp>
#include <boost/asio/ssl.hpp>

WebSocketClientWebSocketPP::WebSocketClientWebSocketPP(const std::string& uri) 
    : uri(uri), isConnected(false) {
    // Initialize websocketpp client
    client.init_asio();
    if (uri.substr(0, 6) == "wss://") {
        client.set_tls_init_handler(std::bind(&WebSocketClientWebSocketPP::onTlsInit, this, std::placeholders::_1));
    }

    client.set_open_handler([this](websocketpp::connection_hdl hdl) { onOpen(hdl); });
    client.set_close_handler([this](websocketpp::connection_hdl hdl) { onClose(hdl); });
    client.set_message_handler([this](websocketpp::connection_hdl hdl, Client::message_ptr msg) { onMessage(hdl, msg); });
}

WebSocketClientWebSocketPP::~WebSocketClientWebSocketPP() {
    disconnect();
}

void WebSocketClientWebSocketPP::connect() {
    websocketpp::lib::error_code ec;
    Client::connection_ptr con = client.get_connection(uri, ec);

    if (ec) {
        std::cerr << "Error creating connection: " << ec.message() << std::endl;
        return;
    }

    connectionHandle = con->get_handle();
    client.connect(con);

    std::thread([this]() { client.run(); }).detach();
}

void WebSocketClientWebSocketPP::disconnect() {
    if (isConnected) {
        websocketpp::lib::error_code ec;
        client.close(connectionHandle, websocketpp::close::status::going_away, "Client disconnecting", ec);
        if (ec) {
            std::cerr << "Error closing connection: " << ec.message() << std::endl;
        }
        isConnected = false;
    }
}

void WebSocketClientWebSocketPP::sendMessage(const std::string& message) {
    if (isConnected) {
        websocketpp::lib::error_code ec;
        client.send(connectionHandle, message, websocketpp::frame::opcode::text, ec);
        if (ec) {
            std::cerr << "Error sending message: " << ec.message() << std::endl;
        }
    } else {
        std::cerr << "Cannot send message, WebSocket not connected." << std::endl;
    }
}

void WebSocketClientWebSocketPP::addObserver(IWebSocketObserver* observer) {
    std::lock_guard<std::mutex> lock(observerMutex);
    observers.push_back(observer);
}

void WebSocketClientWebSocketPP::removeObserver(IWebSocketObserver* observer) {
    std::lock_guard<std::mutex> lock(observerMutex);
    observers.erase(std::remove(observers.begin(), observers.end(), observer), observers.end());
}

void WebSocketClientWebSocketPP::onOpen(websocketpp::connection_hdl hdl) {
    isConnected = true;
    std::cout << "WebSocket connection opened." << std::endl;
}

void WebSocketClientWebSocketPP::onClose(websocketpp::connection_hdl hdl) {
    isConnected = false;
    std::cout << "WebSocket connection closed." << std::endl;
    notifyConnectionClosed();
}

void WebSocketClientWebSocketPP::onMessage(websocketpp::connection_hdl hdl, Client::message_ptr msg) {
    std::string message = msg->get_payload();
    std::cout << "Message received: " << message << std::endl;
    notifyObservers(message);
}

void WebSocketClientWebSocketPP::notifyObservers(const std::string& message) {
    std::lock_guard<std::mutex> lock(observerMutex);
    for (auto observer : observers) {
        if (observer) {
            observer->onMessageReceived(message);
        }
    }
}

void WebSocketClientWebSocketPP::notifyConnectionClosed() {
    std::lock_guard<std::mutex> lock(observerMutex);
    for (auto observer : observers) {
        if (observer) {
            observer->onConnectionClosed();
        }
    }
}

std::shared_ptr<boost::asio::ssl::context> WebSocketClientWebSocketPP::onTlsInit(websocketpp::connection_hdl hdl) {
    // auto ctx = std::make_shared<boost::asio::ssl::context>(boost::asio::ssl::context::tlsv12);
    auto ctx = websocketpp::lib::make_shared<boost::asio::ssl::context>(boost::asio::ssl::context::tlsv12);
    try {
        ctx->set_options(boost::asio::ssl::context::default_workarounds |
                         boost::asio::ssl::context::no_sslv2 |
                         boost::asio::ssl::context::no_sslv3 |
                         boost::asio::ssl::context::single_dh_use);

        // Option 1: Utiliser les chemins par défaut pour les certificats
        // ctx->set_default_verify_paths();

        // Option 2: Spécifier un fichier de certificats manuellement (à utiliser si l'option 1 échoue)
        // ctx->load_verify_file("/path/to/certificates.pem");

        // Désactiver la vérification des certificats pour le test (non recommandé en production)
        // ctx->set_verify_mode(boost::asio::ssl::verify_none);

        ctx->set_verify_mode(boost::asio::ssl::verify_peer);

    } catch (std::exception& e) {
        std::cerr << "Error in TLS initialization: " << e.what() << std::endl;
    }

    return ctx;
}