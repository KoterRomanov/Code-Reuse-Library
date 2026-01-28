# Compression and Decompression

This directory contains file and data compression utilities.

## Contents

- **GZIP Compression**: Compress/decompress using GZIP
- **ZIP Archive**: Create and extract ZIP files
- **TAR Archive**: Create and extract TAR files
- **BZIP2 Compression**: Compress/decompress using BZIP2
- **LZMA Compression**: High ratio compression
- **RAR Extraction**: Extract RAR archives
- **7-Zip Support**: Handle 7z archives
- **Stream Compression**: Compress data streams
- **In-Memory Compression**: Compress data in memory
- **Multi-File Archive**: Archive multiple files
- **Password-Protected Archives**: Handle encrypted archives
- **Split Archives**: Create and extract split archives
- **Compression Level**: Adjust compression ratio vs speed

## Features

- Multiple compression formats
- Progress tracking
- Memory-efficient streaming
- Archive browsing without extraction
- File filtering

## Usage Examples

```python
# Compress file
compress_file('document.txt', 'document.txt.gz', format='gzip')

# Decompress file
decompress_file('archive.gz', 'output/')

# Create ZIP archive
create_archive('backup.zip', ['file1.txt', 'file2.txt'], format='zip')

# Extract archive
extract_archive('backup.zip', 'output_dir/')

# In-memory compression
compressed_data = compress_data(data, format='gzip', level=9)
```
