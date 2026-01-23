#!/bin/bash
set -x

if [ -z "$1" ]; then
    echo "Usage: $0 <source_file>"
    exit 1
fi

TARGET_NAME=$(basename "$1" .${1##*.})
SOURCE_FILE="$1"

# If you're using relative folders:
FREEGLUT_BASEDIR="freeglut"
GLAD_DIR="glad"

set_compiler() {
    EXT="${SOURCE_FILE##*.}"
    case "$EXT" in
        c)   COMPILER="gcc" ;;
        cpp) COMPILER="g++" ;;
        *) echo "Unsupported file type: $EXT"; exit 1 ;;
    esac
}

compile() {
    echo "Compiling $SOURCE_FILE with $COMPILER..."

    # ---- sanity checks ----
    if [ ! -f "$GLAD_DIR/src/glad.c" ]; then
        echo "ERROR: Cannot find $GLAD_DIR/src/glad.c"
        echo "Expected: $GLAD_DIR/src/glad.c"
        exit 1
    fi
    if [ ! -f "$GLAD_DIR/include/glad/glad.h" ]; then
        echo "ERROR: Cannot find $GLAD_DIR/include/glad/glad.h"
        echo "Expected: $GLAD_DIR/include/glad/glad.h"
        exit 1
    fi
    if [ ! -d "$FREEGLUT_BASEDIR/include" ]; then
        echo "ERROR: Cannot find freeglut include dir at $FREEGLUT_BASEDIR/include"
        exit 1
    fi
    if [ ! -d "$FREEGLUT_BASEDIR/lib" ]; then
        echo "ERROR: Cannot find freeglut lib dir at $FREEGLUT_BASEDIR/lib"
        exit 1
    fi

    # 1) Compile glad.c (C file) -> glad.o
    gcc -std=c11 -c -o "glad.o" "$GLAD_DIR/src/glad.c" \
        -I"$GLAD_DIR/include"

    # 2) Compile your source -> main .o
    $COMPILER -std=c++17 -c -o "$TARGET_NAME.o" "$SOURCE_FILE" \
        -D FREEGLUT_STATIC \
        -I"$FREEGLUT_BASEDIR/include" \
        -I"$GLAD_DIR/include"

    # 3) Link everything into exe
    $COMPILER -std=c++17 -o "$TARGET_NAME.exe" "$TARGET_NAME.o" "glad.o" \
        -L"$FREEGLUT_BASEDIR/lib" \
        -lfreeglut_static -lopengl32 -lglu32 -lwinmm -lgdi32 \
        -Wl,--subsystem,console
}

cleanup() {
    echo "Cleaning up object files..."
    rm -f "$TARGET_NAME.o" "glad.o"
}

set_compiler
compile
cleanup
echo "Build completed: $TARGET_NAME.exe"
