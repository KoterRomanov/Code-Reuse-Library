# API Wrapper Tools

This directory contains tools for creating API wrappers and clients.

## Contents

- **REST API Wrapper**: Create REST API clients
- **GraphQL Client**: GraphQL query and mutation helpers
- **API Authentication**: OAuth, JWT, API key management
- **Rate Limiting**: Handle API rate limits
- **Pagination**: Handle paginated API responses
- **Caching**: Cache API responses
- **Request Builder**: Fluent API for building requests
- **Response Parser**: Parse and transform API responses
- **Error Mapping**: Map API errors to application errors
- **Mock Server**: Mock API responses for testing
- **API Documentation**: Generate API docs from code
- **SDK Generator**: Generate client SDKs from API specs

## Features

- Type-safe API clients
- Automatic retry and backoff
- Request/response transformation
- Comprehensive error handling
- OpenAPI/Swagger support

## Usage Examples

```typescript
// Create API wrapper
const api = new APIWrapper('https://api.example.com', {
    apiKey: 'your-api-key'
});

// Define endpoints
api.users.list = () => api.get('/users');
api.users.get = (id) => api.get(`/users/${id}`);
api.users.create = (data) => api.post('/users', data);

// Use the wrapper
const users = await api.users.list();
const user = await api.users.get(123);
```
