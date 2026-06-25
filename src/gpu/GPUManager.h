// src/gpu/GPUManager.h
#pragma once
#include <vector>
#include <memory>
#include <string>
#include "utils/Config.h"

#ifdef USE_CUDA
#include <cuda_runtime.h>
#endif

class GPUManager {
private:
    struct GPUDevice {
        int id;
        std::string name;
        size_t memory;
        bool available;
    };
    
    std::vector<GPUDevice> devices;
    bool initialized;
    
public:
    explicit GPUManager(const Config& config);
    ~GPUManager();
    
    void initialize();
    bool isAvailable() const { return !devices.empty(); }
    std::vector<GPUDevice> getAvailableDevices() const;
    
    // GPU hash computation
    std::vector<std::string> computeMD5Batch(const std::vector<std::string>& inputs);
    std::vector<std::string> computeSHA256Batch(const std::vector<std::string>& inputs);
    
private:
    void detectDevices();
    void allocateMemory();
    void createKernels();
};
