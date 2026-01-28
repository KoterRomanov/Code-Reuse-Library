# Factory Pattern

The Factory pattern provides an interface for creating objects without specifying their exact classes.

## Purpose

- Define an interface for creating objects
- Let subclasses decide which class to instantiate
- Defer instantiation to subclasses

## Use Cases

- When a class can't anticipate the type of objects it needs to create
- When you want to localize object creation logic
- When you want to provide a simple interface for creating complex objects
- Document readers (PDF, Word, Excel)
- UI components (Button, Dialog, Window)
- Database drivers

## Types

### Simple Factory
Basic factory that creates objects based on parameters

### Factory Method
Defines an interface for creating objects, letting subclasses decide which class to instantiate

### Abstract Factory
Provides an interface for creating families of related objects

## Structure

```
Product (interface)
├── ConcreteProductA
└── ConcreteProductB

Creator (abstract)
├── factoryMethod(): Product
└── ConcreteCreator
    └── factoryMethod(): ConcreteProduct
```

## Advantages

- Loose coupling between creator and concrete products
- Single Responsibility Principle
- Open/Closed Principle
- Easy to extend with new product types

## Disadvantages

- Code complexity increases
- Many subclasses needed

## Implementation Examples

This directory contains Factory pattern implementations in multiple languages demonstrating:
- Simple Factory
- Factory Method
- Abstract Factory
- Parameterized Factory
- Reflection-based Factory
