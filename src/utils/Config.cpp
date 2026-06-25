// src/utils/Config.cpp
#include "Config.h"
#include <cxxopts.hpp>
#include <filesystem>

Config Config::parse(int argc, char* argv[]) {
    Config config;
    
    cxxopts::Options options("password-cracker", 
        "Educational Password Security Analysis Framework");
    
    options.add_options()
        ("a,attack-type", "Attack type", cxxopts::value<std::string>())
        ("H,hash-type", "Hash algorithm", cxxopts::value<std::string>())
        ("t,target", "Target hashes file", cxxopts::value<std::string>())
        ("w,wordlist", "Dictionary file(s)", cxxopts::value<std::vector<std::string>>())
        ("r,rules", "Rules file(s)", cxxopts::value<std::vector<std::string>>())
        ("m,mask", "Mask pattern", cxxopts::value<std::string>())
        ("c,charset", "Character set", cxxopts::value<std::string>())
        ("min-length", "Minimum password length", cxxopts::value<int>())
        ("max-length", "Maximum password length", cxxopts::value<int>())
        ("threads", "CPU threads", cxxopts::value<int>()->default_value("4"))
        ("gpu", "Enable GPU acceleration", cxxopts::value<bool>()->default_value("false"))
        ("gpu-devices", "GPU device IDs", cxxopts::value<std::vector<int>>())
        ("o,output", "Output file", cxxopts::value<std::string>())
        ("session", "Session file", cxxopts::value<std::string>())
        ("restore", "Restore from session", cxxopts::value<std::string>())
        ("analyze", "Password analysis mode", cxxopts::value<std::string>())
        ("benchmark", "Run benchmark", cxxopts::value<bool>())
        ("v,verbose", "Verbose output", cxxopts::value<bool>())
        ("h,help", "Print help");
    
    auto result = options.parse(argc, argv);
    
    if (result.count("help")) {
        config.options["help"] = "true";
        return config;
    }
    
    // Store parsed options
    if (result.count("attack-type"))
        config.options["attack_type"] = result["attack-type"].as<std::string>();
    if (result.count("hash-type"))
        config.options["hash_type"] = result["hash-type"].as<std::string>();
    if (result.count("target"))
        config.options["target_file"] = result["target"].as<std::string>();
    if (result.count("mask"))
        config.options["mask"] = result["mask"].as<std::string>();
    if (result.count("threads"))
        config.options["cpu_threads"] = std::to_string(result["threads"].as<int>());
    if (result.count("gpu"))
        config.options["use_gpu"] = result["gpu"].as<std::string>();
    
    return config;
}

bool Config::confirmEthicalUse() {
    std::cout << "\n⚠️  ETHICAL USE CONFIRMATION REQUIRED ⚠️\n";
    std::cout << "This tool is for educational and authorized testing only.\n";
    std::cout << "You must have explicit written permission to test any system.\n\n";
    std::cout << "Do you confirm you will use this tool ethically? (yes/no): ";
    
    std::string response;
    std::cin >> response;
    
    return response == "yes" || response == "YES";
}
