/**
 * @file jsonvalidator.h
 * @brief JSON validation utilities
 */

#ifndef JSONVALIDATOR_H
#define JSONVALIDATOR_H

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QString>
#include <QStringList>
#include <QList>
#include <functional>

/**
 * @brief Validation rule structure
 */
struct ValidationRule {
    QString path;
    std::function<bool(const QJsonValue&)> validator;
    QString errorMessage;
    
    ValidationRule() = default;
    
    ValidationRule(const QString &p, std::function<bool(const QJsonValue&)> v, const QString &err)
        : path(p), validator(v), errorMessage(err) {}
};

/**
 * @brief Validation error structure
 */
struct ValidationError {
    enum ErrorType {
        TypeMismatch,
        MissingField,
        InvalidValue,
        SchemaViolation,
        CustomRule
    };
    
    QString path;
    QString message;
    ErrorType type;
    
    ValidationError() : type(TypeMismatch) {}
    
    ValidationError(const QString &p, const QString &msg, ErrorType t)
        : path(p), message(msg), type(t) {}
    
    QString toString() const {
        return QString("%1: %2").arg(path.isEmpty() ? "root" : path, message);
    }
};

/**
 * @brief JSON validator class
 * 
 * Provides validation capabilities for JSON documents including
 * type checking, required field validation, and custom rules.
 */
class JsonValidator {
public:
    JsonValidator();
    ~JsonValidator();
    
    // Basic validation
    bool validate(const QJsonDocument &doc);
    bool validate(const QJsonObject &obj);
    bool validate(const QJsonArray &arr);
    
    // Schema validation (basic subset)
    bool validateSchema(const QJsonDocument &doc, const QJsonObject &schema);
    bool loadSchema(const QString &schemaFile);
    
    // Custom rules
    void addRule(const QString &path, const ValidationRule &rule);
    void addRule(const QString &path, std::function<bool(const QJsonValue&)> validator, 
                 const QString &errorMessage);
    void clearRules();
    
    // Type checking
    bool isType(const QJsonValue &value, QJsonValue::Type expectedType) const;
    bool hasRequiredFields(const QJsonObject &obj, const QStringList &fields);
    
    // Error reporting
    QList<ValidationError> errors() const;
    QString errorString() const;
    bool hasErrors() const;
    void clearErrors();
    
private:
    QJsonObject m_schema;
    QList<ValidationRule> m_customRules;
    QList<ValidationError> m_errors;
    
    void addError(const QString &path, const QString &message, ValidationError::ErrorType type);
    bool validateAgainstSchema(const QJsonValue &value, const QJsonObject &schema, const QString &path);
    QJsonValue navigateToPath(const QJsonValue &root, const QString &path) const;
};

#endif // JSONVALIDATOR_H
