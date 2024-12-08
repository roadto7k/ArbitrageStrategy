#ifndef ISTRATEGY_H
#define ISTRATEGY_H

#include <map>
#include <string>
#include <vector>
#include <memory>
#include "TriangleGenerator.h"

class IStrategy {
public:
    virtual bool evaluate(const std::map<std::string, float>& priceCache, const std::vector<TriangleGenerator::Triangle>& triangles) = 0;
    virtual std::string generateActions() const = 0;
    virtual ~IStrategy() = default;
};

#endif