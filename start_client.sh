#!/bin/bash

EXECUTABLE="./build/src/client/client"

if [ -x "$EXECUTABLE" ]; then
    $EXECUTABLE Name1 8080
else
    echo "Error: Executable file not found."
fi
