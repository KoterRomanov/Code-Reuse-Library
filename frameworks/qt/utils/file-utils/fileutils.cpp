/**
 * @file fileutils.cpp
 * @brief Implementation of FileUtils class
 */

#include "fileutils.h"
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QTextStream>
#include <QDebug>

// ============= Reading Operations =============

FileResult FileUtils::readFile(const QString &filePath) {
    return readFile(filePath, FileEncoding::AutoDetect);
}

FileResult FileUtils::readFile(const QString &filePath, FileEncoding encoding) {
    FileResult result;
    
    // Check if file exists
    if (!fileExists(filePath)) {
        result.success = false;
        result.errorMessage = QString("File does not exist: %1").arg(filePath);
        return result;
    }
    
    // Read file as bytes
    QByteArray data;
    if (!readFileBytes(filePath, data, &result.errorMessage)) {
        result.success = false;
        return result;
    }
    
    // Auto-detect encoding if needed
    if (encoding == FileEncoding::AutoDetect) {
        encoding = detectEncoding(data);
    }
    
    // Decode content
    result.content = decodeString(data, encoding);
    result.encoding = encoding;
    result.success = true;
    
    return result;
}

bool FileUtils::readFileBytes(const QString &filePath, QByteArray &data, QString *errorMessage) {
    QFile file(filePath);
    
    if (!file.open(QIODevice::ReadOnly)) {
        if (errorMessage) {
            *errorMessage = QString("Cannot open file for reading: %1 - %2")
                           .arg(filePath, file.errorString());
        }
        return false;
    }
    
    data = file.readAll();
    file.close();
    
    return true;
}

bool FileUtils::readFileLines(const QString &filePath, QStringList &lines, QString *errorMessage) {
    return readFileLines(filePath, FileEncoding::AutoDetect, lines, errorMessage);
}

bool FileUtils::readFileLines(const QString &filePath, FileEncoding encoding, 
                             QStringList &lines, QString *errorMessage) {
    FileResult result = readFile(filePath, encoding);
    
    if (!result.success) {
        if (errorMessage) {
            *errorMessage = result.errorMessage;
        }
        return false;
    }
    
    lines = result.content.split('\n');
    
    // Remove '\r' from Windows line endings
    for (QString &line : lines) {
        if (line.endsWith('\r')) {
            line.chop(1);
        }
    }
    
    return true;
}

// ============= Writing Operations =============

bool FileUtils::writeFile(const QString &filePath, const QString &content, QString *errorMessage) {
    return writeFile(filePath, content, FileEncoding::UTF8, errorMessage);
}

bool FileUtils::writeFile(const QString &filePath, const QString &content, 
                         FileEncoding encoding, QString *errorMessage) {
    // Ensure directory exists
    QFileInfo fileInfo(filePath);
    QString dirPath = fileInfo.absolutePath();
    if (!QDir(dirPath).exists()) {
        if (!createDirectory(dirPath, errorMessage)) {
            return false;
        }
    }
    
    // Encode content
    QByteArray data = encodeString(content, encoding);
    
    // Write to file
    return writeFileBytes(filePath, data, errorMessage);
}

bool FileUtils::writeFileBytes(const QString &filePath, const QByteArray &data, QString *errorMessage) {
    QFile file(filePath);
    
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        if (errorMessage) {
            *errorMessage = QString("Cannot open file for writing: %1 - %2")
                           .arg(filePath, file.errorString());
        }
        return false;
    }
    
    qint64 written = file.write(data);
    file.close();
    
    if (written != data.size()) {
        if (errorMessage) {
            *errorMessage = QString("Failed to write all data to file: %1").arg(filePath);
        }
        return false;
    }
    
    return true;
}

