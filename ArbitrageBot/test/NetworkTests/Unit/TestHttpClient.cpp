#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "HttpClient.h"

// Mock class for HttpClient
class MockHttpClient : public HttpClient {
public:
    MOCK_METHOD(HttpResponse, sendRequest, (const HttpRequest& request), (override));
    MOCK_METHOD(void, setTimeout, (int milliseconds), (override));
    MOCK_METHOD(void, setRetryPolicy, (int retries), (override));
};

TEST(HttpClientTest, SendRequestTest) {
    MockHttpClient mockHttpClient;

    HttpRequest request;
    request.url = "http://example.com/api";
    request.method = "GET";

    HttpResponse expectedResponse;
    expectedResponse.statusCode = 200;
    expectedResponse.body = "{\"success\": true}";

    EXPECT_CALL(mockHttpClient, sendRequest(request))
        .WillOnce(testing::Return(expectedResponse));

    auto response = mockHttpClient.sendRequest(request);

    EXPECT_EQ(response.statusCode, 200);
    EXPECT_EQ(response.body, "{\"success\": true}");
}

TEST(HttpClientTest, TimeoutTest) {
    MockHttpClient mockHttpClient;

    EXPECT_CALL(mockHttpClient, setTimeout(5000)).Times(1);

    mockHttpClient.setTimeout(5000);
}
