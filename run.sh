#!/bin/bash

# This script cleans the project, recompiles it, and then runs the executable.

echo "Cleaning previous build and recompiling the project..."
make clean && make

# Check if the compilation was successful
if [ $? -eq 0 ]; then
    echo "Compilation successful. Running Ubuntu System Monitor..."
    ./sysmon
else
    echo "Compilation failed. Please check the build output for errors."
    exit 1
fi
