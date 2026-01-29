/**
 * @file jsonquery.cpp
 * @brief JSON query and navigation utilities implementation
 */

#include "jsonquery.h"
#include <QRegularExpression>

JsonQuery::JsonQuery(const QJsonDocument &doc)
{
    if (doc.isObject()) {
        m_root = doc.object();
    } else if (doc.isArray()) {
        m_root = doc.array();
    }
}

JsonQuery::JsonQuery(const QJsonObject &obj)
    : m_root(obj)
{
}

JsonQuery::JsonQuery(const QJsonArray &arr)
    : m_root(arr)
{
}

JsonQuery::JsonQuery(const QJsonValue &value)
    : m_root(value)
{
}

QJsonValue JsonQuery::value(const QString &path, const QJsonValue &defaultValue) const
{
    if (path.isEmpty()) {
        return m_root;
    }
    
    QJsonValue result = navigate(path);
    return result.isUndefined() ? defaultValue : result;
}

bool JsonQuery::contains(const QString &path) const
{
    return !navigate(path).isUndefined();
}

QList<QJsonValue> JsonQuery::values(const QString &path) const
{
    if (isWildcardPath(path)) {
        return navigateWildcard(path);
    }
    
    QJsonValue val = navigate(path);
    if (val.isUndefined()) {
        return QList<QJsonValue>();
    }
    
    return QList<QJsonValue>() << val;
}

QJsonValue JsonQuery::at(int index, const QJsonValue &defaultValue) const
{
    if (!m_root.isArray()) {
        return defaultValue;
    }
    
    QJsonArray arr = m_root.toArray();
    if (index < 0 || index >= arr.size()) {
        return defaultValue;
    }
    
    return arr.at(index);
}

int JsonQuery::size() const
{
    if (m_root.isArray()) {
        return m_root.toArray().size();
    } else if (m_root.isObject()) {
        return m_root.toObject().size();
    }
    return 0;
}

bool JsonQuery::isEmpty() const
{
    if (m_root.isArray()) {
        return m_root.toArray().isEmpty();
    } else if (m_root.isObject()) {
        return m_root.toObject().isEmpty();
    }
    return true;
}

QStringList JsonQuery::keys() const
{
    if (!m_root.isObject()) {
        return QStringList();
    }
    
    return m_root.toObject().keys();
}

bool JsonQuery::hasKey(const QString &key) const
{
    if (!m_root.isObject()) {
        return false;
    }
    
    return m_root.toObject().contains(key);
}

QJsonArray JsonQuery::filter(std::function<bool(const QJsonValue&)> predicate) const
{
    QJsonArray result;
    
    if (!m_root.isArray()) {
        return result;
    }
    
    QJsonArray arr = m_root.toArray();
    for (const QJsonValue &value : arr) {
        if (predicate(value)) {
            result.append(value);
        }
    }
    
    return result;
}

QJsonArray JsonQuery::find(const QString &key, const QJsonValue &value) const
{
    return filter([key, value](const QJsonValue &v) {
        if (v.isObject()) {
            QJsonObject obj = v.toObject();
            return obj.contains(key) && obj[key] == value;
        }
        return false;
    });
}

QJsonArray JsonQuery::map(std::function<QJsonValue(const QJsonValue&)> mapper) const
{
    QJsonArray result;
    
    if (!m_root.isArray()) {
        return result;
    }
    
    QJsonArray arr = m_root.toArray();
    for (const QJsonValue &value : arr) {
        result.append(mapper(value));
    }
    
    return result;
}

QJsonValue JsonQuery::reduce(std::function<QJsonValue(const QJsonValue&, const QJsonValue&)> reducer,
                             const QJsonValue &initial) const
{
    if (!m_root.isArray()) {
        return initial;
    }
    
    QJsonArray arr = m_root.toArray();
    QJsonValue accumulator = initial;
    
    for (const QJsonValue &value : arr) {
        accumulator = reducer(accumulator, value);
    }
    
    return accumulator;
}

QJsonValue JsonQuery::navigate(const QString &path) const
{
    QStringList parts = parsePath(path);
    QJsonValue current = m_root;
    
    for (const QString &part : parts) {
        // Check for array index: users[0]
        static QRegularExpression arrayRegex(R"(^(\w+)\[(\d+)\]$)");
        QRegularExpressionMatch match = arrayRegex.match(part);
        
        if (match.hasMatch()) {
            // Handle array access
            QString key = match.captured(1);
            int index = match.captured(2).toInt();
            
            if (current.isObject()) {
                current = current.toObject().value(key);
            }
            
            if (current.isArray()) {
                QJsonArray arr = current.toArray();
                if (index >= 0 && index < arr.size()) {
                    current = arr.at(index);
                } else {
                    return QJsonValue(QJsonValue::Undefined);
                }
            } else {
                return QJsonValue(QJsonValue::Undefined);
            }
        } else {
            // Regular object key access
            if (current.isObject()) {
                current = current.toObject().value(part);
            } else {
                return QJsonValue(QJsonValue::Undefined);
            }
        }
        
        if (current.isUndefined() || current.isNull()) {
            return QJsonValue(QJsonValue::Undefined);
        }
    }
    
    return current;
}

QList<QJsonValue> JsonQuery::navigateWildcard(const QString &path) const
{
    QList<QJsonValue> results;
    
    // Simple wildcard support: "users[*].name"
    QStringList parts = parsePath(path);
    QList<QJsonValue> currentLevel;
    currentLevel.append(m_root);
    
    for (const QString &part : parts) {
        QList<QJsonValue> nextLevel;
        
        // Check for wildcard array access: [*]
        if (part.contains("[*]")) {
            QString key = part.left(part.indexOf("[*]"));
            
            for (const QJsonValue &value : currentLevel) {
                if (value.isObject() && !key.isEmpty()) {
                    QJsonValue arrayValue = value.toObject().value(key);
                    if (arrayValue.isArray()) {
                        QJsonArray arr = arrayValue.toArray();
                        for (const QJsonValue &item : arr) {
                            nextLevel.append(item);
                        }
                    }
                } else if (value.isArray() && key.isEmpty()) {
                    QJsonArray arr = value.toArray();
                    for (const QJsonValue &item : arr) {
                        nextLevel.append(item);
                    }
                }
            }
        } else {
            // Regular navigation
            for (const QJsonValue &value : currentLevel) {
                JsonQuery query(value);
                QJsonValue nav = query.navigate(part);
                if (!nav.isUndefined()) {
                    nextLevel.append(nav);
                }
            }
        }
        
        currentLevel = nextLevel;
    }
    
    return currentLevel;
}

QStringList JsonQuery::parsePath(const QString &path) const
{
    if (path.isEmpty()) {
        return QStringList();
    }
    
    // Split by dot, but preserve array notation
    QStringList parts;
    QString current;
    
    for (int i = 0; i < path.length(); ++i) {
        QChar c = path[i];
        
        if (c == '.') {
            if (!current.isEmpty()) {
                parts.append(current);
                current.clear();
            }
        } else {
            current += c;
        }
    }
    
    if (!current.isEmpty()) {
        parts.append(current);
    }
    
    return parts;
}

bool JsonQuery::isWildcardPath(const QString &path) const
{
    return path.contains("[*]");
}