bool FileUtils::appendFile(const QString &filePath, const QString &content, QString *errorMessage) {
    QFile file(filePath);
    
    if (!file.open(QIODevice::WriteOnly | QIODevice::Append)) {
        if (errorMessage) {
            *errorMessage = QString("Cannot open file for appending: %1 - %2")
                           .arg(filePath, file.errorString());
        }
        return false;
    }
    
    QByteArray data = encodeString(content, FileEncoding::UTF8);
    qint64 written = file.write(data);
    file.close();
    
    if (written != data.size()) {
        if (errorMessage) {
            *errorMessage = QString("Failed to append all data to file: %1").arg(filePath);
        }
        return false;
    }
    
    return true;
}

bool FileUtils::writeFileLines(const QString &filePath, const QStringList &lines, QString *errorMessage) {
    QString content = lines.join('\n');
    return writeFile(filePath, content, errorMessage);
}

// ============= File Information =============

bool FileUtils::fileExists(const QString &filePath) {
    return QFile::exists(filePath);
}

qint64 FileUtils::fileSize(const QString &filePath) {
    QFileInfo fileInfo(filePath);
    if (!fileInfo.exists() || !fileInfo.isFile()) {
        return -1;
    }
    return fileInfo.size();
}

FileEncoding FileUtils::detectEncoding(const QString &filePath) {
    QByteArray data;
    if (!readFileBytes(filePath, data)) {
        return FileEncoding::UTF8; // Default to UTF-8
    }
    
    return detectEncoding(data);
}

FileEncoding FileUtils::detectEncoding(const QByteArray &data) {
    if (data.isEmpty()) {
        return FileEncoding::UTF8;
    }
    
    // Check for UTF-8 BOM
    if (hasUtf8Bom(data)) {
        return FileEncoding::UTF8BOM;
    }
    
    // Check for UTF-16 BOM
    bool littleEndian = false;
    if (hasUtf16Bom(data, littleEndian)) {
        return littleEndian ? FileEncoding::UTF16LE : FileEncoding::UTF16BE;
    }
    
    // Try to detect UTF-8 by validating sequences
    bool probablyUtf8 = true;
    int i = 0;
    while (i < data.size()) {
        unsigned char c = static_cast<unsigned char>(data[i]);
        
        if (c < 0x80) {
            // ASCII character
            i++;
        } else if ((c & 0xE0) == 0xC0) {
            // 2-byte sequence
            if (i + 1 >= data.size() || (data[i + 1] & 0xC0) != 0x80) {
                probablyUtf8 = false;
                break;
            }
            i += 2;
        } else if ((c & 0xF0) == 0xE0) {
            // 3-byte sequence
            if (i + 2 >= data.size() || 
                (data[i + 1] & 0xC0) != 0x80 || 
                (data[i + 2] & 0xC0) != 0x80) {
                probablyUtf8 = false;
                break;
            }
            i += 3;
        } else if ((c & 0xF8) == 0xF0) {
            // 4-byte sequence
            if (i + 3 >= data.size() || 
                (data[i + 1] & 0xC0) != 0x80 || 
                (data[i + 2] & 0xC0) != 0x80 || 
                (data[i + 3] & 0xC0) != 0x80) {
                probablyUtf8 = false;
                break;
            }
            i += 4;
        } else {
            probablyUtf8 = false;
            break;
        }
    }
    
    if (probablyUtf8) {
        return FileEncoding::UTF8;
    }
    
    // Check for GBK/GB2312 characteristics (Chinese)
    // GBK uses two bytes for Chinese characters: first byte 0x81-0xFE, second byte 0x40-0xFE
    bool probablyGbk = false;
    for (int j = 0; j < data.size() - 1; j++) {
        unsigned char first = static_cast<unsigned char>(data[j]);
        unsigned char second = static_cast<unsigned char>(data[j + 1]);
        
        if (first >= 0x81 && first <= 0xFE && 
            ((second >= 0x40 && second <= 0x7E) || (second >= 0x80 && second <= 0xFE))) {
            probablyGbk = true;
            break;
        }
    }
    
    if (probablyGbk) {
        return FileEncoding::GBK;
    }
    
    // Default to system encoding
    return FileEncoding::System;
}

