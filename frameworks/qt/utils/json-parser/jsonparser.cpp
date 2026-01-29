/**
 * @file jsonparser.cpp
 * @brief Main JSON parser implementation
 */

#include "jsonparser.h"
#include <QFile>
#include <QJsonParseError>
#include <QVariant>

JsonParser::JsonParser()
    : m_valid(false)
{
}

JsonParser::~JsonParser()
{
}

// ============= Static Convenience Methods =============

QJsonDocument JsonParser::parse(const QString &jsonString, QString *error)
{
    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(jsonString.toUtf8(), &parseError);
    
    if (parseError.error != QJsonParseError::NoError) {
        if (error) {
            *error = parseError.errorString();
        }
        return QJsonDocument();
    }
    
    return doc;
}

QJsonDocument JsonParser::parseFile(const QString &filePath, QString *error)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        if (error) {
            *error = "Cannot open file: " + filePath;
        }
        return QJsonDocument();
    }
    
    QByteArray data = file.readAll();
    file.close();
    
    return parse(QString::fromUtf8(data), error);
}

bool JsonParser::save(const QJsonDocument &doc, const QString &filePath, bool pretty)
{
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly)) {
        return false;
    }
    
    QJsonDocument::JsonFormat format = pretty ? QJsonDocument::Indented : QJsonDocument::Compact;
    file.write(doc.toJson(format));
    file.close();
    
    return true;
}

QString JsonParser::toString(const QJsonDocument &doc, bool pretty)
{
    QJsonDocument::JsonFormat format = pretty ? QJsonDocument::Indented : QJsonDocument::Compact;
    return QString::fromUtf8(doc.toJson(format));
}

// ============= Instance Methods =============

JsonParser& JsonParser::fromString(const QString &jsonString)
{
    QString error;
    QJsonDocument doc = parse(jsonString, &error);
    
    if (doc.isNull()) {
        setError(error);
        m_valid = false;
    } else {
        setDocument(doc);
        m_valid = true;
    }
    
    return *this;
}

JsonParser& JsonParser::fromFile(const QString &filePath)
{
    QString error;
    QJsonDocument doc = parseFile(filePath, &error);
    
    if (doc.isNull()) {
        setError(error);
        m_valid = false;
    } else {
        setDocument(doc);
        m_valid = true;
    }
    
    return *this;
}

JsonParser& JsonParser::fromByteArray(const QByteArray &data)
{
    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(data, &parseError);
    
    if (parseError.error != QJsonParseError::NoError) {
        setError(parseError.errorString());
        m_valid = false;
    } else {
        setDocument(doc);
        m_valid = true;
    }
    
    return *this;
}

JsonParser& JsonParser::fromVariant(const QVariant &variant)
{
    if (variant.canConvert<QVariantMap>()) {
        QJsonObject obj = QJsonObject::fromVariantMap(variant.toMap());
        setDocument(QJsonDocument(obj));
        m_valid = true;
    } else if (variant.canConvert<QVariantList>()) {
        QJsonArray arr = QJsonArray::fromVariantList(variant.toList());
        setDocument(QJsonDocument(arr));
        m_valid = true;
    } else {
        setError("Cannot convert variant to JSON");
        m_valid = false;
    }
    
    return *this;
}

bool JsonParser::isValid() const
{
    return m_valid && !m_document.isNull();
}

QString JsonParser::errorString() const
{
    return m_errorString;
}

bool JsonParser::validate()
{
    if (!isValid()) {
        return false;
    }
    
    JsonValidator validator;
    return validator.validate(m_document);
}

bool JsonParser::validateSchema(const QJsonObject &schema)
{
    if (!isValid()) {
        return false;
    }
    
    JsonValidator validator;
    return validator.validateSchema(m_document, schema);
}

QJsonDocument JsonParser::document() const
{
    return m_document;
}

QJsonObject JsonParser::object() const
{
    return m_document.object();
}

QJsonArray JsonParser::array() const
{
    return m_document.array();
}

QVariantMap JsonParser::toVariantMap() const
{
    if (m_document.isObject()) {
        return m_document.object().toVariantMap();
    }
    return QVariantMap();
}

QVariantList JsonParser::toVariantList() const
{
    if (m_document.isArray()) {
        return m_document.array().toVariantList();
    }
    return QVariantList();
}

QString JsonParser::toString(bool pretty) const
{
    return JsonParser::toString(m_document, pretty);
}

QJsonValue JsonParser::value(const QString &path, const QJsonValue &defaultValue) const
{
    if (!isValid()) {
        return defaultValue;
    }
    
    JsonQuery query(m_document);
    return query.value(path, defaultValue);
}

bool JsonParser::contains(const QString &path) const
{
    if (!isValid()) {
        return false;
    }
    
    JsonQuery query(m_document);
    return query.contains(path);
}

QStringList JsonParser::keys() const
{
    if (!isValid() || !m_document.isObject()) {
        return QStringList();
    }
    
    return m_document.object().keys();
}

JsonParser& JsonParser::setValue(const QString &path, const QJsonValue &value)
{
    if (!isValid()) {
        return *this;
    }
    
    if (m_document.isObject()) {
        QJsonObject obj = m_document.object();
        QStringList parts = path.split('.');
        
        if (parts.size() == 1) {
            obj[parts[0]] = value;
        } else {
            // Navigate to parent and set value
            QJsonObject *current = &obj;
            for (int i = 0; i < parts.size() - 1; ++i) {
                if (!current->contains(parts[i])) {
                    (*current)[parts[i]] = QJsonObject();
                }
                QJsonValue val = (*current)[parts[i]];
                if (val.isObject()) {
                    QJsonObject nested = val.toObject();
                    (*current)[parts[i]] = nested;
                    current = &nested;
                }
            }
            (*current)[parts.last()] = value;
        }
        
        setDocument(QJsonDocument(obj));
    }
    
    return *this;
}

JsonParser& JsonParser::remove(const QString &path)
{
    if (!isValid() || !m_document.isObject()) {
        return *this;
    }
    
    QJsonObject obj = m_document.object();
    QStringList parts = path.split('.');
    
    if (parts.size() == 1) {
        obj.remove(parts[0]);
        setDocument(QJsonDocument(obj));
    }
    
    return *this;
}

JsonParser& JsonParser::merge(const QJsonObject &other)
{
    if (!isValid() || !m_document.isObject()) {
        return *this;
    }
    
    QJsonObject obj = m_document.object();
    
    // Merge objects
    for (auto it = other.begin(); it != other.end(); ++it) {
        obj[it.key()] = it.value();
    }
    
    setDocument(QJsonDocument(obj));
    return *this;
}

bool JsonParser::saveToFile(const QString &filePath, bool pretty)
{
    if (!isValid()) {
        return false;
    }
    
    return save(m_document, filePath, pretty);
}

bool JsonParser::loadFromFile(const QString &filePath)
{
    fromFile(filePath);
    return isValid();
}

void JsonParser::setDocument(const QJsonDocument &doc)
{
    m_document = doc;
    m_valid = !doc.isNull();
    m_errorString.clear();
}

void JsonParser::setError(const QString &error)
{
    m_errorString = error;
    m_valid = false;
    m_document = QJsonDocument();
}
