/**
 * @file logger.h
 * @brief Simple and efficient logging utility for Qt projects
 * 
 * Features:
 * - Multiple log levels (Debug, Info, Warning, Error)
 * - Configurable output path
 * - Automatic file rotation with count limit
 * - Thread-safe operations
 * - Easy to integrate into any Qt project
 * 
 * @author Code Reuse Library
 * @date 2026-02-03
 * @version 1.0.0
 */

#ifndef LOGGER_H
#define LOGGER_H

#include <QString>
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QMutex>
#include <QDir>

/**
 * @brief Log level enumeration
 */
enum class LogLevel {
    Debug = 0,      ///< Debug messages
    Info = 1,       ///< Informational messages
    Warning = 2,    ///< Warning messages
    Error = 3       ///< Error messages
};

/**
 * @brief Simple logging utility class
 * 
 * This class provides basic logging functionality with:
 * - Configurable output directory
 * - Log level filtering
 * - Automatic file rotation
 * - Thread-safe operations
 * 
 * Example usage:
 * @code
 * // Initialize logger
 * Logger::instance().setLogDirectory("logs");
 * Logger::instance().setMaxFileCount(5);
 * Logger::instance().setMinLogLevel(LogLevel::Info);
 * 
 * // Write logs
 * Logger::debug("Debug message");
 * Logger::info("Application started");
 * Logger::warning("Warning message");
 * Logger::error("Error occurred");
 * @endcode
 */
class Logger {
public:
    /**
     * @brief Get singleton instance
     * @return Logger instance
     */
    static Logger& instance();
    
    /**
     * @brief Set log output directory
     * @param dirPath Directory path for log files
     * 
     * If directory doesn't exist, it will be created automatically.
     * Log files are named: app_YYYYMMDD.log
     */
    void setLogDirectory(const QString &dirPath);
    
    /**
     * @brief Set maximum number of log files to keep
     * @param count Maximum file count (default: 7)
     * 
     * When this limit is exceeded, oldest log files will be deleted automatically.
     */
    void setMaxFileCount(int count);
    
    /**
     * @brief Set minimum log level
     * @param level Minimum level to log (default: Debug)
     * 
     * Only messages with this level or higher will be written.
     */
    void setMinLogLevel(LogLevel level);
    
    /**
     * @brief Write debug message
     * @param message Message to log
     */
    static void debug(const QString &message);
    
    /**
     * @brief Write info message
     * @param message Message to log
     */
    static void info(const QString &message);
    
    /**
     * @brief Write warning message
     * @param message Message to log
     */
    static void warning(const QString &message);
    
    /**
     * @brief Write error message
     * @param message Message to log
     */
    static void error(const QString &message);
    
    /**
     * @brief Write log message with specific level
     * @param level Log level
     * @param message Message to log
     */
    void log(LogLevel level, const QString &message);
    
    /**
     * @brief Get log level name as string
     * @param level Log level
     * @return Level name (e.g., "DEBUG", "INFO", "WARNING", "ERROR")
     */
    static QString levelToString(LogLevel level);
    
private:
    Logger();
    ~Logger();
    
    // Disable copy and assignment
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;
    
    /**
     * @brief Get current log file path
     * @return Full path to current log file
     */
    QString getCurrentLogFile();
    
    /**
     * @brief Clean up old log files
     * 
     * Removes oldest log files if count exceeds maxFileCount
     */
    void cleanOldLogFiles();
    
    /**
     * @brief Write message to log file
     * @param level Log level
     * @param message Message to write
     */
    void writeLog(LogLevel level, const QString &message);
    
    QString m_logDirectory;     ///< Log output directory
    int m_maxFileCount;         ///< Maximum number of log files
    LogLevel m_minLogLevel;     ///< Minimum log level to write
    QMutex m_mutex;             ///< Mutex for thread safety
};

#endif // LOGGER_H
