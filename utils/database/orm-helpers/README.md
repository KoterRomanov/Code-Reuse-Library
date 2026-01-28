# ORM Helper Tools

This directory contains Object-Relational Mapping (ORM) helper utilities.

## Contents

- **Model Definition**: Define database models
- **Schema Migration**: Database schema migration tools
- **CRUD Operations**: Create, Read, Update, Delete helpers
- **Relationship Mapping**: Handle one-to-one, one-to-many, many-to-many
- **Lazy Loading**: Load related data on demand
- **Eager Loading**: Preload related data
- **Query Optimization**: Optimize database queries
- **Validation**: Model data validation
- **Hooks/Callbacks**: Before/after save hooks
- **Soft Delete**: Mark records as deleted without removing
- **Timestamps**: Automatic created_at/updated_at
- **Serialization**: Convert models to JSON/dict
- **Bulk Operations**: Batch insert/update/delete

## Features

- Active Record pattern support
- Data Mapper pattern support
- Database abstraction
- Transaction management
- Connection management
- Query caching

## Usage Examples

```python
# Define model
class User(Model):
    table = 'users'
    fields = ['id', 'name', 'email', 'created_at']
    
    def posts(self):
        return self.hasMany(Post, 'user_id')

# CRUD operations
user = User.create(name='John', email='john@example.com')
user = User.find(1)
user.update(name='John Doe')
user.delete()

# Query with relationships
users = User.query().with_('posts').where('status', 'active').get()
```
