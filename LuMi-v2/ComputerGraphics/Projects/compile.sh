#!/bin/bash
set -x
# Check if target file is provided
if [ -z "$1" ]; then
    echo "Usage: $0 <source_file>"
    exit 1
fi

TARGET_NAME=$(basename "$1" .${1##*.})  # Extract filename without extension
SOURCE_FILE="$1"
FREEGLUT_BASEDIR="E:\Projects\TutorialV1\LuMi-v2\ComputerGraphics\Projects\freeglut"

# Helper function to detect file type and set the compiler
set_compiler() {
    EXT="${SOURCE_FILE##*.}"
    case "$EXT" in
        c)
            COMPILER="gcc"
            ;;
        cpp)
            COMPILER="g++"
            ;;
        *)
            echo "Unsupported file type: $EXT"
            exit 1
            ;;
    esac
}

# Compilation function
compile() {
    echo "Compiling $SOURCE_FILE with $COMPILER..."
    $COMPILER -c -o "$TARGET_NAME.o" "$SOURCE_FILE" -D FREEGLUT_STATIC -I"$FREEGLUT_BASEDIR\include"
    $COMPILER -o "$TARGET_NAME.exe" "$TARGET_NAME.o" -L"$FREEGLUT_BASEDIR\lib" -lfreeglut_static -lopengl32 -lglu32 -lwinmm -lgdi32 -Wl,--subsystem,windows
}

# Cleanup function to remove object files
cleanup() {
    echo "Cleaning up object files..."
    rm -f "$TARGET_NAME.o"
}

# Run the script
set_compiler
compile
cleanup

echo "Build completed: $TARGET_NAME.exe"
