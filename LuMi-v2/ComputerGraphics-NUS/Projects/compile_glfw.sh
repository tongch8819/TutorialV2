#!/bin/bash
set -e
set -x

if [ -z "$1" ]; then
  echo "Usage: $0 <source_file.cpp>"
  exit 1
fi

TARGET_NAME=$(basename "$1" .${1##*.})
SOURCE_FILE="$1"

GLAD_DIR="glad"
GLFW_DIR="glfw"
GLFW_LIBDIR="$GLFW_DIR/lib-mingw-w64"

compile() {
  gcc -std=c11 -c -o glad.o "$GLAD_DIR/src/glad.c" -I"$GLAD_DIR/include"

  g++ -std=c++17 -c -o "$TARGET_NAME.o" "$SOURCE_FILE" \
      -I"$GLAD_DIR/include" \
      -I"$GLFW_DIR/include"

  # ---- Choose ONE of these two link modes ----

  # (A) DLL mode (recommended)
  g++ -std=c++17 -o "$TARGET_NAME.exe" "$TARGET_NAME.o" glad.o \
      "$GLFW_LIBDIR/libglfw3dll.a" \
      -lopengl32 -lgdi32 -luser32 -lkernel32 -lwinmm

  cp -f "$GLFW_LIBDIR/glfw3.dll" "./glfw3.dll"

  # (B) Static mode (alternative)
  # g++ -std=c++17 -o "$TARGET_NAME.exe" "$TARGET_NAME.o" glad.o \
  #     "$GLFW_LIBDIR/libglfw3.a" \
  #     -lopengl32 -lgdi32 -luser32 -lkernel32 -lwinmm -lshell32
}

cleanup() {
  rm -f "$TARGET_NAME.o" glad.o
}

compile
cleanup
echo "Build completed: $TARGET_NAME.exe"
