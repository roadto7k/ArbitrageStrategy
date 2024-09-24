#include <gtest/gtest.h>
#include "HttpClient.h"
#include "CurlHttpClient.h"

TEST(HttpClientIntegrationTest, RealServerRequest) {
    CurlHttpClient httpClient;

    HttpRequest request;
    request.url = "http://localhost:8080/test";
    request.method = "GET";

    HttpResponse response = httpClient.sendRequest(request);

    EXPECT_EQ(response.statusCode, 200);
    EXPECT_EQ(response.body, "{\"success\": true}");
}