QString FileUtils::encodingName(FileEncoding encoding) {
    switch (encoding) {
        case FileEncoding::AutoDetect: return "Auto-Detect";
        case FileEncoding::UTF8: return "UTF-8";
        case FileEncoding::UTF8BOM: return "UTF-8 with BOM";
        case FileEncoding::UTF16LE: return "UTF-16 LE";
        case FileEncoding::UTF16BE: return "UTF-16 BE";
        case FileEncoding::GBK: return "GBK";
        case FileEncoding::GB2312: return "GB2312";
        case FileEncoding::GB18030: return "GB18030";
        case FileEncoding::Big5: return "Big5";
        case FileEncoding::ShiftJIS: return "Shift-JIS";
        case FileEncoding::ISO_8859_1: return "ISO-8859-1";
        case FileEncoding::System: return "System";
        default: return "Unknown";
    }
}

// ============= Utility Operations =============

bool FileUtils::copyFile(const QString &sourcePath, const QString &destPath, 
                        bool overwrite, QString *errorMessage) {
    if (!fileExists(sourcePath)) {
        if (errorMessage) {
            *errorMessage = QString("Source file does not exist: %1").arg(sourcePath);
        }
        return false;
    }
    
    if (fileExists(destPath)) {
        if (!overwrite) {
            if (errorMessage) {
                *errorMessage = QString("Destination file already exists: %1").arg(destPath);
            }
            return false;
        }
        if (!deleteFile(destPath, errorMessage)) {
            return false;
        }
    }
    
    // Ensure destination directory exists
    QFileInfo destInfo(destPath);
    if (!createDirectory(destInfo.absolutePath(), errorMessage)) {
        return false;
    }
    
    if (!QFile::copy(sourcePath, destPath)) {
        if (errorMessage) {
            *errorMessage = QString("Failed to copy file from %1 to %2")
                           .arg(sourcePath, destPath);
        }
        return false;
    }
    
    return true;
}

bool FileUtils::moveFile(const QString &sourcePath, const QString &destPath, 
                        bool overwrite, QString *errorMessage) {
    if (!fileExists(sourcePath)) {
        if (errorMessage) {
            *errorMessage = QString("Source file does not exist: %1").arg(sourcePath);
        }
        return false;
    }
    
    if (fileExists(destPath)) {
        if (!overwrite) {
            if (errorMessage) {
                *errorMessage = QString("Destination file already exists: %1").arg(destPath);
            }
            return false;
        }
        if (!deleteFile(destPath, errorMessage)) {
            return false;
        }
    }
    
    // Ensure destination directory exists
    QFileInfo destInfo(destPath);
    if (!createDirectory(destInfo.absolutePath(), errorMessage)) {
        return false;
    }
    
    if (!QFile::rename(sourcePath, destPath)) {
        if (errorMessage) {
            *errorMessage = QString("Failed to move file from %1 to %2")
                           .arg(sourcePath, destPath);
        }
        return false;
    }
    
    return true;
}

bool FileUtils::deleteFile(const QString &filePath, QString *errorMessage) {
    if (!fileExists(filePath)) {
        return true; // Already deleted
    }
    
    if (!QFile::remove(filePath)) {
        if (errorMessage) {
            *errorMessage = QString("Failed to delete file: %1").arg(filePath);
        }
        return false;
    }
    
    return true;
}

bool FileUtils::createDirectory(const QString &dirPath, QString *errorMessage) {
    QDir dir;
    if (!dir.mkpath(dirPath)) {
        if (errorMessage) {
            *errorMessage = QString("Failed to create directory: %1").arg(dirPath);
        }
        return false;
    }
    return true;
}

