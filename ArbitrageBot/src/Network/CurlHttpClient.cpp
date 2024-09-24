#include "HttpClient.h"
#include <iostream>
#include <sstream>

CurlHttpClient::CurlHttpClient() : curlHandle(curl_easy_init()), timeout(3000), retryCount(3) {
    if (!curlHandle) {
        throw std::runtime_error("Failed to initialize CURL");
    }
}

CurlHttpClient::~CurlHttpClient() {
    if (curlHandle) {
        curl_easy_cleanup(curlHandle);
    }
}

size_t writeCallback(void* contents, size_t size, size_t nmemb, std::string* userData) {
    size_t totalSize = size * nmemb;
    userData->append((char*)contents, totalSize);
    return totalSize;
}

HttpResponse CurlHttpClient::sendRequest(const HttpRequest& request) {
    if (!curlHandle) {
        return {500, "", {}, "CURL handle is not initialized"};
    }

    curl_easy_reset(curlHandle);

    curl_easy_setopt(curlHandle, CURLOPT_URL, request.url.c_str());

    if (request.method == "POST") {
        curl_easy_setopt(curlHandle, CURLOPT_POST, 1L);
        curl_easy_setopt(curlHandle, CURLOPT_POSTFIELDS, request.body.c_str());
    } else if (request.method == "PUT") {
        curl_easy_setopt(curlHandle, CURLOPT_CUSTOMREQUEST, "PUT");
        curl_easy_setopt(curlHandle, CURLOPT_POSTFIELDS, request.body.c_str());
    } else if (request.method == "DELETE") {
        curl_easy_setopt(curlHandle, CURLOPT_CUSTOMREQUEST, "DELETE");
    }

    struct curl_slist* headersList = nullptr;
    for (const auto& header : request.headers) {
        std::string headerStr = header.first + ": " + header.second;
        headersList = curl_slist_append(headersList, headerStr.c_str());
    }
    if (headersList) {
        curl_easy_setopt(curlHandle, CURLOPT_HTTPHEADER, headersList);
    }

    curl_easy_setopt(curlHandle, CURLOPT_TIMEOUT_MS, timeout);

    std::string responseData;
    curl_easy_setopt(curlHandle, CURLOPT_WRITEFUNCTION, writeCallback);
    curl_easy_setopt(curlHandle, CURLOPT_WRITEDATA, &responseData);

    CURLcode res;
    int retriesLeft = retryCount;
    HttpResponse httpResponse;

    do {
        res = curl_easy_perform(curlHandle);
        if (res != CURLE_OK) {
            --retriesLeft;
        } else {
            break;
        }
    } while (retriesLeft > 0);

    if (res != CURLE_OK) {
        httpResponse = processCurlResponse(res, responseData);
    } else {
        long httpCode = 0;
        curl_easy_getinfo(curlHandle, CURLINFO_RESPONSE_CODE, &httpCode);
        httpResponse.statusCode = static_cast<int>(httpCode);
        httpResponse.body = responseData;
    }

    if (headersList) {
        curl_slist_free_all(headersList);
    }

    return httpResponse;
}

void CurlHttpClient::setTimeout(int milliseconds) {
    timeout = milliseconds;
}

void CurlHttpClient::setRetryPolicy(int retries) {
    retryCount = retries;
}

HttpResponse CurlHttpClient::processCurlResponse(CURLcode curlCode, const std::string& responseData) {
    HttpResponse response;
    response.statusCode = 500;
    response.body = responseData;
    response.errorMessage = curl_easy_strerror(curlCode);
    return response;
}
