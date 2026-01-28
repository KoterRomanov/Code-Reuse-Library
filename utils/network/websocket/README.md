# WebSocket Utilities

This directory contains WebSocket communication utilities.

## Contents

- **WebSocket Client**: Connect to WebSocket servers
- **WebSocket Server**: Create WebSocket servers
- **Connection Management**: Handle connect/disconnect
- **Message Handling**: Send/receive messages
- **Binary Data**: Handle binary WebSocket messages
- **Ping/Pong**: Implement heartbeat mechanism
- **Reconnection**: Automatic reconnection on disconnect
- **Message Queue**: Queue messages during disconnection
- **Broadcasting**: Send messages to multiple clients
- **Room Management**: Organize connections into rooms
- **Authentication**: Secure WebSocket connections
- **Compression**: Message compression support
- **Protocol Support**: Handle WebSocket subprotocols

## Features

- Event-driven architecture
- Automatic reconnection
- Connection pooling
- Error handling
- SSL/TLS support
- Cross-platform compatibility

## Usage Examples

```javascript
// Create WebSocket client
const ws = new WebSocketClient('wss://example.com/socket');

// Connect and send message
ws.on('open', () => {
    ws.send({ type: 'hello', message: 'Hi' });
});

// Receive messages
ws.on('message', (data) => {
    console.log('Received:', data);
});

// Handle errors
ws.on('error', (error) => {
    console.error('WebSocket error:', error);
});
```
