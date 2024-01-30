#!/bin/bash

# Check if the number of arguments is correct
if [ "$#" -ne 1 ]; then
    echo "Usage: $0 <project_name>"
    exit 1
fi

# Set the project name
PROJECT_NAME=$1

# Set the path to the UF2 file
UF2_FILE_PATH="${PROJECT_NAME}.uf2"

# Set the Raspberry Pi Pico mount point
PICO_MOUNT_POINT="/media/$USER/RPI-RP2"

# Check if the UF2 file exists
if [ ! -f "$UF2_FILE_PATH" ]; then
    echo "Error: UF2 file not found at the specified path: $UF2_FILE_PATH"
    exit 1
fi

# Check if the Pico mount point exists
if [ ! -d "$PICO_MOUNT_POINT" ]; then
    echo "Error: Raspberry Pi Pico not found at the specified mount point: $PICO_MOUNT_POINT"
    exit 1
fi

# Copy the UF2 file to the Pico mount point
cp "$UF2_FILE_PATH" "$PICO_MOUNT_POINT"

# Check if the copy operation was successful
if [ $? -eq 0 ]; then
    echo "UF2 file '$PROJECT_NAME.uf2' successfully flashed to Raspberry Pi Pico!"
else
    echo "Error occurred while flashing the UF2 file."
fi
