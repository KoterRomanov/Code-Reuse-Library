/**
 * @file jsonparser.h
 * @brief Main JSON parser interface
 */

#ifndef JSONPARSER_H
#define JSONPARSER_H

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QString>
#include <QStringList>
#include <QVariantMap>
#include <QVariantList>
#include "jsonerror.h"
#include "jsonvalidator.h"
#include "jsonquery.h"

/**
 * @brief Main JSON parser class providing unified interface for JSON operations
 * 
 * This class provides both static convenience methods for quick operations
 * and instance methods for more complex workflows with chaining support.
 */
class JsonParser {
public:
    JsonParser();
    ~JsonParser();
    
    // ============= Static Convenience Methods =============
    
    /**
     * @brief Parse JSON string
     * @param jsonString JSON string to parse
     * @param error Optional error output parameter
     * @return Parsed JSON document
     */
    static QJsonDocument parse(const QString &jsonString, QString *error = nullptr);
    
    /**
     * @brief Parse JSON from file
     * @param filePath Path to JSON file
     * @param error Optional error output parameter
     * @return Parsed JSON document
     */
    static QJsonDocument parseFile(const QString &filePath, QString *error = nullptr);
    
    /**
     * @brief Save JSON document to file
     * @param doc JSON document to save
     * @param filePath Path to save file
     * @param pretty Whether to format with indentation
     * @return true on success, false on failure
     */
    static bool save(const QJsonDocument &doc, const QString &filePath, bool pretty = true);
    
    /**
     * @brief Convert JSON document to string
     * @param doc JSON document
     * @param pretty Whether to format with indentation
     * @return JSON string
     */
    static QString toString(const QJsonDocument &doc, bool pretty = true);
    
    // ============= Instance Methods for Chaining =============
    
    /**
     * @brief Load JSON from string
     * @param jsonString JSON string
     * @return Reference to this for chaining
     */
    JsonParser& fromString(const QString &jsonString);
    
    /**
     * @brief Load JSON from file
     * @param filePath Path to JSON file
     * @return Reference to this for chaining
     */
    JsonParser& fromFile(const QString &filePath);
    
    /**
     * @brief Load JSON from byte array
     * @param data Byte array containing JSON
     * @return Reference to this for chaining
     */
    JsonParser& fromByteArray(const QByteArray &data);
    
    /**
     * @brief Load JSON from QVariant
     * @param variant QVariant to convert
     * @return Reference to this for chaining
     */
    JsonParser& fromVariant(const QVariant &variant);
    
    // ============= Validation =============
    
    /**
     * @brief Check if loaded JSON is valid
     * @return true if valid, false otherwise
     */
    bool isValid() const;
    
    /**
     * @brief Get error string
     * @return Error message
     */
    QString errorString() const;
    
    /**
     * @brief Validate JSON structure
     * @return true if valid, false otherwise
     */
    bool validate();
    
    /**
     * @brief Validate against JSON schema
     * @param schema Schema object
     * @return true if valid, false otherwise
     */
    bool validateSchema(const QJsonObject &schema);
    
    // ============= Conversion =============
    
    /**
     * @brief Get JSON document
     * @return JSON document
     */
    QJsonDocument document() const;
    
    /**
     * @brief Get as JSON object
     * @return JSON object
     */
    QJsonObject object() const;
    
    /**
     * @brief Get as JSON array
     * @return JSON array
     */
    QJsonArray array() const;
    
    /**
     * @brief Convert to QVariantMap
     * @return Variant map
     */
    QVariantMap toVariantMap() const;
    
    /**
     * @brief Convert to QVariantList
     * @return Variant list
     */
    QVariantList toVariantList() const;
    
    /**
     * @brief Convert to string
     * @param pretty Whether to format with indentation
     * @return JSON string
     */
    QString toString(bool pretty = true) const;
    
    // ============= Query Operations =============
    
    /**
     * @brief Get value at path
     * @param path Dot notation path (e.g., "user.name")
     * @param defaultValue Default value if path not found
     * @return Value at path or default value
     */
    QJsonValue value(const QString &path, const QJsonValue &defaultValue = QJsonValue()) const;
    
    /**
     * @brief Check if path exists
     * @param path Dot notation path
     * @return true if path exists, false otherwise
     */
    bool contains(const QString &path) const;
    
    /**
     * @brief Get all keys at root level
     * @return List of keys
     */
    QStringList keys() const;
    
    // ============= Modification =============
    
    /**
     * @brief Set value at path
     * @param path Dot notation path
     * @param value Value to set
     * @return Reference to this for chaining
     */
    JsonParser& setValue(const QString &path, const QJsonValue &value);
    
    /**
     * @brief Remove value at path
     * @param path Dot notation path
     * @return Reference to this for chaining
     */
    JsonParser& remove(const QString &path);
    
    /**
     * @brief Merge with another JSON object
     * @param other Object to merge
     * @return Reference to this for chaining
     */
    JsonParser& merge(const QJsonObject &other);
    
    // ============= File Operations =============
    
    /**
     * @brief Save to file
     * @param filePath Path to save file
     * @param pretty Whether to format with indentation
     * @return true on success, false on failure
     */
    bool saveToFile(const QString &filePath, bool pretty = true);
    
    /**
     * @brief Load from file
     * @param filePath Path to JSON file
     * @return true on success, false on failure
     */
    bool loadFromFile(const QString &filePath);
    
private:
    QJsonDocument m_document;
    QString m_errorString;
    bool m_valid;
    
    void setDocument(const QJsonDocument &doc);
    void setError(const QString &error);
    QJsonValue navigateToPath(QJsonValue root, const QString &path) const;
    void setValueAtPath(QJsonValue &root, const QString &path, const QJsonValue &value);
    void removeAtPath(QJsonValue &root, const QString &path);
};

#endif // JSONPARSER_H
