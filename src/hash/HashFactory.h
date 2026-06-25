// src/hash/HashFactory.h
#pragma once
#include "IHashAlgorithm.h"
#include <memory>
#include <string>
#include <stdexcept>

class HashFactory {
public:
    static std::unique_ptr<IHashAlgorithm> createHashAlgorithm(const std::string& type) {
        if (type == "md5") {
            return std::make_unique<MD5Hash>();
        } else if (type == "sha1") {
            return std::make_unique<SHA1Hash>();
        } else if (type == "sha256") {
            return std::make_unique<SHA256Hash>();
        } else if (type == "sha512") {
            return std::make_unique<SHA512Hash>();
        } else if (type == "ntlm") {
            return std::make_unique<NTLMHash>();
        } else {
            throw std::runtime_error("Unsupported hash algorithm: " + type);
        }
    }
};
