#ifndef ISTRATEGY_H
#define ISTRATEGY_H

// #include "OrderCommand.h"
#include <map>
#include <memory>
class IStrategy {
public:
    virtual bool evaluate(const std::map<std::string, float>& priceCache) = 0;
    virtual std::string generateActions() const = 0;
    virtual ~IStrategy() = default;
};

#endif