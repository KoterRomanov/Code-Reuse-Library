/**
 * @file jsonhelper.h
 * @brief High-level JSON utility wrapper class
 * 
 * This class provides a simplified interface for JSON operations by wrapping
 * JsonParser, JsonQuery, and JsonValidator into a single, easy-to-use class.
 * It hides implementation details and provides intuitive methods for common tasks.
 */

#ifndef JSONHELPER_H
#define JSONHELPER_H

#include <QString>
#include <QStringList>
#include <QVariant>
#include <QJsonValue>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <functional>

// Forward declarations to hide implementation details
class JsonParser;
class JsonQuery;
class JsonValidator;
struct ValidationRule;

/**
 * @brief Simplified JSON utility class
 * 
 * JsonHelper provides a simple, intuitive interface for all JSON operations.
 * It hides the complexity of the underlying components and provides a
 * streamlined API for common use cases.
 * 
 * Example usage:
 * @code
 * JsonHelper json;
 * json.load("config.json");
 * QString name = json.get("user.name").toString();
 * json.set("user.age", 30);
 * json.save("config.json");
 * @endcode
 */
class JsonHelper {
public:
    /**
     * @brief Constructor
     */
    JsonHelper();
    
    /**
     * @brief Constructor with JSON string
     * @param jsonString JSON string to parse
     */
    explicit JsonHelper(const QString &jsonString);
    
    /**
     * @brief Destructor
     */
    ~JsonHelper();
    
    // ============= Loading / Parsing =============
    
    /**
     * @brief Load JSON from string
     * @param jsonString JSON string
     * @return true if successful, false otherwise
     */
    bool parse(const QString &jsonString);
    
    /**
     * @brief Load JSON from file
     * @param filePath Path to JSON file
     * @return true if successful, false otherwise
     */
    bool load(const QString &filePath);
    
    /**
     * @brief Load JSON from byte array
     * @param data Byte array containing JSON
     * @return true if successful, false otherwise
     */
    bool loadFromBytes(const QByteArray &data);
    
    /**
     * @brief Load JSON from QVariant (QVariantMap or QVariantList)
     * @param variant QVariant to convert
     * @return true if successful, false otherwise
     */
    bool loadFromVariant(const QVariant &variant);
    
    // ============= Saving / Serialization =============
    
    /**
     * @brief Save JSON to file
     * @param filePath Path to save file
     * @param prettyPrint Whether to format with indentation (default: true)
     * @return true if successful, false otherwise
     */
    bool save(const QString &filePath, bool prettyPrint = true);
    
    /**
     * @brief Convert JSON to string
     * @param prettyPrint Whether to format with indentation (default: true)
     * @return JSON string
     */
    QString toString(bool prettyPrint = true) const;
    
    /**
     * @brief Convert JSON to byte array
     * @param prettyPrint Whether to format with indentation (default: false)
     * @return JSON byte array
     */
    QByteArray toBytes(bool prettyPrint = false) const;
    
    // ============= Querying =============
    
    /**
     * @brief Get value at path (supports dot notation and array indices)
     * @param path Path to value (e.g., "user.name", "items[0].title")
     * @param defaultValue Default value if path doesn't exist
     * @return Value at path or default value
     * 
     * Examples:
     * - "name" -> root["name"]
     * - "user.email" -> root["user"]["email"]
     * - "items[0]" -> root["items"][0]
     * - "data.list[2].value" -> root["data"]["list"][2]["value"]
     */
    QJsonValue get(const QString &path, const QJsonValue &defaultValue = QJsonValue()) const;
    
    /**
     * @brief Get string value at path
     * @param path Path to value
     * @param defaultValue Default value if not found or wrong type
     * @return String value
     */
    QString getString(const QString &path, const QString &defaultValue = QString()) const;
    
    /**
     * @brief Get integer value at path
     * @param path Path to value
     * @param defaultValue Default value if not found or wrong type
     * @return Integer value
     */
    int getInt(const QString &path, int defaultValue = 0) const;
    
