# Query Builder

This directory contains SQL query builder utilities.

## Contents

- **SELECT Builder**: Build SELECT queries
- **INSERT Builder**: Build INSERT queries
- **UPDATE Builder**: Build UPDATE queries
- **DELETE Builder**: Build DELETE queries
- **JOIN Operations**: Handle various JOIN types
- **WHERE Clauses**: Build complex WHERE conditions
- **ORDER BY**: Handle sorting
- **GROUP BY**: Handle grouping and aggregation
- **HAVING Clauses**: Filter grouped results
- **LIMIT/OFFSET**: Handle pagination
- **Subqueries**: Build nested queries
- **Parameterized Queries**: Prevent SQL injection
- **Raw Queries**: Execute raw SQL when needed
- **Transaction Support**: Manage database transactions

## Features

- Fluent interface
- SQL injection prevention
- Cross-database support (MySQL, PostgreSQL, SQLite, etc.)
- Type-safe query building
- Query debugging and logging

## Usage Examples

```javascript
// Build SELECT query
const query = queryBuilder
    .select('users', ['id', 'name', 'email'])
    .where('age', '>', 18)
    .andWhere('status', '=', 'active')
    .orderBy('name', 'ASC')
    .limit(10)
    .build();

// Build INSERT query
const insert = queryBuilder
    .insert('users')
    .values({ name: 'John', email: 'john@example.com' })
    .build();

// Complex JOIN query
const join = queryBuilder
    .select('users', ['users.name', 'orders.total'])
    .join('orders', 'users.id', '=', 'orders.user_id')
    .where('orders.status', '=', 'completed')
    .build();
```
