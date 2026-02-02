# FileUtils - Universal File I/O Utility for Qt

A comprehensive, production-ready file operation utility class for Qt projects with automatic encoding detection and robust error handling.

## Features

- ✨ **Automatic Encoding Detection** - Detects UTF-8, UTF-16, GBK, GB2312, and other encodings
- 🌏 **Chinese Character Support** - Full support for Chinese and multibyte encodings (GBK, GB2312, GB18030, Big5)
- 🚀 **Simple API** - Read/write files with a single function call
- 🛡️ **Robust Error Handling** - Comprehensive error reporting and validation
- 📦 **Zero Configuration** - Works out of the box with any Qt project
- 🔧 **Thread-Safe** - All static methods are thread-safe
- 📝 **Well-Documented** - Extensive inline documentation and examples

## Table of Contents

- [Quick Start](#quick-start)
- [Installation](#installation)
- [API Reference](#api-reference)
- [Usage Examples](#usage-examples)
- [Encoding Support](#encoding-support)
- [Error Handling](#error-handling)
- [Best Practices](#best-practices)
- [FAQ](#faq)

## Quick Start

### Basic File Reading

```cpp
#include "fileutils.h"

// Read file with automatic encoding detection
FileResult result = FileUtils::readFile("config.txt");
if (result) {
    qDebug() << "Content:" << result.content;
    qDebug() << "Encoding:" << FileUtils::encodingName(result.encoding);
} else {
    qDebug() << "Error:" << result.errorMessage;
}
```

### Basic File Writing

```cpp
#include "fileutils.h"

// Write file with UTF-8 encoding (default)
QString content = "Hello, 世界!";
bool success = FileUtils::writeFile("output.txt", content);

if (!success) {
    qDebug() << "Write failed!";
}
```

## Installation

### Method 1: Include in Your Project (Recommended)

1. Copy `fileutils.h` and `fileutils.cpp` to your project
2. Add to your `.pro` file:
   ```qmake
   HEADERS += fileutils.h
   SOURCES += fileutils.cpp
   ```
3. Include in your code:
   ```cpp
   #include "fileutils.h"
   ```

### Method 2: Use as Submodule

1. Add to your `.pro` file:
   ```qmake
   include(path/to/file-utils/fileutils.pri)
   ```

### Method 3: CMake Integration

```cmake
# Add to your CMakeLists.txt
add_library(fileutils
    path/to/fileutils.h
    path/to/fileutils.cpp
)

target_link_libraries(your_target fileutils Qt5::Core)
```

## API Reference

### Reading Operations

#### `FileResult readFile(const QString &filePath)`
Read entire file content with automatic encoding detection.

**Returns:** `FileResult` containing content, status, error message, and detected encoding.

**Example:**
```cpp
FileResult result = FileUtils::readFile("data.txt");
if (result) {
    qDebug() << result.content;
}
```

#### `FileResult readFile(const QString &filePath, FileEncoding encoding)`
Read file content with specified encoding.

**Parameters:**
- `filePath` - Path to the file
- `encoding` - Encoding to use (see [Encoding Support](#encoding-support))

**Example:**
```cpp
FileResult result = FileUtils::readFile("chinese.txt", FileEncoding::GBK);
```

#### `bool readFileBytes(const QString &filePath, QByteArray &data, QString *errorMessage = nullptr)`
Read file as raw bytes without encoding conversion.

**Example:**
```cpp
QByteArray data;
QString error;
if (FileUtils::readFileBytes("image.png", data, &error)) {
    qDebug() << "Size:" << data.size();
}
```

#### `bool readFileLines(const QString &filePath, QStringList &lines, QString *errorMessage = nullptr)`
Read file line by line with automatic encoding detection.

**Example:**
```cpp
QStringList lines;
if (FileUtils::readFileLines("config.txt", lines)) {
    for (const QString &line : lines) {
        qDebug() << line;
    }
}
```

### Writing Operations

#### `bool writeFile(const QString &filePath, const QString &content, QString *errorMessage = nullptr)`
Write content to file with UTF-8 encoding.

**Example:**
```cpp
QString error;
if (!FileUtils::writeFile("output.txt", content, &error)) {
    qDebug() << "Error:" << error;
}
```

#### `bool writeFile(const QString &filePath, const QString &content, FileEncoding encoding, QString *errorMessage = nullptr)`
Write content to file with specified encoding.

**Example:**
```cpp
FileUtils::writeFile("chinese.txt", "你好", FileEncoding::GBK);
```

#### `bool writeFileBytes(const QString &filePath, const QByteArray &data, QString *errorMessage = nullptr)`
Write raw bytes to file.

**Example:**
```cpp
QByteArray data = ...;
FileUtils::writeFileBytes("binary.dat", data);
```

#### `bool appendFile(const QString &filePath, const QString &content, QString *errorMessage = nullptr)`
Append content to file with UTF-8 encoding.

**Example:**
```cpp
FileUtils::appendFile("log.txt", "New log entry\n");
```

#### `bool writeFileLines(const QString &filePath, const QStringList &lines, QString *errorMessage = nullptr)`
Write lines to file with UTF-8 encoding.

**Example:**
```cpp
QStringList lines = {"Line 1", "Line 2", "Line 3"};
FileUtils::writeFileLines("output.txt", lines);
```

### File Information

#### `bool fileExists(const QString &filePath)`
Check if file exists.

#### `qint64 fileSize(const QString &filePath)`
Get file size in bytes. Returns -1 on error.

#### `FileEncoding detectEncoding(const QString &filePath)`
Detect file encoding.

#### `QString encodingName(FileEncoding encoding)`
Get encoding name as string.

### Utility Operations

#### `bool copyFile(const QString &sourcePath, const QString &destPath, bool overwrite = true, QString *errorMessage = nullptr)`
Copy file from source to destination.

#### `bool moveFile(const QString &sourcePath, const QString &destPath, bool overwrite = true, QString *errorMessage = nullptr)`
Move or rename file.

#### `bool deleteFile(const QString &filePath, QString *errorMessage = nullptr)`
Delete file.

#### `bool createDirectory(const QString &dirPath, QString *errorMessage = nullptr)`
Create directory including parent directories.

#### `QString fileExtension(const QString &filePath)`
Get file extension (without dot).

#### `QString fileBaseName(const QString &filePath)`
Get file name without extension.

#### `QString directoryPath(const QString &filePath)`
Get directory path from file path.

## Usage Examples

### Example 1: Configuration File Handler

```cpp
class ConfigHandler {
public:
    bool loadConfig(const QString &filePath) {
        FileResult result = FileUtils::readFile(filePath);
        if (!result) {
            qWarning() << "Failed to load config:" << result.errorMessage;
            return false;
        }
        
        // Parse configuration
        parseConfig(result.content);
        return true;
    }
    
    bool saveConfig(const QString &filePath, const QString &config) {
        QString error;
        if (!FileUtils::writeFile(filePath, config, &error)) {
            qWarning() << "Failed to save config:" << error;
            return false;
        }
        return true;
    }
};
```

### Example 2: Log File Writer

```cpp
class Logger {
public:
    void log(const QString &message) {
        QString timestamp = QDateTime::currentDateTime().toString(Qt::ISODate);
        QString logEntry = QString("[%1] %2\n").arg(timestamp, message);
        
        QString error;
        if (!FileUtils::appendFile(m_logFile, logEntry, &error)) {
            qWarning() << "Failed to write log:" << error;
        }
    }
    
private:
    QString m_logFile = "application.log";
};
```

### Example 3: Chinese Text Processor

```cpp
void processChineseFile(const QString &inputFile, const QString &outputFile) {
    // Read file with GBK encoding
    FileResult result = FileUtils::readFile(inputFile, FileEncoding::GBK);
    if (!result) {
        qDebug() << "Read error:" << result.errorMessage;
        return;
    }
    
    // Process content
    QString processed = result.content.toUpper();
    
    // Write with UTF-8 encoding
    if (!FileUtils::writeFile(outputFile, processed, FileEncoding::UTF8)) {
        qDebug() << "Write error";
    }
}
```

### Example 4: Batch File Converter

```cpp
void convertEncodingBatch(const QStringList &files, 
                          FileEncoding from, 
                          FileEncoding to) {
    for (const QString &file : files) {
        // Read with source encoding
        FileResult result = FileUtils::readFile(file, from);
        if (!result) {
            qWarning() << "Skip" << file << ":" << result.errorMessage;
            continue;
        }
        
        // Write with target encoding
        QString outputFile = file + ".converted";
        if (FileUtils::writeFile(outputFile, result.content, to)) {
            qDebug() << "Converted:" << file;
        }
    }
}
```

### Example 5: Safe File Operations

```cpp
bool safeWriteFile(const QString &filePath, const QString &content) {
    // Create backup
    QString backupPath = filePath + ".bak";
    if (FileUtils::fileExists(filePath)) {
        QString error;
        if (!FileUtils::copyFile(filePath, backupPath, true, &error)) {
            qWarning() << "Backup failed:" << error;
            return false;
        }
    }
    
    // Write new content
    QString error;
    if (!FileUtils::writeFile(filePath, content, &error)) {
        qWarning() << "Write failed:" << error;
        
        // Restore backup
        if (FileUtils::fileExists(backupPath)) {
            FileUtils::moveFile(backupPath, filePath, true);
        }
        return false;
    }
    
    // Remove backup
    FileUtils::deleteFile(backupPath);
    return true;
}
```

## Encoding Support

### Supported Encodings

| Encoding | Description | Use Case |
|----------|-------------|----------|
| `AutoDetect` | Automatic detection | Default, recommended |
| `UTF8` | UTF-8 without BOM | Modern text files |
| `UTF8BOM` | UTF-8 with BOM | Windows applications |
| `UTF16LE` | UTF-16 Little Endian | Windows text files |
| `UTF16BE` | UTF-16 Big Endian | Some Unix systems |
| `GBK` | GBK encoding | Simplified Chinese |
| `GB2312` | GB2312 encoding | Legacy Chinese |
| `GB18030` | GB18030 encoding | Modern Chinese standard |
| `Big5` | Big5 encoding | Traditional Chinese |
| `ShiftJIS` | Shift-JIS encoding | Japanese |
| `ISO_8859_1` | Latin-1 encoding | Western European |
| `System` | System default | Platform-specific |

### Auto-Detection Algorithm

The automatic encoding detection follows this order:

1. **UTF-8 BOM** - Check for UTF-8 byte order mark (EF BB BF)
2. **UTF-16 BOM** - Check for UTF-16 byte order marks (FF FE or FE FF)
3. **UTF-8 Validation** - Validate UTF-8 byte sequences
4. **GBK Detection** - Check for GBK/GB2312 characteristics
5. **Fallback** - Default to system encoding

### Encoding Examples

```cpp
// Auto-detect encoding (recommended)
FileResult result = FileUtils::readFile("file.txt");
qDebug() << "Detected:" << FileUtils::encodingName(result.encoding);

// Explicitly specify encoding
FileResult result = FileUtils::readFile("file.txt", FileEncoding::GBK);

// Write with specific encoding
FileUtils::writeFile("output.txt", content, FileEncoding::UTF8BOM);

// Detect encoding without reading content
FileEncoding enc = FileUtils::detectEncoding("file.txt");
qDebug() << FileUtils::encodingName(enc);
```

## Error Handling

### FileResult Structure

```cpp
struct FileResult {
    bool success;           // Operation success flag
    QString errorMessage;   // Error message if failed
    QString content;        // File content (for read operations)
    FileEncoding encoding;  // Detected or used encoding
    
    operator bool() const;  // Implicit conversion to bool
    bool hasError() const;  // Check if operation failed
};
```

### Error Handling Patterns

#### Pattern 1: Simple Check
```cpp
FileResult result = FileUtils::readFile("file.txt");
if (result) {
    // Success
    processContent(result.content);
} else {
    // Error
    qDebug() << "Error:" << result.errorMessage;
}
```

#### Pattern 2: With Error Message
```cpp
QString error;
if (!FileUtils::writeFile("file.txt", content, &error)) {
    qCritical() << "Write failed:" << error;
    return false;
}
```

#### Pattern 3: Try-Catch Style
```cpp
FileResult result = FileUtils::readFile("file.txt");
if (result.hasError()) {
    handleError(result.errorMessage);
    return;
}

// Process result.content
```

### Common Errors

| Error | Cause | Solution |
|-------|-------|----------|
| "File does not exist" | File not found | Check file path |
| "Cannot open file for reading" | Permission denied | Check file permissions |
| "Cannot open file for writing" | Directory doesn't exist or no permission | Create directory or check permissions |
| "Failed to write all data" | Disk full or I/O error | Check disk space |
| "Failed to create directory" | Permission denied | Check parent directory permissions |

## Best Practices

### 1. Always Check Return Values

```cpp
// ❌ Bad
FileUtils::writeFile("file.txt", content);

// ✅ Good
if (!FileUtils::writeFile("file.txt", content)) {
    qWarning() << "Write failed";
}
```

### 2. Use Auto-Detect for Reading

```cpp
// ✅ Recommended - Let FileUtils detect encoding
FileResult result = FileUtils::readFile("file.txt");

// Only specify encoding if you're certain
FileResult result = FileUtils::readFile("file.txt", FileEncoding::GBK);
```

### 3. Handle Errors Properly

```cpp
QString error;
if (!FileUtils::writeFile(path, content, &error)) {
    qCritical() << "Failed to write file:" << error;
    // Show error to user or log it
    return false;
}
```

### 4. Use Appropriate Methods

```cpp
// For text files
FileResult result = FileUtils::readFile("text.txt");

// For binary files
QByteArray data;
FileUtils::readFileBytes("image.png", data);

// For line-based files
QStringList lines;
FileUtils::readFileLines("config.txt", lines);
```

### 5. Create Directories Automatically

```cpp
// FileUtils automatically creates parent directories
FileUtils::writeFile("path/to/new/file.txt", content);
// No need to manually create "path/to/new/" directory
```

### 6. Be Explicit with Encodings When Writing

```cpp
// For Chinese content, be explicit
FileUtils::writeFile("chinese.txt", chineseText, FileEncoding::GBK);

// For general use, UTF-8 is recommended
FileUtils::writeFile("file.txt", content, FileEncoding::UTF8);
```

## FAQ

### Q: Does FileUtils support large files?

**A:** FileUtils reads entire files into memory. For files larger than available RAM, consider streaming approaches or use Qt's `QFile` directly.

### Q: Is FileUtils thread-safe?

**A:** Yes, all FileUtils methods are static and thread-safe. However, be careful with concurrent writes to the same file.

### Q: How accurate is the encoding detection?

**A:** Very accurate for UTF-8, UTF-16 (with BOM), and GBK/GB2312. For ambiguous cases, specify encoding explicitly.

### Q: Can I use FileUtils for binary files?

**A:** Yes, use `readFileBytes()` and `writeFileBytes()` for binary data.

### Q: Does FileUtils work on all platforms?

**A:** Yes, FileUtils works on Windows, macOS, and Linux. It uses Qt's cross-platform APIs.

### Q: How do I handle very large files?

**A:** For streaming large files, use Qt's `QFile` and `QTextStream` directly:
```cpp
QFile file("large.txt");
if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        processLine(line);
    }
}
```

### Q: Can I customize encoding detection?

**A:** The `detectEncoding()` method is public. You can call it separately and handle encodings as needed.

### Q: What about file locking?

**A:** FileUtils doesn't implement file locking. For concurrent access, implement your own locking mechanism using `QFile::open()` with appropriate flags.

## Performance Considerations

- **Small Files (<1MB):** Excellent performance, no concerns
- **Medium Files (1-10MB):** Good performance, memory usage is acceptable
- **Large Files (>10MB):** Consider memory constraints, may want to use streaming
- **Encoding Detection:** Fast for UTF-8/UTF-16 with BOM, slightly slower for auto-detection

## Testing

FileUtils comes with comprehensive unit tests. To run tests:

```bash
cd tests
qmake tests.pro
make
./fileutils_tests
```

## Version History

- **v1.0.0** (2026-02-02)
  - Initial release
  - Automatic encoding detection
  - Support for UTF-8, UTF-16, GBK, GB2312, etc.
  - Comprehensive error handling
  - File utility operations

## License

MIT License - Feel free to use in any project.

## Contributing

Contributions are welcome! Please ensure:
- Code follows existing style
- All tests pass
- New features include tests
- Documentation is updated

## Support

For issues, questions, or suggestions:
- Check the [FAQ](#faq)
- Review [Usage Examples](#usage-examples)
- Open an issue in the repository

---

**FileUtils** - Making file I/O in Qt simple and reliable! 🚀
