# README.md

# Educational Password Security Analysis Framework

## ⚠️ IMPORTANT LEGAL NOTICE
This software is for **EDUCATIONAL PURPOSES ONLY**. You must have explicit written authorization before testing any system. Unauthorized access to computer systems is illegal under the Computer Fraud and Abuse Act (CFAA) and similar laws worldwide.

## Features
- Multiple hash algorithm support (MD5, SHA1, SHA256, SHA512, bcrypt, NTLM)
- Various attack methods (Brute Force, Dictionary, Hybrid)
- GPU acceleration with CUDA
- Session management with resume capability
- Performance benchmarking
- Password strength analysis

## Quick Start

### Prerequisites
- CMake 3.15+
- C++17 compatible compiler
- OpenSSL development libraries
- CUDA Toolkit (optional, for GPU support)

### Build
```bash
mkdir build && cd build
cmake ..
make -j4

# Dictionary attack
./PasswordCracker -a dictionary -w wordlists/rockyou.txt -H md5 -t hashes.txt

# Brute force with mask
./PasswordCracker -a bruteforce -m "?l?l?l?l?d?d" -H sha256 -t hashes.txt

# Password analysis
./PasswordCracker --analyze -p "MyP@ssw0rd!" --detailed-report

# Resume session
./PasswordCracker --restore sessions/session_20260101.json
