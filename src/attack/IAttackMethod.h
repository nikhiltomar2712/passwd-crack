// src/attack/IAttackMethod.h
#pragma once
#include <string>
#include <vector>
#include "utils/Config.h"

class IAttackMethod {
public:
    virtual ~IAttackMethod() = default;
    
    virtual void initialize(const Config& config) = 0;
    virtual std::string getNextCandidate() = 0;
    virtual std::vector<std::string> getNextBatch(size_t count) = 0;
    virtual bool hasMoreCandidates() = 0;
    virtual void reset() = 0;
    virtual double getProgress() const = 0;
    virtual std::string getCurrentState() const = 0;
};
