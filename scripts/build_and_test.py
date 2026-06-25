# scripts/build_and_test.py
#!/usr/bin/env python3
import subprocess
import sys
import os
from pathlib import Path

def build_project():
    """Build the project with CMake"""
    build_dir = Path("build")
    build_dir.mkdir(exist_ok=True)
    
    cmake_cmd = [
        "cmake",
        "..",
        "-DCMAKE_BUILD_TYPE=Release",
        "-DBUILD_TESTS=ON"
    ]
    
    subprocess.run(cmake_cmd, cwd=build_dir, check=True)
    subprocess.run(["cmake", "--build", ".", "-j4"], cwd=build_dir, check=True)
    
    print("✅ Build successful")

def run_tests():
    """Run the test suite"""
    result = subprocess.run(
        ["ctest", "--output-on-failure"],
        cwd="build",
        capture_output=True,
        text=True
    )
    
    if result.returncode == 0:
        print("✅ All tests passed")
    else:
        print("❌ Tests failed:")
        print(result.stderr)
        sys.exit(1)

def run_benchmark():
    """Run performance benchmarks"""
    print("Running benchmarks...")
    subprocess.run([
        "./build/PasswordCracker",
        "--benchmark",
        "--hash-type", "md5"
    ], check=True)

def test_ethical_controls():
    """Verify ethical controls are working"""
    print("Testing ethical controls...")
    
    # Test 1: Should require confirmation
    result = subprocess.run(
        ["./build/PasswordCracker", "--analyze", "-p", "test123"],
        input="no\n",
        capture_output=True,
        text=True
    )
    
    if "ETHICAL USE CONFIRMATION" not in result.stdout:
        print("❌ Ethical confirmation not shown")
        sys.exit(1)
    
    print("✅ Ethical controls working")

if __name__ == "__main__":
    print("Building Educational Password Security Analysis Framework")
    print("=" * 60)
    
    build_project()
    run_tests()
    test_ethical_controls()
    
    print("\n" + "=" * 60)
    print("✅ All checks passed! Ready for educational use.")
