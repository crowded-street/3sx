import sys
import re
import os
import subprocess
from pathlib import Path

def process_file(filepath):
    try:
        with open(filepath, 'r', encoding='utf-8') as f:
            content = f.read()
    except FileNotFoundError:
        print(f"Error: File not found at {filepath}")
        return False
    except UnicodeDecodeError:
        print(f"Skipping binary file: {filepath}")
        return False

    pattern = re.compile(
        r'^\s*#if defined\(TARGET_PS2\)\s*$(.*?)(?:^\s*#else\s*$(.*?))?^\s*#endif\s*?$',
        re.DOTALL | re.MULTILINE
    )

    def replace_block(match):
        code_if_block = match.group(1)
        code_else_block = match.group(2)
        if code_else_block is not None:
            return code_else_block
        else:
            return ""

    new_content = pattern.sub(replace_block, content)

    try:
        with open(filepath, 'w', encoding='utf-8') as f:
            f.write(new_content)
        return True
    except IOError as e:
        print(f"Error writing to file {filepath}: {e}")
        return False

def process_directory_recursive(directory, file_extensions=None):
    """
    Recursively process all files in directory.
    
    Args:
        directory: Root directory to start from
        file_extensions: List of extensions to filter (e.g., ['.c', '.cpp', '.h'])
                        If None, processes all files
    
    Returns:
        List of successfully processed file paths
    """
    processed_files = []
    
    for root, dirs, files in os.walk(directory):
        for filename in files:
            # Filter by extension if specified
            if file_extensions:
                if not any(filename.endswith(ext) for ext in file_extensions):
                    continue
            
            filepath = os.path.join(root, filename)
            print(f"Processing {filepath}...")
            if process_file(filepath):
                processed_files.append(filepath)
    
    return processed_files

def run_clang_format(files):
    """
    Run clang-format on all specified files.
    
    Args:
        files: List of file paths to format
    """
    if not files:
        print("No files to format.")
        return
    
    print(f"\nRunning clang-format on {len(files)} files...")
    
    # Check if clang-format is available
    try:
        subprocess.run(['clang-format', '--version'], 
                      capture_output=True, check=True)
    except (subprocess.CalledProcessError, FileNotFoundError):
        print("Error: clang-format is not installed or not in PATH")
        print("Please install clang-format to use this feature")
        return
    
    # Run clang-format on each file
    failed_files = []
    for filepath in files:
        try:
            result = subprocess.run(
                ['clang-format', '-i', filepath],
                capture_output=True,
                text=True,
                check=True
            )
            print(f"Formatted: {filepath}")
        except subprocess.CalledProcessError as e:
            print(f"Error formatting {filepath}: {e}")
            failed_files.append(filepath)
    
    if failed_files:
        print(f"\nFailed to format {len(failed_files)} files:")
        for f in failed_files:
            print(f"  - {f}")
    else:
        print(f"\nSuccessfully formatted all {len(files)} files.")

if __name__ == '__main__':
    # Default to current directory if no argument provided
    directory = sys.argv[1] if len(sys.argv) >= 2 else '.'
    
    # Optional: Specify file extensions to process (e.g., C/C++ files)
    # Uncomment and modify as needed:
    extensions = ['.c', '.cpp', '.h', '.hpp', '.cc', '.cxx']
    # extensions = None  # Process all files
    
    if not os.path.isdir(directory):
        print(f"Error: '{directory}' is not a valid directory")
        sys.exit(1)
    
    print(f"Starting recursive processing in: {directory}")
    processed_files = process_directory_recursive(directory, extensions)
    print(f"\nBatch processed {len(processed_files)} files.")
    
    # Run clang-format on all processed files
    run_clang_format(processed_files)
