#!/bin/bash

EXECUTABLE="./build/src/server/server"

if [ -x "$EXECUTABLE" ]; then
    $EXECUTABLE 8080
else
    echo "Error: Executable file not found."
fi

