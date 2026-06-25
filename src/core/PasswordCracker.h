// src/core/PasswordCracker.h
#pragma once
#include <memory>
#include <vector>
#include <thread>
#include <atomic>
#include "hash/IHashAlgorithm.h"
#include "attack/IAttackMethod.h"
#include "gpu/GPUManager.h"
#include "utils/SessionManager.h"
#include "utils/ProgressTracker.h"

class PasswordCracker {
private:
    Config config;
    std::unique_ptr<IHashAlgorithm> hashAlgo;
    std::unique_ptr<IAttackMethod> attackMethod;
    std::unique_ptr<GPUManager> gpuManager;
    std::unique_ptr<SessionManager> sessionManager;
    std::unique_ptr<ProgressTracker> progressTracker;
    
    std::vector<std::string> targetHashes;
    std::unordered_map<std::string, std::string> crackedPasswords;
    
    std::atomic<bool> shouldStop{false};
    std::atomic<uint64_t> attemptsCount{0};
    
public:
    explicit PasswordCracker(const Config& cfg);
    ~PasswordCracker();
    
    void run();
    void stop();
    void saveSession();
    void restoreSession(const std::string& sessionFile);
    
private:
    void initializeHashAlgorithm();
    void initializeAttackMethod();
    void initializeGPU();
    void crackPasswords();
    void verifyTargetHashes();
    void reportResults();
};
