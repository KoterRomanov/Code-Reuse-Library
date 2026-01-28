# Qt Signals and Slots

This directory contains utilities and patterns for Qt's signal-slot mechanism.

## Contents

### Connection Helpers
- **AutoConnection**: RAII wrapper for signal-slot connections
- **ConnectionManager**: Manage multiple connections
- **ScopedConnection**: Connection with automatic disconnect
- **SignalBlocker**: Temporarily block signals

### Custom Signals
- **ProgressSignal**: Signal for reporting progress
- **ErrorSignal**: Signal for error reporting
- **DataReadySignal**: Signal for async data availability
- **ValidationSignal**: Signal for validation results

### Event Handling
- **EventFilter**: Reusable event filter implementations
- **GlobalEventFilter**: Application-wide event filtering
- **KeyboardHandler**: Keyboard shortcut handler
- **MouseHandler**: Mouse event handler
- **DragDropHandler**: Drag and drop event handler

### Threading
- **ThreadSafeSignal**: Thread-safe signal emission
- **WorkerThread**: Template for worker threads
- **SignalRelay**: Relay signals between threads
- **AsyncCaller**: Call functions asynchronously

### Patterns
- **SignalAggregator**: Combine multiple signals
- **SignalDebouncer**: Debounce rapid signal emissions
- **SignalThrottler**: Throttle signal emission rate
- **SignalMapper**: Map multiple signals to handlers

## Signal-Slot Connection Types

```cpp
// Auto connection (default)
connect(sender, &Sender::signal, receiver, &Receiver::slot);

// Direct connection (same thread)
connect(sender, &Sender::signal, receiver, &Receiver::slot, 
        Qt::DirectConnection);

// Queued connection (cross-thread)
connect(sender, &Sender::signal, receiver, &Receiver::slot, 
        Qt::QueuedConnection);

// Blocking queued connection
connect(sender, &Sender::signal, receiver, &Receiver::slot, 
        Qt::BlockingQueuedConnection);
```

## Usage Examples

### Connection Manager
```cpp
ConnectionManager manager;

manager.connect(button, &QPushButton::clicked, 
                this, &MyClass::onButtonClicked);
manager.connect(timer, &QTimer::timeout, 
                this, &MyClass::onTimeout);

// Disconnect all at once
manager.disconnectAll();
```

### Signal Debouncer
```cpp
SignalDebouncer debouncer(500); // 500ms delay

connect(lineEdit, &QLineEdit::textChanged, 
        &debouncer, &SignalDebouncer::trigger);
connect(&debouncer, &SignalDebouncer::triggered, 
        this, &MyClass::performSearch);
```

### Worker Thread
```cpp
WorkerThread<QString> worker([](const QString &input) {
    // Long-running operation
    return processData(input);
});

connect(&worker, &WorkerThread::finished, 
        this, &MyClass::onResultReady);
worker.start("input data");
```

## Best Practices

- Use new-style signal-slot syntax (compile-time checking)
- Be careful with object lifetime and connections
- Use QueuedConnection for cross-thread signals
- Consider using lambda functions for simple slots
- Disconnect signals when no longer needed
- Avoid connecting signals in loops
