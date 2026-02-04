# CPP-Data-Structures

> A collection of fundamental data structures implemented in modern C++

## Overview

This repository focuses on clean, educational implementations of core data structures in C++. Each implementation emphasizes proper memory management, RAII principles, and modern C++ best practices.

## Contents

| Data Structure | File | Description |
|---------------|------|-------------|
| Singly Linked List | [design_linked_list.cpp](design_linked_list.cpp) | Linked list with dummy head node |

## Featured Implementation: Singly Linked List

A complete linked list implementation demonstrating essential C++ concepts:

### Features
- Dummy head node for simplified insertion/deletion logic
- O(1) head insertion, O(n) tail insertion
- Index-based access and insertion
- Full memory management with RAII
- Exception-safe copy construction and assignment
- `nothrow` memory allocation

### API Reference

```cpp
class link_list {
public:
    // Access
    int get_value(int idx);           // Get value at 1-based index

    // Modification
    bool add_at_head(int val);        // Insert at head
    bool add_at_tail(int val);        // Insert at tail
    bool add_at_index(int idx, int val); // Insert at index
    bool delete_at_index(int idx);    // Delete at index

    // Utility
    void print_all_elements();        // Print all values

    // Special member functions
    link_list();                      // Default constructor
    link_list(const link_list& l);    // Copy constructor
    link_list& operator=(const link_list& other); // Copy assignment
    ~link_list();                     // Destructor
};
```

### Usage Example

```cpp
link_list list;

// Add elements
for (int i = 1; i <= 10; ++i) {
    list.add_at_tail(i);
}

// Access elements
int value = list.get_value(5);  // Returns 5

// Delete element at index
list.delete_at_index(5);

// Copy semantics
link_list copy(list);           // Deep copy
link_list assigned = list;      // Copy assignment
```

### Design Decisions

| Aspect | Choice | Rationale |
|--------|--------|-----------|
| Head Node | Dummy node | Simplifies edge cases in insertion/deletion |
| Indexing | 1-based | Matches problem statement conventions |
| Memory | `nothrow` | Graceful handling of allocation failures |
| Ownership | RAII | Automatic cleanup in destructor |
| Copy | Deep copy | Ensures independent instances |

## Building

### Requirements
- C++11 or later
- GCC/Clang/MSVC compiler

### Compilation

```bash
# GCC/Clang
g++ -std=c++11 -o linked_list design_linked_list.cpp
./linked_list

# MSVC (Visual Studio)
cl /EHsc /std:c++14 design_linked_list.cpp
linked_list.exe
```

## Learning Objectives

This repository covers fundamental C++ concepts:

- [x] Pointer manipulation and memory management
- [x] Rule of Three (destructor, copy constructor, copy assignment)
- [x] Exception safety and strong exception guarantee
- [x] Resource Acquisition Is Initialization (RAII)
- [x] Class design and encapsulation

## Contributing

This is an educational project. Feel free to submit issues or pull requests for:
- Bug fixes
- Additional data structures
- Documentation improvements
- Performance optimizations

## License

This project is open source and available for educational purposes.
