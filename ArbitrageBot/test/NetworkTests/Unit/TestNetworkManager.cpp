#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "NetworkManager.h"
#include "HttpClient.h"
#include "WebSocketClient.h"

class MockHttpClient : public HttpClient {
public:
    MOCK_METHOD(HttpResponse, sendRequest, (const HttpRequest& request), (override));
    MOCK_METHOD(void, setTimeout, (int milliseconds), (override));
    MOCK_METHOD(void, setRetryPolicy, (int retries), (override));
};

class MockWebSocketClient : public IWebSocketClient {
public:
    MOCK_METHOD(void, connect, (), (override));
    MOCK_METHOD(void, disconnect, (), (override));
    MOCK_METHOD(void, sendMessage, (const std::string& message), (override));
    MOCK_METHOD(void, addObserver, (IWebSocketObserver* observer), (override));
    MOCK_METHOD(void, removeObserver, (IWebSocketObserver* observer), (override));
};

TEST(NetworkManagerTest, MakeHttpRequestTest) {
    MockHttpClient mockHttpClient;
    MockWebSocketClient mockWebSocketClient;

    HttpRequest request;
    request.url = "http://example.com";
    request.method = "GET";

    HttpResponse expectedResponse;
    expectedResponse.statusCode = 200;
    expectedResponse.body = "OK";

    EXPECT_CALL(mockHttpClient, sendRequest(request))
        .WillOnce(testing::Return(expectedResponse));

    NetworkManager networkManager(std::make_unique<MockHttpClient>(mockHttpClient), 
                                  std::make_unique<MockWebSocketClient>(mockWebSocketClient));

    auto response = networkManager.makeHttpRequest(request);

    EXPECT_EQ(response.statusCode, 200);
    EXPECT_EQ(response.body, "OK");
}

TEST(NetworkManagerTest, WebSocketConnectTest) {
    MockHttpClient mockHttpClient;
    MockWebSocketClient mockWebSocketClient;

    EXPECT_CALL(mockWebSocketClient, connect()).Times(1);

    NetworkManager networkManager(std::make_unique<MockHttpClient>(mockHttpClient), 
                                  std::make_unique<MockWebSocketClient>(mockWebSocketClient));

    networkManager.connectWebSocket("testSymbol", nullptr);

}