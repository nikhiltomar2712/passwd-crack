// src/utils/Config.h
#pragma once
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <iostream>
#include <yaml-cpp/yaml.h>

enum class Mode {
    CRACK,
    BENCHMARK,
    ANALYZE
};

class Config {
private:
    std::map<std::string, std::string> options;
    
public:
    static Config parse(int argc, char* argv[]);
    static void printHelp();
    static bool confirmEthicalUse();
    
    // Getters
    std::string getAttackType() const;
    std::string getHashType() const;
    std::string getTargetFile() const;
    std::vector<std::string> getWordlists() const;
    std::vector<std::string> getRuleFiles() const;
    std::string getMask() const;
    std::string getCharset() const;
    size_t getMinLength() const;
    size_t getMaxLength() const;
    int getCPUThreads() const;
    bool useGPU() const;
    std::vector<int> getGPUDevices() const;
    std::string getOutputFile() const;
    std::string getSessionFile() const;
    Mode getMode() const;
    int getLogLevel() const;
    bool showHelp() const;
    bool needsEthicalConfirmation() const;
    bool hasSessionFile() const;
    std::string getPasswordToAnalyze() const;
    
private:
    void loadFromYAML(const std::string& filename);
};
