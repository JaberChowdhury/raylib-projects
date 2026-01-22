#!/bin/bash

# Build script for raylib particle collision project

set -e  # Exit on error

PROJECT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BUILD_DIR="$PROJECT_DIR/build"

echo "Building raylib particle collision project..."
echo "Project directory: $PROJECT_DIR"
echo "Build directory: $BUILD_DIR"

# Remove old build directory if it exists and is not locked
if [ -d "$BUILD_DIR" ]; then
    echo "Removing old build directory..."
    rm -rf "$BUILD_DIR" 2>/dev/null || {
        echo "Warning: Could not remove build directory. Some files may be locked."
        echo "Please close any IDEs or processes using the build directory and try again."
        exit 1
    }
fi

# Create fresh build directory
echo "Creating build directory..."
mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"

# Configure with CMake
echo "Configuring project with CMake..."
cmake .. || {
    echo "CMake configuration failed!"
    exit 1
}

# Build the project
echo "Building project..."
cmake --build . || {
    echo "Build failed!"
    exit 1
}

echo ""
echo "Build successful! Executable location: $BUILD_DIR/raylib-project"
echo ""
echo "To run the project, execute:"
echo "  cd $BUILD_DIR && ./raylib-project"
echo ""
echo "Or from the project root:"
echo "  ./build/raylib-project"
