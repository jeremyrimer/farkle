#!/bin/bash

# Ensure we're in the project root
cd "$(dirname "$0")"

# Remove old build folder if needed (optional, but helps with fresh starts)
# rm -rf build

# Configure with Ninja if not already done
if [ ! -f build/build.ninja ]; then
    echo "Configuring project with Ninja..."
    cmake -S . -B build -G Ninja
fi

# Build everything
echo "Building project..."
cmake --build build

# Check if build succeeded
if [ $? -ne 0 ]; then
    echo "Build failed!"
    exit 1
fi

# Run tests (executable is in build/)
if [ -f build/run_tests ]; then
    echo "Running tests..."
    ./build/run_tests
else
    echo "Test executable not found. Did you add files to tests/ folder?"
fi

# Check if build succeeded
if [ $? -ne 0 ]; then
    echo "Tests failed!"
    exit 1
fi

# Run the game
if [ -f build/Farkle ]; then
    echo "Launching Farkle..."
    ./build/Farkle
else
    echo "Game executable not found."
fi