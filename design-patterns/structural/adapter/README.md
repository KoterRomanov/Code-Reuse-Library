# Adapter Pattern

The Adapter pattern converts the interface of a class into another interface clients expect, allowing incompatible interfaces to work together.

## Purpose

- Convert interface of a class into another interface
- Allow classes with incompatible interfaces to work together
- Wrap an existing class with a new interface

## Use Cases

- Integrating legacy code with new systems
- Using third-party libraries with incompatible interfaces
- Creating reusable classes that cooperate with unforeseen classes
- Media players (playing different file formats)
- Payment gateways (unified interface for different providers)
- Database adapters

## Types

### Class Adapter
Uses inheritance to adapt one interface to another

### Object Adapter
Uses composition to adapt one interface to another

## Structure

```
Target (interface)
Client → uses → Target

Adapter implements Target
└── adaptee: Adaptee
    └── delegates to Adaptee methods

Adaptee (existing class with incompatible interface)
```

## Advantages

- Single Responsibility Principle
- Open/Closed Principle
- Increases class reusability
- Improves flexibility

## Disadvantages

- Increases overall complexity
- Sometimes many adaptations required

## Implementation Examples

This directory contains Adapter pattern implementations showing:
- Class Adapter (using inheritance)
- Object Adapter (using composition)
- Two-way Adapter
- Pluggable Adapter
