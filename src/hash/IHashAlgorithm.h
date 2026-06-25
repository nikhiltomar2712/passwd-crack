// src/hash/IHashAlgorithm.h
#pragma once
#include <string>
#include <vector>
#include <cstdint>

class IHashAlgorithm {
public:
    virtual ~IHashAlgorithm() = default;
    
    virtual std::string getName() const = 0;
    virtual std::vector<uint8_t> hash(const std::string& input) = 0;
    virtual std::string hashToString(const std::string& input) = 0;
    virtual bool supportsGPU() const = 0;
    virtual size_t getHashSize() const = 0;
    virtual bool verify(const std::string& input, const std::string& expectedHash) = 0;
    
    static std::string bytesToHex(const std::vector<uint8_t>& bytes) {
        std::string result;
        result.reserve(bytes.size() * 2);
        for (uint8_t byte : bytes) {
            char hex[3];
            snprintf(hex, sizeof(hex), "%02x", byte);
            result += hex;
        }
        return result;
    }
};
