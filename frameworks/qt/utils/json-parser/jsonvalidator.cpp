/**
 * @file jsonvalidator.cpp
 * @brief JSON validation utilities implementation
 */

#include "jsonvalidator.h"
#include <QFile>
#include <QJsonParseError>

JsonValidator::JsonValidator()
{
}

JsonValidator::~JsonValidator()
{
}

bool JsonValidator::validate(const QJsonDocument &doc)
{
    clearErrors();
    
    if (doc.isNull() || doc.isEmpty()) {
        addError("", "Document is null or empty", ValidationError::InvalidValue);
        return false;
    }
    
    if (doc.isObject()) {
        return validate(doc.object());
    } else if (doc.isArray()) {
        return validate(doc.array());
    }
    
    return true;
}

bool JsonValidator::validate(const QJsonObject &obj)
{
    clearErrors();
    
    // Apply custom rules
    for (const ValidationRule &rule : m_customRules) {
        QJsonValue value = navigateToPath(obj, rule.path);
        if (!value.isUndefined() && rule.validator) {
            if (!rule.validator(value)) {
                addError(rule.path, rule.errorMessage, ValidationError::CustomRule);
            }
        }
    }
    
    return !hasErrors();
}

bool JsonValidator::validate(const QJsonArray &arr)
{
    clearErrors();
    
    // Basic array validation
    if (arr.isEmpty()) {
        return true;
    }
    
    return !hasErrors();
}

bool JsonValidator::validateSchema(const QJsonDocument &doc, const QJsonObject &schema)
{
    clearErrors();
    m_schema = schema;
    
    if (doc.isObject()) {
        return validateAgainstSchema(doc.object(), schema, "");
    } else if (doc.isArray()) {
        return validateAgainstSchema(doc.array(), schema, "");
    }
    
    return !hasErrors();
}

bool JsonValidator::loadSchema(const QString &schemaFile)
{
    QFile file(schemaFile);
    if (!file.open(QIODevice::ReadOnly)) {
        addError("", "Cannot open schema file: " + schemaFile, ValidationError::SchemaViolation);
        return false;
    }
    
    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(file.readAll(), &error);
    file.close();
    
    if (error.error != QJsonParseError::NoError) {
        addError("", "Schema parse error: " + error.errorString(), ValidationError::SchemaViolation);
        return false;
    }
    
    if (!doc.isObject()) {
        addError("", "Schema must be a JSON object", ValidationError::SchemaViolation);
        return false;
    }
    
    m_schema = doc.object();
    return true;
}

void JsonValidator::addRule(const QString &path, const ValidationRule &rule)
{
    m_customRules.append(rule);
}

void JsonValidator::addRule(const QString &path, std::function<bool(const QJsonValue&)> validator,
                           const QString &errorMessage)
{
    ValidationRule rule(path, validator, errorMessage);
    m_customRules.append(rule);
}

void JsonValidator::clearRules()
{
    m_customRules.clear();
}

bool JsonValidator::isType(const QJsonValue &value, QJsonValue::Type expectedType) const
{
    return value.type() == expectedType;
}

bool JsonValidator::hasRequiredFields(const QJsonObject &obj, const QStringList &fields)
{
    clearErrors();
    
    for (const QString &field : fields) {
        if (!obj.contains(field)) {
            addError(field, "Required field missing", ValidationError::MissingField);
        }
    }
    
    return !hasErrors();
}

QList<ValidationError> JsonValidator::errors() const
{
    return m_errors;
}

QString JsonValidator::errorString() const
{
    if (m_errors.isEmpty()) {
        return QString();
    }
    
    QStringList errorStrings;
    for (const ValidationError &error : m_errors) {
        errorStrings.append(error.toString());
    }
    
    return errorStrings.join("; ");
}

bool JsonValidator::hasErrors() const
{
    return !m_errors.isEmpty();
}

void JsonValidator::clearErrors()
{
    m_errors.clear();
}

void JsonValidator::addError(const QString &path, const QString &message, ValidationError::ErrorType type)
{
    m_errors.append(ValidationError(path, message, type));
}

bool JsonValidator::validateAgainstSchema(const QJsonValue &value, const QJsonObject &schema, const QString &path)
{
    // Basic schema validation implementation
    // This is a simplified version - full JSON Schema support would be more complex
    
    // Check type
    if (schema.contains("type")) {
        QString typeStr = schema["type"].toString();
        QJsonValue::Type expectedType = QJsonValue::Undefined;
        
        if (typeStr == "object") expectedType = QJsonValue::Object;
        else if (typeStr == "array") expectedType = QJsonValue::Array;
        else if (typeStr == "string") expectedType = QJsonValue::String;
        else if (typeStr == "number") expectedType = QJsonValue::Double;
        else if (typeStr == "boolean") expectedType = QJsonValue::Bool;
        else if (typeStr == "null") expectedType = QJsonValue::Null;
        
        if (expectedType != QJsonValue::Undefined && value.type() != expectedType) {
            addError(path, QString("Type mismatch: expected %1").arg(typeStr), 
                    ValidationError::TypeMismatch);
            return false;
        }
    }
    
    // Check required properties for objects
    if (value.isObject() && schema.contains("required")) {
        QJsonArray required = schema["required"].toArray();
        QJsonObject obj = value.toObject();
        
        for (const QJsonValue &reqValue : required) {
            QString reqField = reqValue.toString();
            if (!obj.contains(reqField)) {
                addError(path.isEmpty() ? reqField : path + "." + reqField,
                        "Required field missing", ValidationError::MissingField);
            }
        }
    }
    
    // Check properties for objects
    if (value.isObject() && schema.contains("properties")) {
        QJsonObject properties = schema["properties"].toObject();
        QJsonObject obj = value.toObject();
        
        for (auto it = properties.begin(); it != properties.end(); ++it) {
            QString propName = it.key();
            if (obj.contains(propName)) {
                QString propPath = path.isEmpty() ? propName : path + "." + propName;
                validateAgainstSchema(obj[propName], it.value().toObject(), propPath);
            }
        }
    }
    
    return !hasErrors();
}

QJsonValue JsonValidator::navigateToPath(const QJsonValue &root, const QString &path) const
{
    if (path.isEmpty()) {
        return root;
    }
    
    QStringList parts = path.split('.');
    QJsonValue current = root;
    
    for (const QString &part : parts) {
        if (current.isObject()) {
            current = current.toObject().value(part);
        } else if (current.isArray()) {
            bool ok;
            int index = part.toInt(&ok);
            if (ok && index >= 0 && index < current.toArray().size()) {
                current = current.toArray().at(index);
            } else {
                return QJsonValue(QJsonValue::Undefined);
            }
        } else {
            return QJsonValue(QJsonValue::Undefined);
        }
    }
    
    return current;
}
