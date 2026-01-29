/**
 * @file jsonhelper.cpp
 * @brief Implementation of JsonHelper class
 */

#include "jsonhelper.h"
#include "jsonparser.h"
#include "jsonquery.h"
#include "jsonvalidator.h"
#include <QFile>

/**
 * @brief Private implementation class (pimpl idiom)
 */
class JsonHelper::JsonHelperPrivate {
public:
    JsonParser parser;
    JsonValidator validator;
    QStringList requiredFields;
    
    JsonHelperPrivate() {}
    
    JsonQuery createQuery() const {
        return JsonQuery(parser.document());
    }
};

// ============= Construction / Destruction =============

JsonHelper::JsonHelper() : d(new JsonHelperPrivate()) {
}

JsonHelper::JsonHelper(const QString &jsonString) : d(new JsonHelperPrivate()) {
    parse(jsonString);
}

JsonHelper::~JsonHelper() {
    delete d;
}

// ============= Loading / Parsing =============

bool JsonHelper::parse(const QString &jsonString) {
    d->parser.fromString(jsonString);
    return d->parser.isValid();
}

bool JsonHelper::load(const QString &filePath) {
    return d->parser.loadFromFile(filePath);
}

bool JsonHelper::loadFromBytes(const QByteArray &data) {
    d->parser.fromByteArray(data);
    return d->parser.isValid();
}

bool JsonHelper::loadFromVariant(const QVariant &variant) {
    d->parser.fromVariant(variant);
    return d->parser.isValid();
}

// ============= Saving / Serialization =============

bool JsonHelper::save(const QString &filePath, bool prettyPrint) {
    return d->parser.saveToFile(filePath, prettyPrint);
}

QString JsonHelper::toString(bool prettyPrint) const {
    return d->parser.toString(prettyPrint);
}

QByteArray JsonHelper::toBytes(bool prettyPrint) const {
    return toString(prettyPrint).toUtf8();
}

// ============= Querying =============

QJsonValue JsonHelper::get(const QString &path, const QJsonValue &defaultValue) const {
    return d->parser.value(path, defaultValue);
}

QString JsonHelper::getString(const QString &path, const QString &defaultValue) const {
    QJsonValue val = get(path);
    return val.isString() ? val.toString() : defaultValue;
}

int JsonHelper::getInt(const QString &path, int defaultValue) const {
    QJsonValue val = get(path);
    return val.isDouble() ? val.toInt() : defaultValue;
}

double JsonHelper::getDouble(const QString &path, double defaultValue) const {
    QJsonValue val = get(path);
    return val.isDouble() ? val.toDouble() : defaultValue;
}

bool JsonHelper::getBool(const QString &path, bool defaultValue) const {
    QJsonValue val = get(path);
    return val.isBool() ? val.toBool() : defaultValue;
}

QJsonObject JsonHelper::getObject(const QString &path) const {
    if (path.isEmpty()) {
        return d->parser.object();
    }
    QJsonValue val = get(path);
    return val.isObject() ? val.toObject() : QJsonObject();
}

QJsonArray JsonHelper::getArray(const QString &path) const {
    if (path.isEmpty()) {
        return d->parser.array();
    }
    QJsonValue val = get(path);
    return val.isArray() ? val.toArray() : QJsonArray();
}

QList<QJsonValue> JsonHelper::getAll(const QString &path) const {
    JsonQuery query = d->createQuery();
    return query.values(path);
}

bool JsonHelper::has(const QString &path) const {
    return d->parser.contains(path);
}

QStringList JsonHelper::keys(const QString &path) const {
    if (path.isEmpty()) {
        return d->parser.keys();
    }
    
    QJsonValue val = get(path);
    if (val.isObject()) {
        return val.toObject().keys();
    }
    return QStringList();
}

// ============= Modifying =============

JsonHelper& JsonHelper::set(const QString &path, const QJsonValue &value) {
    d->parser.setValue(path, value);
    return *this;
}

JsonHelper& JsonHelper::setString(const QString &path, const QString &value) {
    return set(path, QJsonValue(value));
}

JsonHelper& JsonHelper::setInt(const QString &path, int value) {
    return set(path, QJsonValue(value));
}

JsonHelper& JsonHelper::setDouble(const QString &path, double value) {
    return set(path, QJsonValue(value));
}

JsonHelper& JsonHelper::setBool(const QString &path, bool value) {
    return set(path, QJsonValue(value));
}

JsonHelper& JsonHelper::remove(const QString &path) {
    d->parser.remove(path);
    return *this;
}

JsonHelper& JsonHelper::merge(const QJsonObject &other, const QString &path) {
    if (path.isEmpty()) {
        d->parser.merge(other);
    } else {
        QJsonValue val = get(path);
        if (val.isObject()) {
            QJsonObject obj = val.toObject();
            // Merge into the object
            for (auto it = other.begin(); it != other.end(); ++it) {
                obj[it.key()] = it.value();
            }
            set(path, obj);
        }
    }
    return *this;
}

JsonHelper& JsonHelper::clear() {
    d->parser = JsonParser();
    d->validator.clearRules();
    d->validator.clearErrors();
    d->requiredFields.clear();
    return *this;
}

// ============= Validation =============

bool JsonHelper::isValid() const {
    return d->parser.isValid();
}

bool JsonHelper::validate() {
    d->validator.clearErrors();
    
    // Check required fields
    if (!d->requiredFields.isEmpty()) {
        QJsonObject obj = d->parser.object();
        for (const QString &field : d->requiredFields) {
            if (!has(field)) {
                return false;
            }
        }
    }
    
    bool result = d->validator.validate(d->parser.document());
    return result && d->parser.validate();
}

