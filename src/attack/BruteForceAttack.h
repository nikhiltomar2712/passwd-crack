// src/attack/BruteForceAttack.h
#pragma once
#include "IAttackMethod.h"
#include <string>
#include <vector>
#include <cmath>

class BruteForceAttack : public IAttackMethod {
private:
    std::string charset;
    size_t minLength;
    size_t maxLength;
    std::string mask;
    
    std::vector<size_t> currentIndices;
    size_t currentLength;
    uint64_t totalCombinations;
    uint64_t currentCombination;
    
public:
    void initialize(const Config& config) override {
        charset = config.getCharset();
        minLength = config.getMinLength();
        maxLength = config.getMaxLength();
        mask = config.getMask();
        
        currentLength = minLength;
        currentIndices.resize(currentLength, 0);
        currentCombination = 0;
        
        totalCombinations = calculateTotalCombinations();
    }
    
    std::string getNextCandidate() override {
        if (!hasMoreCandidates()) return "";
        
        std::string candidate;
        for (size_t idx : currentIndices) {
            candidate += charset[idx];
        }
        
        incrementIndices();
        currentCombination++;
        return candidate;
    }
    
    std::vector<std::string> getNextBatch(size_t count) override {
        std::vector<std::string> batch;
        batch.reserve(count);
        
        for (size_t i = 0; i < count && hasMoreCandidates(); ++i) {
            batch.push_back(getNextCandidate());
        }
        
        return batch;
    }
    
    bool hasMoreCandidates() override {
        return currentLength <= maxLength;
    }
    
    double getProgress() const override {
        return static_cast<double>(currentCombination) / totalCombinations;
    }
    
    std::string getCurrentState() const override {
        std::string state;
        for (size_t idx : currentIndices) {
            state += charset[idx];
        }
        return state;
    }
    
    void reset() override {
        currentLength = minLength;
        currentIndices.resize(currentLength, 0);
        currentCombination = 0;
    }
    
private:
    uint64_t calculateTotalCombinations() const {
        uint64_t total = 0;
        for (size_t len = minLength; len <= maxLength; ++len) {
            total += static_cast<uint64_t>(std::pow(charset.size(), len));
        }
        return total;
    }
    
    void incrementIndices() {
        for (int i = currentIndices.size() - 1; i >= 0; --i) {
            currentIndices[i]++;
            if (currentIndices[i] < charset.size()) {
                return;
            }
            currentIndices[i] = 0;
        }
        
        // All indices wrapped around
        currentLength++;
        if (currentLength <= maxLength) {
            currentIndices.resize(currentLength, 0);
        }
    }
};
