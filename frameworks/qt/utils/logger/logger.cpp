/**
 * @file logger.cpp
 * @brief Implementation of Logger class
 */

#include "logger.h"
#include <QFileInfo>
#include <QDebug>

Logger::Logger()
    : m_logDirectory("logs")
    , m_maxFileCount(7)
    , m_minLogLevel(LogLevel::Debug)
{
}

Logger::~Logger() {
}

Logger& Logger::instance() {
    static Logger instance;
    return instance;
}

void Logger::setLogDirectory(const QString &dirPath) {
    QMutexLocker locker(&m_mutex);
    m_logDirectory = dirPath;
    
    // Create directory if it doesn't exist
    QDir dir;
    if (!dir.exists(m_logDirectory)) {
        dir.mkpath(m_logDirectory);
    }
}

void Logger::setMaxFileCount(int count) {
    QMutexLocker locker(&m_mutex);
    m_maxFileCount = count;
}

void Logger::setMinLogLevel(LogLevel level) {
    QMutexLocker locker(&m_mutex);
    m_minLogLevel = level;
}

void Logger::debug(const QString &message) {
    instance().log(LogLevel::Debug, message);
}

void Logger::info(const QString &message) {
    instance().log(LogLevel::Info, message);
}

void Logger::warning(const QString &message) {
    instance().log(LogLevel::Warning, message);
}

void Logger::error(const QString &message) {
    instance().log(LogLevel::Error, message);
}

void Logger::log(LogLevel level, const QString &message) {
    // Check if this level should be logged
    if (level < m_minLogLevel) {
        return;
    }
    
    writeLog(level, message);
}

QString Logger::levelToString(LogLevel level) {
    switch (level) {
        case LogLevel::Debug:   return "DEBUG";
        case LogLevel::Info:    return "INFO";
        case LogLevel::Warning: return "WARNING";
        case LogLevel::Error:   return "ERROR";
        default:                return "UNKNOWN";
    }
}

QString Logger::getCurrentLogFile() {
    // Create log file name based on current date: app_YYYYMMDD.log
    QString date = QDateTime::currentDateTime().toString("yyyyMMdd");
    QString fileName = QString("app_%1.log").arg(date);
    return m_logDirectory + "/" + fileName;
}

void Logger::cleanOldLogFiles() {
    QDir dir(m_logDirectory);
    if (!dir.exists()) {
        return;
    }
    
    // Get all log files
    QStringList filters;
    filters << "app_*.log";
    QFileInfoList files = dir.entryInfoList(filters, QDir::Files, QDir::Time | QDir::Reversed);
    
    // Remove oldest files if count exceeds limit
    while (files.size() > m_maxFileCount) {
        QFileInfo oldestFile = files.takeLast();
        QFile::remove(oldestFile.absoluteFilePath());
    }
}

void Logger::writeLog(LogLevel level, const QString &message) {
    QMutexLocker locker(&m_mutex);
    
    // Ensure log directory exists
    QDir dir;
    if (!dir.exists(m_logDirectory)) {
        dir.mkpath(m_logDirectory);
    }
    
    // Get current log file path
    QString logFile = getCurrentLogFile();
    
    // Open file in append mode
    QFile file(logFile);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
        qWarning() << "Cannot open log file:" << logFile;
        return;
    }
    
    // Format log message
    QString timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    QString levelStr = levelToString(level);
    QString logMessage = QString("[%1] [%2] %3\n").arg(timestamp, levelStr, message);
    
    // Write to file
    QTextStream out(&file);
    out << logMessage;
    file.close();
    
    // Clean up old files
    cleanOldLogFiles();
}
