# Qt Networking Utilities

This directory contains Qt network programming utilities and patterns.

## Contents

### HTTP Client
- **HttpClient**: Wrapper for QNetworkAccessManager
- **RestClient**: REST API client
- **JsonClient**: JSON API client
- **DownloadManager**: File download manager
- **UploadManager**: File upload manager
- **RequestBuilder**: Fluent request builder

### WebSocket
- **WebSocketClient**: WebSocket client wrapper
- **WebSocketServer**: WebSocket server wrapper
- **MessageHandler**: WebSocket message handler
- **ReconnectManager**: Automatic reconnection
- **HeartbeatManager**: Connection keepalive

### TCP/UDP
- **TcpClient**: TCP client wrapper
- **TcpServer**: TCP server wrapper
- **UdpSocket**: UDP socket wrapper
- **SocketPool**: Connection pool for sockets
- **ProtocolHandler**: Custom protocol handler

### Network Utilities
- **NetworkMonitor**: Monitor network connectivity
- **ProxyManager**: Proxy configuration
- **SslHelper**: SSL/TLS utilities
- **CookieJar**: Cookie management
- **CacheManager**: Network cache management
- **RateLimiter**: Rate limiting for requests

## Features

- Asynchronous network operations
- SSL/TLS support
- Proxy support
- Cookie management
- Request/response caching
- Progress reporting
- Error handling
- Timeout management

## Usage Examples

### HTTP Client
```cpp
#include "HttpClient.h"

HttpClient client;

// GET request
client.get("https://api.example.com/users")
    .then([](const QJsonDocument &response) {
        qDebug() << "Users:" << response;
    })
    .error([](const QString &error) {
        qWarning() << "Error:" << error;
    });

// POST request with JSON
QJsonObject data;
data["name"] = "John";
data["email"] = "john@example.com";

client.post("https://api.example.com/users", data)
    .then([](const QJsonDocument &response) {
        qDebug() << "Created user:" << response;
    });
```

### WebSocket Client
```cpp
#include "WebSocketClient.h"

WebSocketClient ws("wss://example.com/socket");

connect(&ws, &WebSocketClient::connected, []() {
    qDebug() << "Connected to server";
});

connect(&ws, &WebSocketClient::messageReceived, [](const QString &message) {
    qDebug() << "Received:" << message;
});

ws.connect();
ws.sendMessage("Hello, Server!");
```

### Download Manager
```cpp
#include "DownloadManager.h"

DownloadManager manager;

QString url = "https://example.com/file.zip";
QString destination = "/path/to/save/file.zip";

manager.download(url, destination)
    .progress([](qint64 received, qint64 total) {
        int percent = (received * 100) / total;
        qDebug() << "Progress:" << percent << "%";
    })
    .finished([](const QString &filePath) {
        qDebug() << "Downloaded to:" << filePath;
    })
    .error([](const QString &error) {
        qWarning() << "Download error:" << error;
    });
```

### TCP Server
```cpp
#include "TcpServer.h"

TcpServer server;

connect(&server, &TcpServer::newConnection, [](QTcpSocket *socket) {
    qDebug() << "New client connected:" << socket->peerAddress();
    
    connect(socket, &QTcpSocket::readyRead, [socket]() {
        QByteArray data = socket->readAll();
        qDebug() << "Received:" << data;
        
        // Echo back
        socket->write(data);
    });
});

server.listen(QHostAddress::Any, 8080);
qDebug() << "Server listening on port 8080";
```

## Best Practices

- Always handle network errors
- Use QNetworkAccessManager singleton for efficiency
- Delete QNetworkReply objects to prevent memory leaks
- Implement timeout mechanisms
- Use SSL for sensitive data
- Validate SSL certificates in production
- Handle network state changes
- Implement retry logic for failed requests
- Use connection pooling for multiple requests
- Compress data when appropriate
