/**
 * @file fileutils.h
 * @brief Universal file operation utility class for Qt projects
 * 
 * This class provides a simple, robust interface for file I/O operations
 * with automatic encoding detection and error handling.
 * 
 * Features:
 * - Automatic encoding detection (UTF-8, UTF-16, GBK, GB2312, etc.)
 * - Support for Chinese and other multibyte encodings
 * - Simple API: read/write files with one function call
 * - Comprehensive error handling
 * - Easy to integrate into any Qt project
 * 
 * @author Code Reuse Library
 * @date 2026-02-02
 * @version 1.0.0
 * @license MIT
 */

#ifndef FILEUTILS_H
#define FILEUTILS_H

#include <QString>
#include <QStringList>
#include <QByteArray>
#include <QTextCodec>

/**
 * @brief File encoding types
 */
enum class FileEncoding {
    AutoDetect,     ///< Automatically detect encoding
    UTF8,           ///< UTF-8 encoding
    UTF8BOM,        ///< UTF-8 with BOM
    UTF16LE,        ///< UTF-16 Little Endian
    UTF16BE,        ///< UTF-16 Big Endian
    GBK,            ///< GBK (Chinese)
    GB2312,         ///< GB2312 (Simplified Chinese)
    GB18030,        ///< GB18030 (Chinese)
    Big5,           ///< Big5 (Traditional Chinese)
    ShiftJIS,       ///< Shift-JIS (Japanese)
    ISO_8859_1,     ///< ISO-8859-1 (Latin-1)
    System          ///< System default encoding
};

/**
 * @brief File operation result
 */
struct FileResult {
    bool success;           ///< Operation success flag
    QString errorMessage;   ///< Error message if failed
    QString content;        ///< File content (for read operations)
    FileEncoding encoding;  ///< Detected or used encoding
    
    FileResult() : success(false), encoding(FileEncoding::UTF8) {}
    
    /**
     * @brief Check if operation succeeded
     */
    operator bool() const { return success; }
    
    /**
     * @brief Check if operation failed
     */
    bool hasError() const { return !success; }
};

/**
 * @brief Universal file operation utility class
 * 
 * This class provides static methods for common file operations.
 * All methods are thread-safe and handle errors gracefully.
 * 
 * Example usage:
 * @code
 * // Read file (auto-detect encoding)
 * FileResult result = FileUtils::readFile("config.txt");
 * if (result) {
 *     qDebug() << result.content;
 * } else {
 *     qDebug() << "Error:" << result.errorMessage;
 * }
 * 
 * // Write file (UTF-8)
 * bool ok = FileUtils::writeFile("output.txt", content);
 * @endcode
 */
class FileUtils {
public:
    // ============= Reading Operations =============
    
    /**
     * @brief Read entire file content with automatic encoding detection
     * @param filePath Path to the file
     * @return FileResult containing content and status
     * 
     * This method automatically detects the file encoding and converts
     * the content to QString. It supports UTF-8, UTF-16, GBK, GB2312, etc.
     */
    static FileResult readFile(const QString &filePath);
    
    /**
     * @brief Read file content with specified encoding
     * @param filePath Path to the file
     * @param encoding Encoding to use
     * @return FileResult containing content and status
     */
    static FileResult readFile(const QString &filePath, FileEncoding encoding);
    
    /**
     * @brief Read file as raw bytes without encoding conversion
     * @param filePath Path to the file
     * @param data Output parameter for file content
     * @param errorMessage Output parameter for error message
     * @return true on success, false on failure
     */
    static bool readFileBytes(const QString &filePath, QByteArray &data, QString *errorMessage = nullptr);
    
    /**
     * @brief Read file line by line with automatic encoding detection
     * @param filePath Path to the file
     * @param lines Output parameter for file lines
     * @param errorMessage Output parameter for error message
     * @return true on success, false on failure
     */
    static bool readFileLines(const QString &filePath, QStringList &lines, QString *errorMessage = nullptr);
    
    /**
     * @brief Read file line by line with specified encoding
     * @param filePath Path to the file
     * @param encoding Encoding to use
     * @param lines Output parameter for file lines
     * @param errorMessage Output parameter for error message
     * @return true on success, false on failure
     */
    static bool readFileLines(const QString &filePath, FileEncoding encoding, 
                             QStringList &lines, QString *errorMessage = nullptr);
    
    // ============= Writing Operations =============
    
    /**
     * @brief Write content to file with UTF-8 encoding
     * @param filePath Path to the file
     * @param content Content to write
     * @param errorMessage Output parameter for error message
     * @return true on success, false on failure
     */
    static bool writeFile(const QString &filePath, const QString &content, 
                         QString *errorMessage = nullptr);
    
    /**
     * @brief Write content to file with specified encoding
     * @param filePath Path to the file
     * @param content Content to write
     * @param encoding Encoding to use
     * @param errorMessage Output parameter for error message
     * @return true on success, false on failure
     */
    static bool writeFile(const QString &filePath, const QString &content, 
                         FileEncoding encoding, QString *errorMessage = nullptr);
    
