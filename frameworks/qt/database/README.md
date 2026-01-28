# Qt Database Utilities

This directory contains Qt SQL module utilities and database patterns.

## Contents

### Connection Management
- **DatabaseManager**: Manage database connections
- **ConnectionPool**: Connection pooling
- **ConnectionFactory**: Create database connections
- **MultiDatabaseManager**: Handle multiple databases
- **DatabaseMigration**: Schema migration utilities

### Query Utilities
- **QueryBuilder**: Fluent query builder
- **PreparedStatement**: Parameterized query helper
- **QueryExecutor**: Execute queries safely
- **BatchQuery**: Batch query execution
- **TransactionManager**: Transaction handling

### ORM Helpers
- **ActiveRecord**: Active Record pattern
- **DataMapper**: Data Mapper pattern
- **EntityManager**: Entity management
- **ModelSync**: Sync models with database
- **LazyLoader**: Lazy load relationships

### Model Integration
- **SqlTableModel**: Enhanced QSqlTableModel
- **SqlQueryModel**: Enhanced QSqlQueryModel
- **SqlRelationalModel**: Enhanced QSqlRelationalTableModel
- **CachedSqlModel**: SQL model with caching
- **PaginatedSqlModel**: SQL model with pagination

### Database Tools
- **SchemaInspector**: Inspect database schema
- **DatabaseBackup**: Backup/restore utilities
- **DataImporter**: Import data from files
- **DataExporter**: Export data to files
- **DatabaseSeeder**: Seed test data

## Supported Databases

- SQLite
- MySQL/MariaDB
- PostgreSQL
- Oracle
- Microsoft SQL Server
- ODBC

## Usage Examples

### Database Manager
```cpp
#include "DatabaseManager.h"

DatabaseManager &dbManager = DatabaseManager::instance();

// Add database connection
dbManager.addConnection("mydb", {
    {"type", "QSQLITE"},
    {"database", "path/to/database.db"}
});

// Get database
QSqlDatabase db = dbManager.database("mydb");

if (db.isOpen()) {
    qDebug() << "Database connected successfully";
}
```

### Query Builder
```cpp
#include "QueryBuilder.h"

QueryBuilder query;

// SELECT query
QString sql = query.select("users")
    .columns({"id", "name", "email"})
    .where("age", ">", 18)
    .andWhere("status", "=", "active")
    .orderBy("name")
    .limit(10)
    .toSql();

QSqlQuery result = query.execute();

while (result.next()) {
    qDebug() << result.value("name").toString();
}
```

### Transaction Manager
```cpp
#include "TransactionManager.h"

TransactionManager transaction(db);

try {
    // Execute queries
    query1.exec("INSERT INTO users ...");
    query2.exec("UPDATE orders ...");
    
    // Commit transaction
    transaction.commit();
    qDebug() << "Transaction committed";
    
} catch (const std::exception &e) {
    // Rollback on error
    transaction.rollback();
    qWarning() << "Transaction rolled back:" << e.what();
}
```

### Active Record Pattern
```cpp
#include "ActiveRecord.h"

class User : public ActiveRecord {
public:
    User() : ActiveRecord("users") {}
    
    QString name() const { return value("name").toString(); }
    void setName(const QString &name) { setValue("name", name); }
    
    QString email() const { return value("email").toString(); }
    void setEmail(const QString &email) { setValue("email", email); }
};

// Create new user
User user;
user.setName("John Doe");
user.setEmail("john@example.com");
user.save();

// Find user
User user = User::find(1);
qDebug() << user.name();

// Update user
user.setName("Jane Doe");
user.save();

// Delete user
user.remove();
```

### SQL Model with View
```cpp
#include <QSqlTableModel>
#include <QTableView>

QSqlTableModel *model = new QSqlTableModel(this);
model->setTable("users");
model->setEditStrategy(QSqlTableModel::OnManualSubmit);
model->select();

// Set headers
model->setHeaderData(0, Qt::Horizontal, "ID");
model->setHeaderData(1, Qt::Horizontal, "Name");
model->setHeaderData(2, Qt::Horizontal, "Email");

QTableView *view = new QTableView(this);
view->setModel(model);
view->show();
```

## Best Practices

- Use prepared statements to prevent SQL injection
- Always handle database errors
- Use transactions for multiple related operations
- Close database connections when done
- Use connection pooling for better performance
- Index frequently queried columns
- Use appropriate data types
- Implement database migrations for schema changes
- Backup databases before migrations
- Use parameterized queries
- Handle NULL values properly
- Test database operations
