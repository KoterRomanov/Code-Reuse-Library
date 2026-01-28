# Singleton Pattern

The Singleton pattern ensures a class has only one instance and provides a global point of access to it.

## Purpose

- Ensure only one instance of a class exists
- Provide global access to that instance
- Lazy initialization of the instance

## Use Cases

- Database connections
- Configuration managers
- Logger instances
- Cache managers
- Thread pools
- Device drivers

## Structure

```
Singleton
├── private constructor
├── private static instance
└── public static getInstance()
```

## Advantages

- Controlled access to sole instance
- Reduced namespace pollution
- Permits refinement of operations
- Lazy initialization possible

## Disadvantages

- Difficult to unit test
- Violates Single Responsibility Principle
- Hidden dependencies
- Not suitable for multi-threaded environments without proper synchronization

## Implementation Examples

This directory contains Singleton pattern implementations in:
- Python (with thread-safety)
- JavaScript/TypeScript (module-based and class-based)
- Java (synchronized and double-checked locking)
- C++ (Meyer's Singleton)
- C# (lazy initialization)

## Thread-Safety Considerations

- Double-checked locking
- Eager initialization
- Bill Pugh Singleton (Java)
- Synchronized methods
