# Qt JSON Parser

A comprehensive Qt-based JSON parser utility that provides easy-to-use interfaces for parsing, validating, and manipulating JSON data from various sources (strings, files, network responses).

[![Qt Version](https://img.shields.io/badge/Qt-6.0%2B-green.svg)](https://www.qt.io/)
[![C++ Standard](https://img.shields.io/badge/C%2B%2B-17-blue.svg)](https://en.cppreference.com/w/cpp/17)
[![License](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)

## ✨ Features

- � **Simple API**: Intuitive interface for common JSON operations
- 🛡️ **Robust Validation**: Built-in validation with JSON Schema support
- 🔍 **Powerful Queries**: JSONPath-like queries with dot notation and wildcards
- 🔗 **Method Chaining**: Fluent API design for readable code
- 📁 **File Operations**: Easy file reading/writing with encoding support
- ⚡ **Performance**: Efficient parsing and memory management
- 🔧 **Qt Integration**: Seamless integration with Qt types (QString, QVariant, etc.)

## 📋 Table of Contents

- [Features](#-features)
- [Quick Start](#-quick-start)
- [Building](#-building)
- [Usage](#-usage)
- [API Documentation](#-api-documentation)
- [Examples](#-examples)
- [Testing](#-testing)
- [Design Goals](#-design-goals)
- [Architecture](#-architecture)

## 🚀 Quick Start

### The Simple Way (JsonHelper - Recommended)

```cpp
#include "jsonhelper.h"

// Parse and query in one line
JsonHelper json(R"({"name": "John", "age": 30})");

// Type-safe getters with defaults
QString name = json.getString("name");
int age = json.getInt("age");

// Simple setters with chaining
json.setString("email", "john@example.com")
    .setInt("age", 31)
    .save("user.json");

// Easy validation
json.requireField("name")
    .addValidationRule("age", [](const QJsonValue &v) {
        return v.toInt() >= 18;
    }, "Must be 18+");

if (json.validate()) {
    qDebug() << "Valid!";
}
```

**See [JSONHELPER_EXAMPLES.md](JSONHELPER_EXAMPLES.md) for comprehensive examples.**

### The Advanced Way (JsonParser)

For fine-grained control, use the lower-level classes directly:

```cpp
#include "jsonparser.h"

// Parse JSON
JsonParser parser;
parser.fromString(R"({"name": "John", "age": 30})");

// Query values
QString name = parser.value("name").toString();
int age = parser.value("age").toInt();

// Modify and save
parser.setValue("age", QJsonValue(31));
parser.saveToFile("output.json");
```

## 🏗️ Building

### Using qmake

```bash
# Build library
qmake jsonparser.pro
make

# Install
sudo make install
```

### Using CMake

```bash
# Configure
mkdir build && cd build
cmake .. -DBUILD_TESTS=ON

# Build
cmake --build .

# Install
sudo cmake --install .
```

### Adding to Your Project

#### qmake (.pro file)

```qmake
# Link against the library
LIBS += -lqtjsonparser

# Add include path
INCLUDEPATH += /usr/local/include/qtjsonparser

# Or if you include the sources directly
include(path/to/json-parser/jsonparser.pri)
```

#### CMake (CMakeLists.txt)

```cmake
# Find the package
find_package(QtJsonParser REQUIRED)

# Link against your target
target_link_libraries(your_target PRIVATE QtJsonParser::qtjsonparser)
```

## 📖 Usage

### The Simple Way: JsonHelper (Recommended for Most Users)

JsonHelper wraps all functionality into one easy-to-use class:

```cpp
#include "jsonhelper.h"

// Load and query
JsonHelper json = JsonHelper::fromFile("config.json");
QString theme = json.getString("theme", "light");
int fontSize = json.getInt("fontSize", 12);

// Modify with type safety
json.setString("theme", "dark")
    .setInt("fontSize", 14)
    .setBool("autoSave", true);

// Validate with custom rules
json.requireFields({"username", "email"})
    .addValidationRule("email", [](const QJsonValue &v) {
        return v.toString().contains("@");
    }, "Invalid email");

if (json.validate()) {
    json.save("config.json");
}
```

**Benefits**:
- 🎯 Simple, intuitive API
- 🛡️ Type-safe getters/setters
- 🔗 Method chaining
- ✅ Built-in validation
- 📦 All features in one class

See [JSONHELPER_EXAMPLES.md](JSONHELPER_EXAMPLES.md) for comprehensive examples and real-world use cases.

### The Advanced Way: Individual Classes

For fine-grained control and advanced features:

#### Basic Parsing (JsonParser)

```cpp
#include "jsonparser.h"

QString jsonStr = R"({
    "name": "Alice",
    "email": "alice@example.com"
})";

// Parse JSON
JsonParser parser;
parser.fromString(jsonStr);

if (parser.isValid()) {
    qDebug() << parser.value("name").toString();
}
```

#### File Operations

```cpp
// Load from file
JsonParser parser;
if (parser.loadFromFile("config.json")) {
    // Process data
}

// Save to file
parser.setValue("lastModified", QJsonValue(QDateTime::currentDateTime().toString()));
parser.saveToFile("config.json", true);  // true = pretty print
```

#### Validation (JsonValidator)

```cpp
#include "jsonvalidator.h"

// Schema validation
QJsonObject schema;
schema["type"] = "object";
// ... define schema

JsonValidator validator;
if (validator.validateSchema(parser.document(), schema)) {
    qDebug() << "Valid!";
}

// Custom validation rules
validator.addRule("age", [](const QJsonValue &v) { 
    return v.toInt() >= 18; 
}, "Must be 18+");
```

#### Querying (JsonQuery)

```cpp
#include "jsonquery.h"

JsonQuery query(parser.document());

// Nested queries
QString city = query.value("user.address.city").toString();

// Array access
QString firstName = query.value("users[0].name").toString();

// Wildcard queries
QList<QJsonValue> allNames = query.values("users[*].name");
```

**When to use individual classes**:
- Need maximum performance
- Require advanced customization
- Building complex query logic
- Want separation of concerns

## 📚 API Documentation

See the detailed API documentation in the sections below.

## 🧪 Testing

```bash
# Build and run tests
cd tests
qmake jsonparser_test.pro && make && ./bin/jsonparser_tests
qmake jsonvalidator_test.pro && make && ./bin/jsonvalidator_tests
qmake jsonquery_test.pro && make && ./bin/jsonquery_tests
```

See [tests/README.md](tests/README.md) for more information.

## 🎯 Design Goals

1. **Simplicity**: Intuitive API for common JSON operations
2. **Safety**: Robust error handling and validation
3. **Performance**: Efficient parsing and memory management
4. **Flexibility**: Support multiple input/output formats
5. **Qt Integration**: Seamless integration with Qt types (QString, QVariant, etc.)

## 🏗️ Architecture

### Core Components

```
JsonParser (Main Class)
├── JsonReader (Reading & Parsing)
├── JsonWriter (Writing & Serialization)
├── JsonValidator (Validation & Schema)
└── JsonQuery (Query & Path Operations)
```

## 📦 Class Design

### 1. JsonParser (Main Interface)

**Purpose**: Unified interface for all JSON operations

**Key Features**:
- Static methods for quick operations
- Instance methods for complex workflows
- Chainable API design
- Thread-safe operations

**Public Interface**:
```cpp
class JsonParser {
public:
    // Static convenience methods
    static QJsonDocument parse(const QString &jsonString, QString *error = nullptr);
    static QJsonDocument parseFile(const QString &filePath, QString *error = nullptr);
    static bool save(const QJsonDocument &doc, const QString &filePath, bool pretty = true);
    static QString toString(const QJsonDocument &doc, bool pretty = true);
    
    // Instance methods for chaining
    JsonParser& fromString(const QString &jsonString);
    JsonParser& fromFile(const QString &filePath);
    JsonParser& fromByteArray(const QByteArray &data);
    JsonParser& fromVariant(const QVariant &variant);
    
    // Validation
    bool isValid() const;
    QString errorString() const;
    bool validate();
    bool validateSchema(const QJsonObject &schema);
    
    // Conversion
    QJsonDocument document() const;
    QJsonObject object() const;
    QJsonArray array() const;
    QVariantMap toVariantMap() const;
    QVariantList toVariantList() const;
    QString toString(bool pretty = true) const;
    
    // Query operations
    QJsonValue value(const QString &path, const QJsonValue &defaultValue = QJsonValue()) const;
    bool contains(const QString &path) const;
    QStringList keys() const;
    
    // Modification
    JsonParser& setValue(const QString &path, const QJsonValue &value);
    JsonParser& remove(const QString &path);
    JsonParser& merge(const QJsonObject &other);
    
    // File operations
    bool saveToFile(const QString &filePath, bool pretty = true);
    bool loadFromFile(const QString &filePath);
    
private:
    QJsonDocument m_document;
    QString m_errorString;
    bool m_valid;
};
```

### 2. JsonReader

**Purpose**: Handle reading and parsing from various sources

**Responsibilities**:
- Parse JSON strings with error reporting
- Read and parse JSON files
- Handle encoding detection (UTF-8, UTF-16, etc.)
- Stream parsing for large files
- Network response parsing

**Public Interface**:
```cpp
class JsonReader {
public:
    JsonReader();
    
    // Parse methods
    QJsonDocument parse(const QString &jsonString, JsonError *error = nullptr);
    QJsonDocument parseFile(const QString &filePath, JsonError *error = nullptr);
    QJsonDocument parseData(const QByteArray &data, JsonError *error = nullptr);
    
    // Stream parsing for large files
    bool parseStream(QIODevice *device, JsonStreamCallback callback);
    
    // Configuration
    void setEncoding(QStringConverter::Encoding encoding);
    void setMaxDepth(int depth);
    void setStrictMode(bool strict);
    
    // Error handling
    JsonError lastError() const;
    bool hasError() const;
    QString errorString() const;
    
private:
    QStringConverter::Encoding m_encoding;
    int m_maxDepth;
    bool m_strictMode;
    JsonError m_lastError;
};
```

### 3. JsonWriter

**Purpose**: Handle writing and serialization

**Responsibilities**:
- Convert JSON to string (compact/pretty)
- Write to files with encoding
- Custom formatting options
- Stream writing for large data

**Public Interface**:
```cpp
class JsonWriter {
public:
    JsonWriter();
    
    // Write methods
    QString toString(const QJsonDocument &doc, FormatOption format = Indented);
    QByteArray toByteArray(const QJsonDocument &doc, FormatOption format = Indented);
    bool toFile(const QJsonDocument &doc, const QString &filePath, FormatOption format = Indented);
    
    // Stream writing
    bool writeStream(const QJsonDocument &doc, QIODevice *device, FormatOption format = Indented);
    
    // Configuration
    void setIndentSize(int size);
    void setEncoding(QStringConverter::Encoding encoding);
    void setSortKeys(bool sort);
    void setEscapeUnicode(bool escape);
    
    // Format options
    enum FormatOption {
        Compact,        // Single line, no spaces
        Indented,       // Pretty print with indentation
        Custom          // Use custom settings
    };
    
private:
    int m_indentSize;
    QStringConverter::Encoding m_encoding;
    bool m_sortKeys;
    bool m_escapeUnicode;
};
```

### 4. JsonValidator

**Purpose**: Validate JSON structure and content

**Responsibilities**:
- Basic structure validation
- Type checking
- Required field validation
- JSON Schema validation (subset)
- Custom validation rules

**Public Interface**:
```cpp
class JsonValidator {
public:
    JsonValidator();
    
    // Basic validation
    bool validate(const QJsonDocument &doc);
    bool validate(const QJsonObject &obj);
    bool validate(const QJsonArray &arr);
    
    // Schema validation
    bool validateSchema(const QJsonDocument &doc, const QJsonObject &schema);
    bool loadSchema(const QString &schemaFile);
    
    // Custom rules
    void addRule(const QString &path, ValidationRule rule);
    void clearRules();
    
    // Type checking
    bool isType(const QJsonValue &value, QJsonValue::Type expectedType);
    bool hasRequiredFields(const QJsonObject &obj, const QStringList &fields);
    
    // Error reporting
    QList<ValidationError> errors() const;
    QString errorString() const;
    bool hasErrors() const;
    
private:
    QJsonObject m_schema;
    QList<ValidationRule> m_customRules;
    QList<ValidationError> m_errors;
};

// Validation structures
struct ValidationRule {
    QString path;
    std::function<bool(const QJsonValue&)> validator;
    QString errorMessage;
};

struct ValidationError {
    QString path;
    QString message;
    ErrorType type;
    
    enum ErrorType {
        TypeMismatch,
        MissingField,
        InvalidValue,
        SchemaViolation,
        CustomRule
    };
};
```

### 5. JsonQuery

**Purpose**: Query and navigate JSON structures

**Responsibilities**:
- JSONPath-like query syntax
- Dot notation path access
- Array indexing
- Wildcard support
- Filter operations

**Public Interface**:
```cpp
class JsonQuery {
public:
    JsonQuery(const QJsonDocument &doc);
    JsonQuery(const QJsonObject &obj);
    JsonQuery(const QJsonArray &arr);
    
    // Path queries (dot notation)
    QJsonValue value(const QString &path, const QJsonValue &defaultValue = QJsonValue()) const;
    bool contains(const QString &path) const;
    QList<QJsonValue> values(const QString &path) const;
    
    // Examples:
    // "user.name" -> obj["user"]["name"]
    // "users[0].email" -> arr[0]["email"]
    // "users[*].name" -> all names in users array
    
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
    
private:
    QJsonValue m_root;
    
    QJsonValue navigate(const QString &path) const;
    QStringList parsePath(const QString &path) const;
};
```

### 6. JsonError

**Purpose**: Comprehensive error information

**Structure**:
```cpp
struct JsonError {
    ErrorType type;
    QString message;
    int line;
    int column;
    int offset;
    
    enum ErrorType {
        NoError,
        ParseError,
        FileError,
        EncodingError,
        ValidationError,
        PathError,
        UnknownError
    };
    
    bool isError() const { return type != NoError; }
    QString toString() const;
};
```

## 🔧 Usage Patterns

### Pattern 1: Quick Parse and Access

```cpp
// Parse JSON string
auto doc = JsonParser::parse(jsonString);
if (doc.isNull()) {
    // Handle error
}

// Access values
JsonQuery query(doc);
QString name = query.value("user.name").toString();
int age = query.value("user.age", 0).toInt();
```

### Pattern 2: File Operations

```cpp
// Load from file
JsonParser parser;
if (parser.loadFromFile("config.json")) {
    // Use data
    QString value = parser.value("settings.theme").toString();
    
    // Modify
    parser.setValue("settings.theme", "dark");
    
    // Save back
    parser.saveToFile("config.json");
}
```

### Pattern 3: Validation

```cpp
// Validate structure
JsonParser parser;
parser.fromString(jsonString);

if (parser.validate()) {
    // Additional validation
    JsonValidator validator;
    validator.addRule("user.email", [](const QJsonValue &v) {
        return v.toString().contains("@");
    });
    
    if (validator.validate(parser.document())) {
        // Process valid data
    }
}
```

### Pattern 4: Building JSON

```cpp
// Create JSON programmatically
QJsonObject root;
root["name"] = "John Doe";
root["age"] = 30;

QJsonArray hobbies;
hobbies.append("reading");
hobbies.append("coding");
root["hobbies"] = hobbies;

// Save
JsonParser::save(QJsonDocument(root), "user.json", true);
```

### Pattern 5: Stream Processing

```cpp
// Process large JSON file
JsonReader reader;
QFile file("large_data.json");

reader.parseStream(&file, [](const QJsonValue &value) {
    // Process each element
    processData(value);
    return true; // Continue
});
```

## 📊 Data Flow

```
Input Sources          Parser           Operations        Output
─────────────        ─────────        ─────────────     ──────────
                                                        
String ──────┐                       ┌─ Query          ├─ String
             │                       │                 │
File ────────┤──→ JsonReader ──→ QJsonDocument ──→ Validate ──→ File
             │                       │                 │
ByteArray ───┤                       └─ Modify         ├─ QVariant
             │                                         │
Network ─────┘                                         └─ Object/Array
```

## 🛡️ Error Handling Strategy

### 1. Defensive Programming
- Null checks on all inputs
- Bounds checking for array access
- File existence verification
- Encoding validation

### 2. Error Reporting
- Detailed error messages
- Line/column information for parse errors
- Error accumulation for validation
- Optional error output parameters

### 3. Exception Safety
- No exceptions thrown (Qt style)
- Return boolean or null for errors
- Error string available via getter
- State preserved on error

## 🔒 Thread Safety

### Design Decisions
- **JsonParser instance**: Not thread-safe (by design)
- **Static methods**: Thread-safe
- **Shared documents**: Use with Qt's implicit sharing
- **Recommendation**: One parser per thread or use locks

## 🎨 Design Patterns Used

1. **Facade Pattern**: JsonParser provides unified interface
2. **Builder Pattern**: Chainable API for configuration
3. **Strategy Pattern**: Different validation strategies
4. **Factory Pattern**: Create parsers for different sources
5. **Visitor Pattern**: Tree traversal for queries

## 📈 Performance Considerations

### Optimization Strategies
1. **Lazy Evaluation**: Parse only when needed
2. **Caching**: Cache frequently accessed paths
3. **Stream Processing**: For large files
4. **Move Semantics**: Use std::move for large objects
5. **Implicit Sharing**: Leverage Qt's COW

### Memory Management
- Use Qt's parent-child ownership
- RAII for file handles
- Automatic cleanup in destructors
- Minimal copying (use references)

## 🧪 Testing Strategy

### Unit Tests
- Parse valid/invalid JSON
- File I/O operations
- Path query correctness
- Validation rules
- Error handling

### Integration Tests
- Real-world JSON files
- Large file handling
- Unicode handling
- Network data parsing

### Edge Cases
- Empty JSON
- Deep nesting
- Large arrays
- Special characters
- Malformed JSON
- File permission errors

## 📝 Example Use Cases

### Use Case 1: Configuration File
```cpp
// Load application config
JsonParser config;
config.loadFromFile("app_config.json");

QString theme = config.value("ui.theme", "light").toString();
int fontSize = config.value("ui.fontSize", 12).toInt();
bool autoSave = config.value("features.autoSave", true).toBool();
```

### Use Case 2: API Response
```cpp
// Parse API response
auto doc = JsonParser::parse(networkReply->readAll());
JsonQuery query(doc);

QStringList userNames = query.values("users[*].name")
    .transform([](const QJsonValue &v) { return v.toString(); });
```

### Use Case 3: Data Validation
```cpp
// Validate user input
JsonValidator validator;
validator.loadSchema("user_schema.json");

if (validator.validateSchema(userDoc, schema)) {
    saveUser(userDoc);
} else {
    showErrors(validator.errors());
}
```

### Use Case 4: JSON Transformation
```cpp
// Transform JSON data
JsonQuery query(inputDoc);
auto transformed = query.map([](const QJsonValue &v) {
    QJsonObject obj = v.toObject();
    obj["processed"] = true;
    return obj;
});
```

## 🔄 Version History

- **v1.0** (Planned): Core functionality
  - Basic parsing and writing
  - File operations
  - Simple validation
  - Path queries

- **v1.1** (Future): Enhanced features
  - JSON Schema validation
  - Stream processing
  - Advanced queries
  - Performance optimizations

- **v2.0** (Future): Advanced features
  - JSONPath full support
  - Custom serialization
  - Async operations
  - Plugin system

## 📚 Dependencies

### Required
- Qt Core (QJsonDocument, QJsonObject, QJsonArray)
- Qt Core (QFile, QIODevice)
- Qt Core (QString, QVariant)

### Optional
- Qt Network (for network response parsing)
- Qt Test (for unit testing)

## 🔗 References

- [Qt JSON Support](https://doc.qt.io/qt-6/json.html)
- [JSON Specification (RFC 8259)](https://tools.ietf.org/html/rfc8259)
- [JSON Schema](https://json-schema.org/)
- [JSONPath Specification](https://goessner.net/articles/JsonPath/)

## 📋 Implementation Checklist

### Phase 1: Core Functionality
- [ ] JsonParser class skeleton
- [ ] Basic parse/toString methods
- [ ] File read/write operations
- [ ] Error handling structure
- [ ] Basic unit tests

### Phase 2: Advanced Features
- [ ] JsonReader implementation
- [ ] JsonWriter with formatting
- [ ] JsonValidator basics
- [ ] JsonQuery path parsing
- [ ] Comprehensive tests

### Phase 3: Polish
- [ ] Documentation
- [ ] Examples
- [ ] Performance testing
- [ ] API refinement
- [ ] Code review

### Phase 4: Optional Features
- [ ] JSON Schema validation
- [ ] Stream processing
- [ ] Advanced JSONPath
- [ ] Benchmarking

## 💡 Future Enhancements

1. **BSON Support**: Binary JSON format
2. **JSON5 Support**: JSON with comments and trailing commas
3. **YAML Interop**: Convert between JSON and YAML
4. **Diff/Patch**: JSON diff and patch operations
5. **Compression**: Automatic compression for large files
6. **Encryption**: Built-in encryption support
7. **Remote Loading**: Fetch JSON from URLs
8. **Watch Mode**: Auto-reload on file changes

## 📄 License

Part of Code Reuse Library - See repository license for details.
