# Logging Utilities

This directory contains logging and debugging utilities.

## Contents

- **Logger Configuration**: Set up logging system
- **Log Levels**: Debug, Info, Warning, Error, Critical
- **File Logging**: Write logs to files
- **Console Logging**: Output logs to console
- **Rotating Logs**: Rotate log files by size/time
- **Structured Logging**: JSON/structured log format
- **Context Logging**: Add contextual information
- **Log Filtering**: Filter logs by level/pattern
- **Remote Logging**: Send logs to remote servers
- **Performance Logging**: Log execution time
- **Error Tracking**: Track and report errors
- **Log Aggregation**: Centralize logs from multiple sources
- **Async Logging**: Non-blocking log operations

## Features

- Multiple output handlers
- Custom log formatters
- Log correlation IDs
- Stack trace capture
- Sensitive data masking
- Log compression

## Usage Examples

```python
# Configure logger
logger = Logger(
    name='app',
    level=LogLevel.INFO,
    handlers=[
        FileHandler('app.log', rotate='daily'),
        ConsoleHandler()
    ]
)

# Log messages
logger.debug('Debug message')
logger.info('Application started')
logger.warning('Warning message')
logger.error('Error occurred', exc_info=True)

# Structured logging
logger.info('User logged in', extra={
    'user_id': 123,
    'ip': '192.168.1.1'
})
```
