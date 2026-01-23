import os
import re
from pathlib import Path

def natural_sort_key(s):
    """
    Sorts strings containing numbers in the order a human would expect.
    Example: ['1.png', '2.png', '10.png'] instead of ['1.png', '10.png', '2.png']
    """
    return [int(text) if text.isdigit() else text.lower()
            for text in re.split(r'(\d+)', str(s))]

def rename_images(directory="."):
    # Target folder (default is current directory)
    path = Path(directory)
    
    # Filter for .png files and sort them naturally
    files = sorted(path.glob("*.png"), key=natural_sort_key)
    
    if not files:
        print("No .png files found.")
        return

    print(f"Found {len(files)} files. Starting rename...")

    for index, file_path in enumerate(files, start=1):
        new_name = f"{index}.png"
        new_path = file_path.with_name(new_name)

        # Avoid renaming if the name is already correct
        if file_path.name != new_name:
            try:
                print(f"Renaming: '{file_path.name}' -> '{new_name}'")
                file_path.rename(new_path)
            except FileExistsError:
                print(f"Error: Could not rename '{file_path.name}' because '{new_name}' already exists.")
        
    print("Done!")

if __name__ == "__main__":
    rename_images()