# HTTP Client

This directory contains HTTP client wrapper utilities.

## Contents

- **GET Requests**: Perform HTTP GET requests
- **POST Requests**: Perform HTTP POST requests
- **PUT/PATCH Requests**: Update resource requests
- **DELETE Requests**: Delete resource requests
- **Request Headers**: Manage HTTP headers
- **Query Parameters**: Handle URL query parameters
- **Request Body**: Send JSON, form data, multipart data
- **Response Handling**: Parse JSON, text, binary responses
- **Error Handling**: Handle HTTP errors and timeouts
- **Retry Logic**: Automatic retry on failure
- **Interceptors**: Request/response interceptors
- **Authentication**: Bearer token, Basic auth, OAuth
- **Cookie Handling**: Manage cookies
- **Progress Tracking**: Upload/download progress
- **Proxy Support**: HTTP/HTTPS proxy configuration

## Features

- Promise-based API
- Automatic JSON parsing
- Timeout configuration
- Request cancellation
- Base URL configuration
- Default headers

## Usage Examples

```javascript
// Simple GET request
const data = await httpGet('https://api.example.com/users');

// POST with JSON body
const result = await httpPost('https://api.example.com/users', {
    name: 'John',
    email: 'john@example.com'
});

// With custom headers
const data = await httpGet('https://api.example.com/data', {
    headers: { 'Authorization': 'Bearer token123' }
});
```
