import subprocess
import os

# Set the size limit to 100MB (GitHub's per-file limit)
SIZE_LIMIT = 100 * 1024 * 1024

def get_tracked_files():
    # List all files in HEAD
    result = subprocess.run(
        ['git', 'ls-tree', '-r', '--long', 'HEAD'],
        stdout=subprocess.PIPE,
        text=True
    )
    files = []
    for line in result.stdout.splitlines():
        parts = line.split()
        if len(parts) >= 4:
            size = int(parts[3])
            filename = parts[-1]
            files.append((filename, size))
    return files

def main():
    files = get_tracked_files()
    large_files = [(f, s) for f, s in files if s > SIZE_LIMIT]
    if large_files:
        print("Files exceeding 100MB (will cause push to fail):")
        for f, s in large_files:
            print(f"{f}: {s/1024/1024:.2f} MB")
    else:
        print("No files exceed the 100MB limit.")

if __name__ == "__main__":
    main()