    /**
     * @brief Get double value at path
     * @param path Path to value
     * @param defaultValue Default value if not found or wrong type
     * @return Double value
     */
    double getDouble(const QString &path, double defaultValue = 0.0) const;
    
    /**
     * @brief Get boolean value at path
     * @param path Path to value
     * @param defaultValue Default value if not found or wrong type
     * @return Boolean value
     */
    bool getBool(const QString &path, bool defaultValue = false) const;
    
    /**
     * @brief Get object at path
     * @param path Path to object
     * @return QJsonObject at path or empty object
     */
    QJsonObject getObject(const QString &path = QString()) const;
    
    /**
     * @brief Get array at path
     * @param path Path to array
     * @return QJsonArray at path or empty array
     */
    QJsonArray getArray(const QString &path = QString()) const;
    
    /**
     * @brief Get all values matching wildcard path
     * @param path Wildcard path (e.g., "users[*].name")
     * @return List of matching values
     */
    QList<QJsonValue> getAll(const QString &path) const;
    
    /**
     * @brief Check if path exists
     * @param path Path to check
     * @return true if path exists, false otherwise
     */
    bool has(const QString &path) const;
    
    /**
     * @brief Get all keys at path (for objects)
     * @param path Path to object (empty for root)
     * @return List of keys
     */
    QStringList keys(const QString &path = QString()) const;
    
    // ============= Modifying =============
    
    /**
     * @brief Set value at path
     * @param path Path where to set value
     * @param value Value to set
     * @return Reference to this for chaining
     */
    JsonHelper& set(const QString &path, const QJsonValue &value);
    
    /**
     * @brief Set string value at path
     * @param path Path where to set value
     * @param value String value
     * @return Reference to this for chaining
     */
    JsonHelper& setString(const QString &path, const QString &value);
    
    /**
     * @brief Set integer value at path
     * @param path Path where to set value
     * @param value Integer value
     * @return Reference to this for chaining
     */
    JsonHelper& setInt(const QString &path, int value);
    
    /**
     * @brief Set double value at path
     * @param path Path where to set value
     * @param value Double value
     * @return Reference to this for chaining
     */
    JsonHelper& setDouble(const QString &path, double value);
    
    /**
     * @brief Set boolean value at path
     * @param path Path where to set value
     * @param value Boolean value
     * @return Reference to this for chaining
     */
    JsonHelper& setBool(const QString &path, bool value);
    
    /**
     * @brief Remove value at path
     * @param path Path to remove
     * @return Reference to this for chaining
     */
    JsonHelper& remove(const QString &path);
    
    /**
     * @brief Merge another JSON object into current (at root or specified path)
     * @param other Object to merge
     * @param path Path where to merge (empty for root)
     * @return Reference to this for chaining
     */
    JsonHelper& merge(const QJsonObject &other, const QString &path = QString());
    
    /**
     * @brief Clear all data
     * @return Reference to this for chaining
     */
    JsonHelper& clear();
    
    // ============= Validation =============
    
    /**
     * @brief Check if loaded JSON is valid
     * @return true if valid, false otherwise
     */
    bool isValid() const;
    
    /**
     * @brief Validate current JSON data
     * @return true if valid, false otherwise
     */
    bool validate();
    
    /**
     * @brief Validate against JSON schema
     * @param schemaJson JSON schema as object
     * @return true if valid, false otherwise
     */
    bool validateWithSchema(const QJsonObject &schemaJson);
    
    /**
     * @brief Validate against JSON schema from file
     * @param schemaFilePath Path to schema file
     * @return true if valid, false otherwise
     */
    bool validateWithSchemaFile(const QString &schemaFilePath);
    
    /**
     * @brief Add custom validation rule
     * @param path Path to validate
     * @param validator Validation function (returns true if valid)
     * @param errorMessage Error message if validation fails
     * @return Reference to this for chaining
     */
    JsonHelper& addValidationRule(const QString &path, 
                                   std::function<bool(const QJsonValue&)> validator,
                                   const QString &errorMessage);
    
