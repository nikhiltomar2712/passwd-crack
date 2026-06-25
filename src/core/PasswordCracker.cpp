// src/core/PasswordCracker.cpp
#include "PasswordCracker.h"
#include "utils/Logger.h"
#include <fstream>
#include <algorithm>

PasswordCracker::PasswordCracker(const Config& cfg) 
    : config(cfg) {
    
    Logger::info("Initializing Password Security Analysis Framework");
    
    verifyTargetHashes();
    initializeHashAlgorithm();
    initializeAttackMethod();
    initializeGPU();
    
    if (config.hasSessionFile()) {
        restoreSession(config.getSessionFile());
    }
    
    sessionManager = std::make_unique<SessionManager>(config);
    progressTracker = std::make_unique<ProgressTracker>(config);
}

PasswordCracker::~PasswordCracker() {
    saveSession();
    shouldStop = true;
}

void PasswordCracker::verifyTargetHashes() {
    std::ifstream hashFile(config.getTargetFile());
    if (!hashFile.is_open()) {
        throw std::runtime_error("Cannot open target hash file");
    }
    
    std::string line;
    while (std::getline(hashFile, line)) {
        if (!line.empty()) {
            targetHashes.push_back(line);
        }
    }
    
    Logger::info("Loaded " + std::to_string(targetHashes.size()) + " target hashes");
}

void PasswordCracker::initializeHashAlgorithm() {
    hashAlgo = HashFactory::createHashAlgorithm(config.getHashType());
    Logger::info("Initialized hash algorithm: " + hashAlgo->getName());
}

void PasswordCracker::initializeAttackMethod() {
    attackMethod = AttackFactory::createAttackMethod(config.getAttackType(), config);
    Logger::info("Initialized attack method: " + config.getAttackType());
}

void PasswordCracker::initializeGPU() {
    if (config.useGPU()) {
        gpuManager = std::make_unique<GPUManager>(config);
        gpuManager->initialize();
        Logger::info("GPU acceleration enabled");
    }
}

void PasswordCracker::run() {
    Logger::info("Starting password cracking operation");
    
    auto startTime = std::chrono::steady_clock::now();
    
    if (config.getMode() == Mode::BENCHMARK) {
        runBenchmark();
        return;
    }
    
    if (config.getMode() == Mode::ANALYZE) {
        analyzePassword(config.getPasswordToAnalyze());
        return;
    }
    
    std::vector<std::thread> workers;
    int numThreads = config.useGPU() ? 1 : config.getCPUThreads();
    
    for (int i = 0; i < numThreads; ++i) {
        workers.emplace_back([this, i]() {
            crackPasswords();
        });
    }
    
    for (auto& worker : workers) {
        worker.join();
    }
    
    auto endTime = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(endTime - startTime);
    
    Logger::info("Cracking operation completed in " + std::to_string(duration.count()) + " seconds");
    reportResults();
}

void PasswordCracker::crackPasswords() {
    std::vector<std::string> batch;
    
    while (!shouldStop && attackMethod->hasMoreCandidates()) {
        auto candidates = attackMethod->getNextBatch(1000);
        
        for (const auto& candidate : candidates) {
            if (shouldStop) break;
            
            attemptsCount++;
            auto hash = hashAlgo->hash(candidate);
            std::string hashStr = bytesToHex(hash);
            
            for (const auto& targetHash : targetHashes) {
                if (hashStr == targetHash) {
                    std::lock_guard<std::mutex> lock(resultMutex);
                    crackedPasswords[targetHash] = candidate;
                    Logger::info("Cracked: " + targetHash + " -> " + candidate);
                }
            }
            
            if (attemptsCount % 10000 == 0) {
                progressTracker->update(attemptsCount, crackedPasswords.size(), targetHashes.size());
            }
        }
        
        if (crackedPasswords.size() == targetHashes.size()) {
            shouldStop = true;
        }
    }
}

void PasswordCracker::reportResults() {
    Logger::info("=== Results ===");
    Logger::info("Passwords cracked: " + std::to_string(crackedPasswords.size()) + 
                 "/" + std::to_string(targetHashes.size()));
    
    for (const auto& [hash, password] : crackedPasswords) {
        Logger::info("Hash: " + hash + " -> Password: " + password);
    }
    
    if (config.getOutputFile().empty()) {
        std::string outputFile = config.getOutputFile();
        std::ofstream out(outputFile);
        for (const auto& [hash, password] : crackedPasswords) {
            out << hash << ":" << password << std::endl;
        }
        Logger::info("Results saved to " + outputFile);
    }
}

void PasswordCracker::saveSession() {
    if (sessionManager) {
        sessionManager->save(crackedPasswords, attemptsCount);
    }
}

void PasswordCracker::restoreSession(const std::string& sessionFile) {
    if (sessionManager) {
        sessionManager->restore(sessionFile);
    }
}
