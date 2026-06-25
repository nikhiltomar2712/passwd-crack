// src/attack/DictionaryAttack.h
#pragma once
#include "IAttackMethod.h"
#include <vector>
#include <fstream>
#include <algorithm>
#include <random>

class DictionaryAttack : public IAttackMethod {
private:
    std::vector<std::string> dictionary;
    std::vector<std::string> rules;
    size_t currentIndex;
    size_t currentRuleIndex;
    std::string currentWord;
    
public:
    void initialize(const Config& config) override {
        loadDictionary(config.getWordlists());
        loadRules(config.getRuleFiles());
        currentIndex = 0;
        currentRuleIndex = 0;
        
        if (!dictionary.empty()) {
            currentWord = dictionary[0];
        }
        
        // Sort by probability (common passwords first)
        sortDictionaryByProbability();
    }
    
    std::string getNextCandidate() override {
        if (!hasMoreCandidates()) return "";
        
        std::string candidate = applyRules(currentWord, currentRuleIndex);
        
        currentRuleIndex++;
        if (currentRuleIndex >= rules.size()) {
            currentRuleIndex = 0;
            currentIndex++;
            if (currentIndex < dictionary.size()) {
                currentWord = dictionary[currentIndex];
            }
        }
        
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
        return currentIndex < dictionary.size();
    }
    
    double getProgress() const override {
        return static_cast<double>(currentIndex) / dictionary.size();
    }
    
    std::string getCurrentState() const override {
        return currentWord;
    }
    
    void reset() override {
        currentIndex = 0;
        currentRuleIndex = 0;
        if (!dictionary.empty()) {
            currentWord = dictionary[0];
        }
    }
    
private:
    void loadDictionary(const std::vector<std::string>& wordlists) {
        for (const auto& filepath : wordlists) {
            std::ifstream file(filepath);
            std::string word;
            while (std::getline(file, word)) {
                if (!word.empty()) {
                    dictionary.push_back(word);
                }
            }
        }
        
        // Remove duplicates
        std::sort(dictionary.begin(), dictionary.end());
        dictionary.erase(std::unique(dictionary.begin(), dictionary.end()), dictionary.end());
    }
    
    void loadRules(const std::vector<std::string>& ruleFiles) {
        rules = {
            "",                    // Original word
            "capitalize",          // First letter uppercase
            "uppercase",           // All uppercase
            "lowercase",           // All lowercase
            "reverse",             // Reverse word
            "append_1",            // Append 1
            "append_123",          // Append 123
            "append_!",            // Append !
            "prepend_1",           // Prepend 1
            "leet_basic"           // Basic leet speak
        };
    }
    
    std::string applyRules(const std::string& word, size_t ruleIndex) {
        if (ruleIndex >= rules.size()) return word;
        
        std::string transformed = word;
        const std::string& rule = rules[ruleIndex];
        
        if (rule == "capitalize") {
            if (!transformed.empty()) {
                transformed[0] = std::toupper(transformed[0]);
            }
        } else if (rule == "uppercase") {
            std::transform(transformed.begin(), transformed.end(), 
                          transformed.begin(), ::toupper);
        } else if (rule == "lowercase") {
            std::transform(transformed.begin(), transformed.end(), 
                          transformed.begin(), ::tolower);
        } else if (rule == "reverse") {
            std::reverse(transformed.begin(), transformed.end());
        } else if (rule == "append_1") {
            transformed += "1";
        } else if (rule == "append_123") {
            transformed += "123";
        } else if (rule == "append_!") {
            transformed += "!";
        } else if (rule == "prepend_1") {
            transformed = "1" + transformed;
        } else if (rule == "leet_basic") {
            applyLeetSpeak(transformed);
        }
        
        return transformed;
    }
    
    void applyLeetSpeak(std::string& word) {
        for (char& c : word) {
            switch (std::tolower(c)) {
                case 'e': c = '3'; break;
                case 'a': c = '4'; break;
                case 's': c = '5'; break;
                case 'i': c = '1'; break;
                case 'o': c = '0'; break;
                case 't': c = '7'; break;
            }
        }
    }
    
    void sortDictionaryByProbability() {
        // Simple probability sorting based on length and complexity
        std::sort(dictionary.begin(), dictionary.end(), 
                 [](const std::string& a, const std::string& b) {
                     // Shorter passwords are more common
                     if (a.length() != b.length()) {
                         return a.length() < b.length();
                     }
                     // Lowercase only passwords are more common
                     bool a_lower = std::all_of(a.begin(), a.end(), ::islower);
                     bool b_lower = std::all_of(b.begin(), b.end(), ::islower);
                     if (a_lower != b_lower) return a_lower;
                     
                     return a < b;
                 });
    }
};
