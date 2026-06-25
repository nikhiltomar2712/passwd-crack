// src/utils/ProgressTracker.h
#pragma once
#include <chrono>
#include <iostream>
#include <iomanip>
#include <cmath>

class ProgressTracker {
private:
    std::chrono::steady_clock::time_point startTime;
    uint64_t lastUpdateAttempts;
    std::chrono::steady_clock::time_point lastUpdateTime;
    Config config;
    
public:
    explicit ProgressTracker(const Config& cfg) : config(cfg) {
        startTime = std::chrono::steady_clock::now();
        lastUpdateTime = startTime;
        lastUpdateAttempts = 0;
    }
    
    void update(uint64_t attempts, size_t cracked, size_t total) {
        auto now = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - lastUpdateTime);
        
        if (elapsed.count() < 5) return; // Update every 5 seconds
        
        auto totalElapsed = std::chrono::duration_cast<std::chrono::seconds>(now - startTime);
        double speed = (attempts - lastUpdateAttempts) / static_cast<double>(elapsed.count());
        
        std::cout << "\rProgress: " << cracked << "/" << total << " cracked | "
                  << "Speed: " << formatSpeed(speed) << " | "
                  << "Attempts: " << attempts << " | "
                  << "Elapsed: " << formatTime(totalElapsed.count()) << std::flush;
        
        lastUpdateAttempts = attempts;
        lastUpdateTime = now;
    }
    
private:
    std::string formatSpeed(double attemptsPerSecond) {
        if (attemptsPerSecond > 1e9) {
            return std::to_string(static_cast<int>(attemptsPerSecond / 1e9)) + " GH/s";
        } else if (attemptsPerSecond > 1e6) {
            return std::to_string(static_cast<int>(attemptsPerSecond / 1e6)) + " MH/s";
        } else if (attemptsPerSecond > 1e3) {
            return std::to_string(static_cast<int>(attemptsPerSecond / 1e3)) + " KH/s";
        }
        return std::to_string(static_cast<int>(attemptsPerSecond)) + " H/s";
    }
    
    std::string formatTime(long seconds) {
        long hours = seconds / 3600;
        long minutes = (seconds % 3600) / 60;
        long secs = seconds % 60;
        
        std::stringstream ss;
        if (hours > 0) ss << hours << "h ";
        if (minutes > 0) ss << minutes << "m ";
        ss << secs << "s";
        return ss.str();
    }
};
