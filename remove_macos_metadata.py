import os
import shutil

def remove_macos_folders(target_dir="."):
    deleted_count = 0
    
    # Walk through the directory tree
    for root, dirs, files in os.walk(target_dir, topdown=False):
        for name in dirs:
            # Check for standard ._ or .MACOS metadata folders
            if name == ".MACOSX" or name == ".MACOS":
                folder_path = os.path.join(root, name)
                try:
                    shutil.rmtree(folder_path)
                    print(f"Removed: {folder_path}")
                    deleted_count += 1
                except Exception as e:
                    print(f"Failed to delete {folder_path}: {e}")
    
    print(f"\nCleanup complete. Total folders removed: {deleted_count}")

if __name__ == "__main__":
    # Confirm before running
    confirm = input("This will permanently delete all .MACOS folders in the current directory and subdirectories. Proceed? (y/n): ")
    if confirm.lower() == 'y':
        remove_macos_folders()
    else:
        print("Operation cancelled.")