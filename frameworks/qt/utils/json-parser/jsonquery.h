/**
 * @file jsonquery.h
 * @brief JSON query and navigation utilities
 */

#ifndef JSONQUERY_H
#define JSONQUERY_H

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QString>
#include <QStringList>
#include <QList>
#include <functional>

/**
 * @brief JSON query class for navigating and querying JSON structures
 * 
 * Provides convenient methods for accessing nested JSON data using
 * dot notation paths, array indexing, and filter operations.
 */
class JsonQuery {
public:
    // Constructors
    explicit JsonQuery(const QJsonDocument &doc);
    explicit JsonQuery(const QJsonObject &obj);
    explicit JsonQuery(const QJsonArray &arr);
    explicit JsonQuery(const QJsonValue &value);
    
    // Path queries (dot notation)
    QJsonValue value(const QString &path, const QJsonValue &defaultValue = QJsonValue()) const;
    bool contains(const QString &path) const;
    QList<QJsonValue> values(const QString &path) const;
    
    // Array operations
    QJsonValue at(int index, const QJsonValue &defaultValue = QJsonValue()) const;
    int size() const;
    bool isEmpty() const;
    
    // Object operations
    QStringList keys() const;
    bool hasKey(const QString &key) const;
    
    // Filter operations
    QJsonArray filter(std::function<bool(const QJsonValue&)> predicate) const;
    QJsonArray find(const QString &key, const QJsonValue &value) const;
    
    // Transformation
    QJsonArray map(std::function<QJsonValue(const QJsonValue&)> mapper) const;
    QJsonValue reduce(std::function<QJsonValue(const QJsonValue&, const QJsonValue&)> reducer,
                     const QJsonValue &initial = QJsonValue()) const;
    
    // Get root value
    QJsonValue root() const { return m_root; }
    
private:
    QJsonValue m_root;
    
    QJsonValue navigate(const QString &path) const;
    QList<QJsonValue> navigateWildcard(const QString &path) const;
    QStringList parsePath(const QString &path) const;
    bool isWildcardPath(const QString &path) const;
};

#endif // JSONQUERY_H