    /**
     * @brief Require field to exist
     * @param path Path to required field
     * @return Reference to this for chaining
     */
    JsonHelper& requireField(const QString &path);
    
    /**
     * @brief Require fields to exist
     * @param paths Paths to required fields
     * @return Reference to this for chaining
     */
    JsonHelper& requireFields(const QStringList &paths);
    
    /**
     * @brief Clear all validation rules
     * @return Reference to this for chaining
     */
    JsonHelper& clearValidationRules();
    
    // ============= Searching / Filtering =============
    
    /**
     * @brief Find array items matching key-value pair
     * @param arrayPath Path to array
     * @param key Key to match
     * @param value Value to match
     * @return Array of matching items
     */
    QJsonArray find(const QString &arrayPath, const QString &key, const QJsonValue &value) const;
    
    /**
     * @brief Filter array items using predicate
     * @param arrayPath Path to array
     * @param predicate Filter function (returns true to include item)
     * @return Filtered array
     */
    QJsonArray filter(const QString &arrayPath, std::function<bool(const QJsonValue&)> predicate) const;
    
    /**
     * @brief Map array items using transformer function
     * @param arrayPath Path to array
     * @param mapper Transform function
     * @return Transformed array
     */
    QJsonArray map(const QString &arrayPath, std::function<QJsonValue(const QJsonValue&)> mapper) const;
    
    // ============= Conversion =============
    
    /**
     * @brief Convert to QVariantMap
     * @return QVariantMap representation
     */
    QVariantMap toVariantMap() const;
    
    /**
     * @brief Convert to QVariantList
     * @return QVariantList representation
     */
    QVariantList toVariantList() const;
    
    /**
     * @brief Get underlying QJsonDocument
     * @return QJsonDocument
     */
    QJsonDocument document() const;
    
    // ============= Error Handling =============
    
    /**
     * @brief Get last error message
     * @return Error message or empty string if no error
     */
    QString lastError() const;
    
    /**
     * @brief Check if there are any errors
     * @return true if errors exist, false otherwise
     */
    bool hasError() const;
    
    /**
     * @brief Get all validation errors
     * @return List of error messages
     */
    QStringList validationErrors() const;
    
    // ============= Utility Methods =============
    
    /**
     * @brief Check if JSON is empty
     * @return true if empty, false otherwise
     */
    bool isEmpty() const;
    
    /**
     * @brief Check if root is an object
     * @return true if object, false otherwise
     */
    bool isObject() const;
    
    /**
     * @brief Check if root is an array
     * @return true if array, false otherwise
     */
    bool isArray() const;
    
    /**
     * @brief Get size (for arrays) or key count (for objects)
     * @param path Path to check (empty for root)
     * @return Size or count
     */
    int size(const QString &path = QString()) const;
    
    // ============= Static Utility Methods =============
    
    /**
     * @brief Quick parse JSON string
     * @param jsonString JSON string
     * @return JsonHelper instance (check isValid())
     */
    static JsonHelper fromString(const QString &jsonString);
    
    /**
     * @brief Quick load JSON from file
     * @param filePath Path to JSON file
     * @return JsonHelper instance (check isValid())
     */
    static JsonHelper fromFile(const QString &filePath);
    
    /**
     * @brief Quick convert QVariant to JSON
     * @param variant QVariant to convert
     * @return JsonHelper instance (check isValid())
     */
    static JsonHelper fromVariant(const QVariant &variant);
    
    /**
     * @brief Create empty JSON object
     * @return JsonHelper with empty object
     */
    static JsonHelper createObject();
    
    /**
     * @brief Create empty JSON array
     * @return JsonHelper with empty array
     */
    static JsonHelper createArray();
    
private:
    // Implementation details hidden using pimpl idiom
    class JsonHelperPrivate;
    JsonHelperPrivate *d;
    
    // Disable copy (use explicit methods if needed)
    JsonHelper(const JsonHelper&) = delete;
    JsonHelper& operator=(const JsonHelper&) = delete;
};

#endif // JSONHELPER_H
