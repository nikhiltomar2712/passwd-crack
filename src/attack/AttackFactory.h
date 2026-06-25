// src/attack/AttackFactory.h
#pragma once
#include "IAttackMethod.h"
#include "BruteForceAttack.h"
#include "DictionaryAttack.h"
#include <memory>
#include <string>

class AttackFactory {
public:
    static std::unique_ptr<IAttackMethod> createAttackMethod(
        const std::string& type, const Config& config) {
        
        std::unique_ptr<IAttackMethod> attack;
        
        if (type == "bruteforce") {
            attack = std::make_unique<BruteForceAttack>();
        } else if (type == "dictionary") {
            attack = std::make_unique<DictionaryAttack>();
        } else if (type == "hybrid") {
            attack = std::make_unique<HybridAttack>();
        } else {
            throw std::runtime_error("Unknown attack type: " + type);
        }
        
        attack->initialize(config);
        return attack;
    }
};
