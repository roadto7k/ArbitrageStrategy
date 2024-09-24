#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "IWebSocketClient.h"
#include "WebSocketClientWebSocketPP.h"

class MockWebSocketObserver : public IWebSocketObserver {
public:
    MOCK_METHOD(void, onMessageReceived, (const std::string& message), (override));
    MOCK_METHOD(void, onConnectionClosed, (), (override));
};

TEST(WebSocketClientTest, ConnectAndReceiveMessage) {
    MockWebSocketObserver observer;

    WebSocketClientWebSocketPP client("ws://localhost:9000");
    client.addObserver(&observer);

    EXPECT_CALL(observer, onMessageReceived("Test message")).Times(1);

    client.connect();

    client.onMessageReceived("Test message");

    client.disconnect();
}