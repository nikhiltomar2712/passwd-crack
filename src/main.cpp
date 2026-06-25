// src/main.cpp
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include "core/PasswordCracker.h"
#include "utils/Config.h"
#include "utils/Logger.h"
#include "hash/HashFactory.h"
#include "attack/AttackFactory.h"

void printBanner() {
    std::cout << R"(
╔══════════════════════════════════════════════════════════════╗
║     Educational Password Security Analysis Framework        ║
║                    Version 1.0.0                             ║
║  FOR AUTHORIZED SECURITY TESTING AND EDUCATION ONLY         ║
╚══════════════════════════════════════════════════════════════╝
    )" << std::endl;
}

int main(int argc, char* argv[]) {
    printBanner();
    
    try {
        auto config = Config::parse(argc, argv);
        Logger::init(config.getLogLevel());
        
        if (config.showHelp()) {
            Config::printHelp();
            return 0;
        }
        
        if (config.needsEthicalConfirmation()) {
            if (!Config::confirmEthicalUse()) {
                std::cerr << "Ethical confirmation required. Exiting." << std::endl;
                return 1;
            }
        }
        
        PasswordCracker cracker(config);
        cracker.run();
        
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}