bool JsonHelper::validateWithSchema(const QJsonObject &schemaJson) {
    d->validator.clearErrors();
    return d->validator.validateSchema(d->parser.document(), schemaJson);
}

bool JsonHelper::validateWithSchemaFile(const QString &schemaFilePath) {
    // Load schema file
    QFile file(schemaFilePath);
    if (!file.open(QIODevice::ReadOnly)) {
        return false;
    }
    
    QByteArray data = file.readAll();
    file.close();
    
    QJsonParseError error;
    QJsonDocument schemaDoc = QJsonDocument::fromJson(data, &error);
    
    if (error.error != QJsonParseError::NoError || !schemaDoc.isObject()) {
        return false;
    }
    
    return validateWithSchema(schemaDoc.object());
}

JsonHelper& JsonHelper::addValidationRule(const QString &path,
                                           std::function<bool(const QJsonValue&)> validator,
                                           const QString &errorMessage) {
    d->validator.addRule(path, validator, errorMessage);
    return *this;
}

JsonHelper& JsonHelper::requireField(const QString &path) {
    if (!d->requiredFields.contains(path)) {
        d->requiredFields.append(path);
    }
    return *this;
}

JsonHelper& JsonHelper::requireFields(const QStringList &paths) {
    for (const QString &path : paths) {
        requireField(path);
    }
    return *this;
}

JsonHelper& JsonHelper::clearValidationRules() {
    d->validator.clearRules();
    d->validator.clearErrors();
    d->requiredFields.clear();
    return *this;
}

// ============= Searching / Filtering =============

QJsonArray JsonHelper::find(const QString &arrayPath, const QString &key, const QJsonValue &value) const {
    JsonQuery query = d->createQuery();
    QJsonValue arrayVal = get(arrayPath);
    
    if (!arrayVal.isArray()) {
        return QJsonArray();
    }
    
    JsonQuery arrayQuery(arrayVal.toArray());
    return arrayQuery.find(key, value);
}

QJsonArray JsonHelper::filter(const QString &arrayPath, std::function<bool(const QJsonValue&)> predicate) const {
    QJsonValue arrayVal = get(arrayPath);
    
    if (!arrayVal.isArray()) {
        return QJsonArray();
    }
    
    JsonQuery arrayQuery(arrayVal.toArray());
    return arrayQuery.filter(predicate);
}

QJsonArray JsonHelper::map(const QString &arrayPath, std::function<QJsonValue(const QJsonValue&)> mapper) const {
    QJsonValue arrayVal = get(arrayPath);
    
    if (!arrayVal.isArray()) {
        return QJsonArray();
    }
    
    JsonQuery arrayQuery(arrayVal.toArray());
    return arrayQuery.map(mapper);
}

// ============= Conversion =============

QVariantMap JsonHelper::toVariantMap() const {
    return d->parser.toVariantMap();
}

QVariantList JsonHelper::toVariantList() const {
    return d->parser.toVariantList();
}

QJsonDocument JsonHelper::document() const {
    return d->parser.document();
}

// ============= Error Handling =============

QString JsonHelper::lastError() const {
    if (!d->parser.isValid()) {
        return d->parser.errorString();
    }
    if (d->validator.hasErrors()) {
        return d->validator.errorString();
    }
    return QString();
}

bool JsonHelper::hasError() const {
    return !d->parser.isValid() || d->validator.hasErrors();
}

QStringList JsonHelper::validationErrors() const {
    QStringList errors;
    
    // Check required fields
    for (const QString &field : d->requiredFields) {
        if (!has(field)) {
            errors.append(QString("Required field missing: %1").arg(field));
        }
    }
    
    // Get validator errors
    auto validatorErrors = d->validator.errors();
    for (const auto &error : validatorErrors) {
        errors.append(error.toString());
    }
    
    return errors;
}

// ============= Utility Methods =============

bool JsonHelper::isEmpty() const {
    QJsonDocument doc = d->parser.document();
    if (doc.isObject()) {
        return doc.object().isEmpty();
    } else if (doc.isArray()) {
        return doc.array().isEmpty();
    }
    return true;
}

bool JsonHelper::isObject() const {
    return d->parser.document().isObject();
}

bool JsonHelper::isArray() const {
    return d->parser.document().isArray();
}

int JsonHelper::size(const QString &path) const {
    if (path.isEmpty()) {
        QJsonDocument doc = d->parser.document();
        if (doc.isObject()) {
            return doc.object().size();
        } else if (doc.isArray()) {
            return doc.array().size();
        }
        return 0;
    }
    
    QJsonValue val = get(path);
    if (val.isObject()) {
        return val.toObject().size();
    } else if (val.isArray()) {
        return val.toArray().size();
    }
    return 0;
}

// ============= Static Utility Methods =============

JsonHelper JsonHelper::fromString(const QString &jsonString) {
    JsonHelper helper;
    helper.parse(jsonString);
    return helper;
}

JsonHelper JsonHelper::fromFile(const QString &filePath) {
    JsonHelper helper;
    helper.load(filePath);
    return helper;
}

JsonHelper JsonHelper::fromVariant(const QVariant &variant) {
    JsonHelper helper;
    helper.loadFromVariant(variant);
    return helper;
}

JsonHelper JsonHelper::createObject() {
    JsonHelper helper;
    helper.d->parser.fromVariant(QVariantMap());
    return helper;
}

JsonHelper JsonHelper::createArray() {
    JsonHelper helper;
    helper.d->parser.fromVariant(QVariantList());
    return helper;
}
