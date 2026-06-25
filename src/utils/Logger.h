// src/utils/Logger.h
#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <memory>
#include <mutex>

enum class LogLevel {
    DEBUG,
    INFO,
    WARNING,
    ERROR
};

class Logger {
private:
    static LogLevel currentLevel;
    static std::ofstream logFile;
    static std::mutex logMutex;
    
public:
    static void init(int level) {
        currentLevel = static_cast<LogLevel>(level);
        logFile.open("password_cracker.log", std::ios::app);
    }
    
    static void debug(const std::string& message) {
        log(LogLevel::DEBUG, message);
    }
    
    static void info(const std::string& message) {
        log(LogLevel::INFO, message);
    }
    
    static void warning(const std::string& message) {
        log(LogLevel::WARNING, message);
    }
    
    static void error(const std::string& message) {
        log(LogLevel::ERROR, message);
    }
    
private:
    static void log(LogLevel level, const std::string& message) {
        if (level < currentLevel) return;
        
        std::lock_guard<std::mutex> lock(logMutex);
        
        std::string prefix;
        switch (level) {
            case LogLevel::DEBUG: prefix = "[DEBUG] "; break;
            case LogLevel::INFO: prefix = "[INFO] "; break;
            case LogLevel::WARNING: prefix = "[WARNING] "; break;
            case LogLevel::ERROR: prefix = "[ERROR] "; break;
        }
        
        std::cout << prefix << message << std::endl;
        if (logFile.is_open()) {
            logFile << prefix << message << std::endl;
        }
    }
};
