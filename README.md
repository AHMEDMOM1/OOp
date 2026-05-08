# 🚀 Object-Oriented Programming (OOP) Repository

Welcome to the OOP Mastery Repository! This repository serves as a comprehensive collection of Object-Oriented Programming principles applied in real-world C++ scenarios. 

## 🧩 Core OOP Concepts Demonstrated

This repository emphasizes the four fundamental pillars of OOP:

1. **Encapsulation**
   - Hiding internal state and requiring all interaction to be performed through an object's methods.
   - *Example*: Device states and capacities are strictly controlled via getter and setter methods.

2. **Abstraction**
   - Hiding complex implementation details and showing only the essential features of the object.
   - *Example*: A central `SmartHomeSystem` class that manages rooms without needing to know the complexities of memory allocation behind them.

3. **Inheritance**
   - Creating new classes derived from existing ones to promote code reusability.
   - *Example*: `SmartLight`, `SmartAirConditioner`, and `SmartLock` inheriting from a base `SmartDevice` class.

4. **Polymorphism**
   - Allowing objects of different types to be treated as objects of a common base type, typically achieved through virtual functions.
   - *Example*: Calling a virtual `exitMode()` or `getState()` method on a base `SmartDevice` pointer, which executes the specific implementation for the derived device type.

## 📁 Projects

- **[Smart Home System](./SmartHome/)**: A professional, console-based application to manage rooms and smart devices dynamically.
- **[Parking Management System](./Car/)**: A system designed to manage parking spots, records, and vehicles using robust data structures.

---
*Built with using C++ and standard OOP design patterns.*
