// tests/test_attack_methods.cpp
#include <gtest/gtest.h>
#include "attack/BruteForceAttack.h"
#include "attack/DictionaryAttack.h"

TEST(BruteForceTest, GeneratesCorrectSequence) {
    Config config;
    config.setOption("charset", "ab");
    config.setOption("min_length", "1");
    config.setOption("max_length", "2");
    
    BruteForceAttack attack;
    attack.initialize(config);
    
    std::vector<std::string> expected = {"a", "b", "aa", "ab", "ba", "bb"};
    std::vector<std::string> actual;
    
    for (int i = 0; i < 6; i++) {
        actual.push_back(attack.getNextCandidate());
    }
    
    EXPECT_EQ(actual, expected);
    EXPECT_FALSE(attack.hasMoreCandidates());
}

TEST(DictionaryTest, AppliesRules) {
    // Test dictionary attack with rules
}
