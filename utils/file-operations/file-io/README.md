# General File I/O

This directory contains general file input/output utilities.

## Contents

- **File Reading**: Read text and binary files
- **File Writing**: Write text and binary files
- **File Copying**: Copy files with progress
- **File Moving**: Move/rename files
- **File Deletion**: Safe file deletion
- **Directory Operations**: Create, list, delete directories
- **File Search**: Search files by name, pattern, content
- **File Watching**: Monitor file system changes
- **File Locking**: Handle file locks
- **Temporary Files**: Create and manage temp files
- **File Permissions**: Get/set file permissions
- **File Metadata**: Read file size, date, attributes
- **Path Operations**: Join, normalize, resolve paths
- **File Compression**: Compress/decompress files

## Features

- Cross-platform compatibility
- Async I/O support
- Buffered reading/writing
- Memory-mapped file support
- Atomic file operations

## Usage Examples

```javascript
// Read file
const content = readFile('document.txt', 'utf-8');

// Write file
writeFile('output.txt', content);

// Copy file
copyFile('source.txt', 'destination.txt');

// Watch directory
watchDirectory('./data', (event, filename) => {
    console.log(`File ${filename} was ${event}`);
});
```
