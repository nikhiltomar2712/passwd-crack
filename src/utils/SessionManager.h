// src/utils/SessionManager.h
#pragma once
#include <string>
#include <map>
#include <fstream>
#include <nlohmann/json.hpp>
#include <chrono>

using json = nlohmann::json;

class SessionManager {
private:
    std::string sessionFile;
    Config config;
    
public:
    SessionManager(const Config& cfg) : config(cfg) {
        auto now = std::chrono::system_clock::now();
        auto time_t = std::chrono::system_clock::to_time_t(now);
        std::stringstream ss;
        ss << "session_" << std::put_time(std::localtime(&time_t), "%Y%m%d_%H%M%S") << ".json";
        sessionFile = ss.str();
    }
    
    void save(const std::map<std::string, std::string>& crackedPasswords, 
              uint64_t attempts) {
        json session;
        session["attack_type"] = config.getAttackType();
        session["hash_type"] = config.getHashType();
        session["attempts"] = attempts;
        
        json cracked = json::array();
        for (const auto& [hash, password] : crackedPasswords) {
            json entry;
            entry["hash"] = hash;
            entry["password"] = password;
            cracked.push_back(entry);
        }
        session["cracked_passwords"] = cracked;
        
        session["timestamp"] = std::chrono::system_clock::now().time_since_epoch().count();
        
        std::ofstream file(sessionFile);
        file << session.dump(4);
    }
    
    void restore(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            throw std::runtime_error("Cannot open session file: " + filename);
        }
        
        json session;
        file >> session;
        
        // Restore state from session
        Logger::info("Restored session from " + filename);
        Logger::info("Previous attempts: " + std::to_string(session["attempts"].get<uint64_t>()));
    }
};