    /**
     * @brief Write raw bytes to file
     * @param filePath Path to the file
     * @param data Data to write
     * @param errorMessage Output parameter for error message
     * @return true on success, false on failure
     */
    static bool writeFileBytes(const QString &filePath, const QByteArray &data, 
                              QString *errorMessage = nullptr);
    
    /**
     * @brief Append content to file with UTF-8 encoding
     * @param filePath Path to the file
     * @param content Content to append
     * @param errorMessage Output parameter for error message
     * @return true on success, false on failure
     */
    static bool appendFile(const QString &filePath, const QString &content, 
                          QString *errorMessage = nullptr);
    
    /**
     * @brief Write lines to file with UTF-8 encoding
     * @param filePath Path to the file
     * @param lines Lines to write
     * @param errorMessage Output parameter for error message
     * @return true on success, false on failure
     */
    static bool writeFileLines(const QString &filePath, const QStringList &lines, 
                              QString *errorMessage = nullptr);
    
    // ============= File Information =============
    
    /**
     * @brief Check if file exists
     * @param filePath Path to the file
     * @return true if file exists, false otherwise
     */
    static bool fileExists(const QString &filePath);
    
    /**
     * @brief Get file size in bytes
     * @param filePath Path to the file
     * @return File size, or -1 on error
     */
    static qint64 fileSize(const QString &filePath);
    
    /**
     * @brief Detect file encoding
     * @param filePath Path to the file
     * @return Detected encoding
     */
    static FileEncoding detectEncoding(const QString &filePath);
    
    /**
     * @brief Detect encoding from byte array
     * @param data Data to analyze
     * @return Detected encoding
     */
    static FileEncoding detectEncoding(const QByteArray &data);
    
    /**
     * @brief Get encoding name as string
     * @param encoding Encoding type
     * @return Encoding name
     */
    static QString encodingName(FileEncoding encoding);
    
    // ============= Utility Operations =============
    
    /**
     * @brief Copy file
     * @param sourcePath Source file path
     * @param destPath Destination file path
     * @param overwrite Whether to overwrite existing file
     * @param errorMessage Output parameter for error message
     * @return true on success, false on failure
     */
    static bool copyFile(const QString &sourcePath, const QString &destPath, 
                        bool overwrite = true, QString *errorMessage = nullptr);
    
    /**
     * @brief Move/rename file
     * @param sourcePath Source file path
     * @param destPath Destination file path
     * @param overwrite Whether to overwrite existing file
     * @param errorMessage Output parameter for error message
     * @return true on success, false on failure
     */
    static bool moveFile(const QString &sourcePath, const QString &destPath, 
                        bool overwrite = true, QString *errorMessage = nullptr);
    
    /**
     * @brief Delete file
     * @param filePath Path to the file
     * @param errorMessage Output parameter for error message
     * @return true on success, false on failure
     */
    static bool deleteFile(const QString &filePath, QString *errorMessage = nullptr);
    
    /**
     * @brief Create directory (including parent directories)
     * @param dirPath Path to the directory
     * @param errorMessage Output parameter for error message
     * @return true on success, false on failure
     */
    static bool createDirectory(const QString &dirPath, QString *errorMessage = nullptr);
    
    /**
     * @brief Get file extension
     * @param filePath Path to the file
     * @return File extension (without dot), or empty string
     */
    static QString fileExtension(const QString &filePath);
    
    /**
     * @brief Get file name without extension
     * @param filePath Path to the file
     * @return File name without extension
     */
    static QString fileBaseName(const QString &filePath);
    
    /**
     * @brief Get directory path from file path
     * @param filePath Path to the file
     * @return Directory path
     */
    static QString directoryPath(const QString &filePath);
    
private:
    /**
     * @brief Convert encoding enum to codec name
     * @param encoding Encoding type
     * @return Codec name for QTextCodec
     */
    static const char* encodingToCodecName(FileEncoding encoding);
    
    /**
     * @brief Decode byte array to QString with specified encoding
     * @param data Byte array to decode
     * @param encoding Encoding to use
     * @return Decoded string
     */
    static QString decodeString(const QByteArray &data, FileEncoding encoding);
    
    /**
     * @brief Encode QString to byte array with specified encoding
     * @param str String to encode
     * @param encoding Encoding to use
     * @return Encoded byte array
     */
    static QByteArray encodeString(const QString &str, FileEncoding encoding);
    
    /**
     * @brief Check for UTF-8 BOM
     * @param data Data to check
     * @return true if UTF-8 BOM found
     */
    static bool hasUtf8Bom(const QByteArray &data);
    
    /**
     * @brief Check for UTF-16 BOM
     * @param data Data to check
     * @param littleEndian Output parameter for endianness
     * @return true if UTF-16 BOM found
     */
    static bool hasUtf16Bom(const QByteArray &data, bool &littleEndian);
    
    /**
     * @brief Remove BOM from data if present
     * @param data Data to process
     * @return Data without BOM
     */
    static QByteArray removeBom(const QByteArray &data);
};

#endif // FILEUTILS_H
