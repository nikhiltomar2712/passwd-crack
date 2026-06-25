# CMakeLists.txt
cmake_minimum_required(VERSION 3.15)
project(PasswordCracker VERSION 1.0.0 LANGUAGES CXX CUDA)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Options
option(USE_CUDA "Enable CUDA GPU acceleration" OFF)
option(USE_OPENCL "Enable OpenCL support" OFF)
option(BUILD_TESTS "Build test suite" ON)
option(BUILD_BENCHMARKS "Build benchmarks" OFF)

# Find packages
find_package(OpenSSL REQUIRED)
find_package(Boost REQUIRED COMPONENTS filesystem program_options)
find_package(nlohmann_json REQUIRED)
find_package(spdlog REQUIRED)

if(USE_CUDA)
    find_package(CUDA REQUIRED)
    enable_language(CUDA)
    set(CMAKE_CUDA_STANDARD 14)
    add_definitions(-DUSE_CUDA)
endif()

if(USE_OPENCL)
    find_package(OpenCL REQUIRED)
    add_definitions(-DUSE_OPENCL)
endif()

# Source files
set(SOURCES
    src/main.cpp
    src/core/PasswordCracker.cpp
    src/utils/Config.cpp
    src/utils/Logger.cpp
    src/utils/SessionManager.cpp
)

set(HEADERS
    src/core/PasswordCracker.h
    src/hash/IHashAlgorithm.h
    src/hash/MD5Hash.h
    src/hash/SHA256Hash.h
    src/hash/HashFactory.h
    src/attack/IAttackMethod.h
    src/attack/BruteForceAttack.h
    src/attack/DictionaryAttack.h
    src/attack/AttackFactory.h
    src/gpu/GPUManager.h
    src/utils/Config.h
    src/utils/Logger.h
    src/utils/ProgressTracker.h
    src/utils/SessionManager.h
)

if(USE_CUDA)
    list(APPEND SOURCES src/gpu/cuda_kernels.cu)
endif()

# Create executable
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${OPENSSL_INCLUDE_DIR}
    ${Boost_INCLUDE_DIRS}
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    OpenSSL::Crypto
    Boost::filesystem
    Boost::program_options
    nlohmann_json::nlohmann_json
    spdlog::spdlog
)

if(USE_CUDA)
    target_link_libraries(${PROJECT_NAME} PRIVATE
        ${CUDA_LIBRARIES}
        ${CUDA_CUDART_LIBRARY}
    )
endif()

if(USE_OPENCL)
    target_link_libraries(${PROJECT_NAME} PRIVATE
        ${OpenCL_LIBRARY}
    )
endif()

# Tests
if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

# Install
install(TARGETS ${PROJECT_NAME} DESTINATION bin)
install(DIRECTORY wordlists/ DESTINATION share/password-cracker/wordlists)
install(DIRECTORY rules/ DESTINATION share/password-cracker/rules)
