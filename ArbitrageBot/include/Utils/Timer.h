#pragma once
#include <chrono>

class Timer {
public:
    Timer() {
        start = std::chrono::high_resolution_clock::now();
    }

    void reset() {
        start = std::chrono::high_resolution_clock::now();
    }

    double elapsedMilliseconds() const {
        auto now = std::chrono::high_resolution_clock::now();
        return std::chrono::duration_cast<std::chrono::milliseconds>(now - start).count();
    }

    double elapsedSeconds() const {
        auto now = std::chrono::high_resolution_clock::now();
        return std::chrono::duration_cast<std::chrono::seconds>(now - start).count();
    }

private:
    std::chrono::high_resolution_clock::time_point start;
};