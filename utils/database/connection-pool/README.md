# Connection Pool Management

This directory contains database connection pool utilities.

## Contents

- **Pool Creation**: Create and configure connection pools
- **Connection Acquisition**: Get connections from pool
- **Connection Release**: Return connections to pool
- **Pool Monitoring**: Monitor pool health and metrics
- **Connection Validation**: Test connection health
- **Connection Timeout**: Handle connection timeouts
- **Pool Resizing**: Dynamic pool size adjustment
- **Connection Lifecycle**: Manage connection lifecycle
- **Error Handling**: Handle connection errors
- **Load Balancing**: Distribute connections across nodes
- **Failover**: Handle database failover scenarios

## Features

- Configurable pool size (min/max)
- Connection reuse
- Automatic cleanup
- Connection health checks
- Timeout management
- Thread-safe operations

## Usage Examples

```python
# Create connection pool
pool = ConnectionPool(
    host='localhost',
    database='mydb',
    user='admin',
    password='secret',
    min_size=5,
    max_size=20
)

# Use connection
with pool.get_connection() as conn:
    cursor = conn.cursor()
    cursor.execute('SELECT * FROM users')
    results = cursor.fetchall()

# Pool automatically manages connections
```
