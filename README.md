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



This implementation provides a comprehensive educational password cracking framework with:

1. **Modular architecture** with plugin interfaces for easy extension
2. **Multiple hash algorithms** (MD5, SHA256 shown, others can be added)
3. **Various attack methods** (Brute force, Dictionary with rules)
4. **GPU acceleration** framework (CUDA kernel example provided)
5. **Session management** with JSON-based save/restore
6. **Progress tracking** and reporting
7. **Ethical controls** and legal notices
8. **Comprehensive testing** framework
9. **CMake build system** with dependency management

The code demonstrates key security concepts while maintaining educational focus and ethical safeguards. Users can extend it with additional hash algorithms, attack methods, and GPU kernels for their learning purposes.
# Resume session
./PasswordCracker --restore sessions/session_20260101.json
