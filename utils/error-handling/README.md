# Error Handling

This directory contains error handling and exception management utilities.

## Contents

- **Custom Exceptions**: Define custom exception classes
- **Error Codes**: Standardized error code system
- **Error Messages**: Internationalized error messages
- **Try-Catch Wrappers**: Simplified error handling
- **Error Recovery**: Automatic recovery strategies
- **Error Logging**: Log errors with context
- **Stack Trace**: Capture and format stack traces
- **Error Propagation**: Propagate errors across layers
- **Error Transformation**: Transform errors between layers
- **Retry Mechanism**: Retry on specific errors
- **Circuit Breaker**: Prevent cascading failures
- **Fallback Handling**: Provide fallback values
- **Error Reporting**: Send errors to monitoring services

## Features

- Typed exception hierarchies
- Error context preservation
- Async error handling
- Error aggregation
- User-friendly error messages

## Usage Examples

```javascript
// Define custom error
class ValidationError extends AppError {
    constructor(message, field) {
        super(message, 'VALIDATION_ERROR');
        this.field = field;
    }
}

// Use error wrapper
const result = await tryCatch(
    async () => await riskyOperation(),
    {
        onError: (error) => logger.error(error),
        fallback: defaultValue,
        retries: 3
    }
);

// Error recovery
try {
    await operation();
} catch (error) {
    if (isRetryable(error)) {
        await retry(operation, { maxRetries: 3 });
    } else {
        throw new FatalError(error);
    }
}
```
