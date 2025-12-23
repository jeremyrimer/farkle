#!/bin/bash

# Ensure we're in the project root
cd "$(dirname "$0")"

CLEAN=false

# Parse arguments
for arg in "$@"; do
    case $arg in
        --clean|-c)
            CLEAN=true
            shift
            ;;
    esac
done

# Optional clean step
if [ "$CLEAN" = true ]; then
    echo "Cleaning build directory..."
    rm -rf build
fi

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

# Run tests (optional)
if [ -f build/run_tests ]; then
    echo "Running tests..."
    ./build/run_tests
    if [ $? -ne 0 ]; then
        echo "Tests failed!"
        exit 1
    fi
else
    echo "Test executable not found. Skipping tests."
fi

# Run the game
if [ -f build/Farkle ]; then
    echo "Launching Farkle..."
    ./build/Farkle
else
    echo "Game executable not found."
fi

echo "Game Over!"
