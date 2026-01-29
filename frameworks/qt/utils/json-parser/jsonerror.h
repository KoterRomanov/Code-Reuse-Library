/**
 * @file jsonerror.h
 * @brief JSON error information structure
 */

#ifndef JSONERROR_H
#define JSONERROR_H

#include <QString>

/**
 * @brief Comprehensive error information for JSON operations
 */
struct JsonError {
    enum ErrorType {
        NoError,
        ParseError,
        FileError,
        EncodingError,
        ValidationError,
        PathError,
        UnknownError
    };
    
    ErrorType type = NoError;
    QString message;
    int line = -1;
    int column = -1;
    int offset = -1;
    
    JsonError() = default;
    
    JsonError(ErrorType t, const QString &msg, int l = -1, int c = -1, int o = -1)
        : type(t), message(msg), line(l), column(c), offset(o) {}
    
    /**
     * @brief Check if this represents an error
     * @return true if error, false otherwise
     */
    bool isError() const { return type != NoError; }
    
    /**
     * @brief Convert error to string representation
     * @return Formatted error string
     */
    QString toString() const {
        if (type == NoError) {
            return "No error";
        }
        
        QString result = message;
        if (line >= 0) {
            result += QString(" (line %1, column %2)").arg(line).arg(column);
        }
        return result;
    }
    
    /**
     * @brief Get error type as string
     * @return Error type name
     */
    QString typeName() const {
        switch (type) {
            case NoError: return "No Error";
            case ParseError: return "Parse Error";
            case FileError: return "File Error";
            case EncodingError: return "Encoding Error";
            case ValidationError: return "Validation Error";
            case PathError: return "Path Error";
            case UnknownError: return "Unknown Error";
            default: return "Unknown";
        }
    }
};

#endif // JSONERROR_H