QString FileUtils::fileExtension(const QString &filePath) {
    QFileInfo fileInfo(filePath);
    return fileInfo.suffix();
}

QString FileUtils::fileBaseName(const QString &filePath) {
    QFileInfo fileInfo(filePath);
    return fileInfo.completeBaseName();
}

QString FileUtils::directoryPath(const QString &filePath) {
    QFileInfo fileInfo(filePath);
    return fileInfo.absolutePath();
}

// ============= Private Helper Methods =============

const char* FileUtils::encodingToCodecName(FileEncoding encoding) {
    switch (encoding) {
        case FileEncoding::UTF8:
        case FileEncoding::UTF8BOM:
            return "UTF-8";
        case FileEncoding::UTF16LE:
            return "UTF-16LE";
        case FileEncoding::UTF16BE:
            return "UTF-16BE";
        case FileEncoding::GBK:
            return "GBK";
        case FileEncoding::GB2312:
            return "GB2312";
        case FileEncoding::GB18030:
            return "GB18030";
        case FileEncoding::Big5:
            return "Big5";
        case FileEncoding::ShiftJIS:
            return "Shift-JIS";
        case FileEncoding::ISO_8859_1:
            return "ISO-8859-1";
        case FileEncoding::System:
            return nullptr; // Use system default
        default:
            return "UTF-8";
    }
}

QString FileUtils::decodeString(const QByteArray &data, FileEncoding encoding) {
    // Remove BOM if present
    QByteArray cleanData = removeBom(data);
    
    const char* codecName = encodingToCodecName(encoding);
    
    if (codecName == nullptr) {
        // Use system default
        return QString::fromLocal8Bit(cleanData);
    }
    
    QTextCodec *codec = QTextCodec::codecForName(codecName);
    if (codec) {
        return codec->toUnicode(cleanData);
    }
    
    // Fallback to UTF-8
    return QString::fromUtf8(cleanData);
}

QByteArray FileUtils::encodeString(const QString &str, FileEncoding encoding) {
    const char* codecName = encodingToCodecName(encoding);
    
    if (codecName == nullptr) {
        // Use system default
        return str.toLocal8Bit();
    }
    
    QTextCodec *codec = QTextCodec::codecForName(codecName);
    QByteArray data;
    
    if (codec) {
        data = codec->fromUnicode(str);
    } else {
        // Fallback to UTF-8
        data = str.toUtf8();
    }
    
    // Add BOM for UTF-8 BOM
    if (encoding == FileEncoding::UTF8BOM) {
        data.prepend("\xEF\xBB\xBF");
    }
    
    return data;
}

bool FileUtils::hasUtf8Bom(const QByteArray &data) {
    return data.size() >= 3 && 
           static_cast<unsigned char>(data[0]) == 0xEF &&
           static_cast<unsigned char>(data[1]) == 0xBB &&
           static_cast<unsigned char>(data[2]) == 0xBF;
}

bool FileUtils::hasUtf16Bom(const QByteArray &data, bool &littleEndian) {
    if (data.size() < 2) {
        return false;
    }
    
    // UTF-16 LE BOM: FF FE
    if (static_cast<unsigned char>(data[0]) == 0xFF &&
        static_cast<unsigned char>(data[1]) == 0xFE) {
        littleEndian = true;
        return true;
    }
    
    // UTF-16 BE BOM: FE FF
    if (static_cast<unsigned char>(data[0]) == 0xFE &&
        static_cast<unsigned char>(data[1]) == 0xFF) {
        littleEndian = false;
        return true;
    }
    
    return false;
}

QByteArray FileUtils::removeBom(const QByteArray &data) {
    // Remove UTF-8 BOM
    if (hasUtf8Bom(data)) {
        return data.mid(3);
    }
    
    // Remove UTF-16 BOM
    bool littleEndian;
    if (hasUtf16Bom(data, littleEndian)) {
        return data.mid(2);
    }
    
    return data;
}
