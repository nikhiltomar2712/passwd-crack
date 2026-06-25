// src/hash/SHA256Hash.h
#pragma once
#include "IHashAlgorithm.h"
#include <openssl/sha.h>

class SHA256Hash : public IHashAlgorithm {
public:
    std::string getName() const override { return "SHA256"; }
    
    std::vector<uint8_t> hash(const std::string& input) override {
        std::vector<uint8_t> result(SHA256_DIGEST_LENGTH);
        SHA256(reinterpret_cast<const unsigned char*>(input.c_str()), 
               input.length(), result.data());
        return result;
    }
    
    std::string hashToString(const std::string& input) override {
        auto hashBytes = hash(input);
        return bytesToHex(hashBytes);
    }
    
    bool supportsGPU() const override { return true; }
    size_t getHashSize() const override { return SHA256_DIGEST_LENGTH; }
    
    bool verify(const std::string& input, const std::string& expectedHash) override {
        return hashToString(input) == expectedHash;
    }
};
