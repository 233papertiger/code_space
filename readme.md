# System Programming Learning Repository

> A collection of data structures and network server implementations for learning systems programming

## Overview

This repository contains educational implementations of fundamental data structures in C++ and high-performance network servers in C. Each project emphasizes clean code, proper resource management, and industry best practices.

## Project Structure

```
.
├── data_structure/          # C++ Data Structures
│   └── linked_list.cpp      # Singly Linked List implementation
├── server_development/      # Network Server Implementations
│   ├── select.c             # Select-based I/O multiplexing server
│   ├── epoll.c              # Epoll-based server (LT & ET modes)
│   └── reactor.c            # Reactor pattern server implementation
└── README.md
```

## Contents

### Data Structures ([data_structure/](data_structure/))

| Data Structure | File | Description |
|---------------|------|-------------|
| Singly Linked List | [linked_list.cpp](data_structure/linked_list.cpp) | Linked list with dummy head node |

### Network Servers ([server_development/](server_development/))

| Server Type | File | Description |
|-------------|------|-------------|
| Select Server | [select.c](server_development/select.c) | I/O multiplexing using `select()` |
| Epoll Server | [epoll.c](server_development/epoll.c) | I/O multiplexing using `epoll()` with LT/ET modes |
| Reactor Server | [reactor.c](server_development/reactor.c) | Reactor pattern with epoll and threading |

---

## Featured Implementations

### 1. Singly Linked List ([linked_list.cpp](data_structure/linked_list.cpp))

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

### 2. Select Server ([select.c](server_development/select.c))

A simple I/O multiplexing server using the traditional `select()` system call.

#### Features
- Supports up to 10 concurrent clients (`MAX_CLIENTS`)
- Uses `fd_set` for file descriptor monitoring
- Automatic client connection/disconnection handling
- Echo server: reads data and echoes it back

#### Build & Run
```bash
gcc -o select_server select.c
./select_server
```

#### Key Concepts
- **select()**: Blocks until an event occurs on any monitored file descriptor
- **FD_SET/FD_ISSET**: Macros for managing file descriptor sets
- **O(n) complexity**: Must scan all file descriptors on each iteration

### 3. Epoll Server ([epoll.c](server_development/epoll.c))

High-performance server using Linux's `epoll` I/O notification facility.

#### Features
- Supports both Level Triggered (LT) and Edge Triggered (ET) modes
- Non-blocking I/O for ET mode
- Handles up to 64 simultaneous events (`MAX_EVENTS`)
- Proper error handling with `EAGAIN`/`EWOULDBLOCK`

#### Build & Run
```bash
gcc -o epoll_server epoll.c
./epoll_server lt    # Level Triggered mode
./epoll_server et    # Edge Triggered mode
```

#### Key Concepts

| Mode | Description | Blocking Required |
|------|-------------|-------------------|
| **LT (Level Triggered)** | Default mode; notifies as long as condition exists | Optional |
| **ET (Edge Triggered)** | Notifies only on state changes; must read until `EAGAIN` | Required |

#### LT vs ET Mode

| Aspect | Level Triggered | Edge Triggered |
|--------|----------------|----------------|
| Notification | Repeatedly triggers while data available | Triggers only once per state change |
| Reading | Can read partially, system will re-notify | Must read until `EAGAIN` in a loop |
| Performance | Slightly lower due to more wakeups | Higher performance, fewer system calls |
| Complexity | Simpler to implement | Requires non-blocking I/O and careful loop design |

### 4. Reactor Server ([reactor.c](server_development/reactor.c))

Full implementation of the Reactor pattern using epoll and pthreads.

#### Features
- Event-driven architecture with callback registration
- Thread-safe event handler management with mutexes
- Separate reactor thread for event loop
- Dynamic handler registration/deregistration
- Clean shutdown with `reactor_stop()`

#### Build & Run
```bash
gcc -o reactor_server reactor.c -lpthread
./reactor_server
# Press 'q' + Enter to quit
```

#### Architecture

```
┌─────────────────────────────────────────────────────────┐
│                    Reactor Core                         │
│  ┌───────────────────────────────────────────────────┐  │
│  │            Event Loop Thread                      │  │
│  │   epoll_wait() ──► Dispatch Callbacks             │  │
│  └───────────────────────────────────────────────────┘  │
│                                                         │
│  Handler Registry (Thread-safe linked list)            │
│  ┌──────────┐  ┌──────────┐  ┌──────────┐             │
│  │ Server   │  │ Client 1 │  │ Client 2 │  ...        │
│  │ Handler  │  │ Handler  │  │ Handler  │             │
│  └──────────┘  └──────────┘  └──────────┘             │
└─────────────────────────────────────────────────────────┘
```

#### API Reference

```c
// Lifecycle management
reactor_t* reactor_create(void);
int reactor_start(reactor_t* reactor);
int reactor_stop(reactor_t* reactor);
void reactor_destroy(reactor_t* reactor);

// Event handler registration
int reactor_register(reactor_t* reactor, int fd, int events,
                     event_callback_t callback, void* arg);
int reactor_unregister(reactor_t* reactor, int fd);
```

#### Design Patterns

| Pattern | Implementation |
|---------|----------------|
| **Reactor** | Demultiplexing events to registered handlers |
| **Observer** | Callback functions for event notifications |
| **Thread-per-Loop** | Dedicated thread for event processing |

---

## Building

### Requirements
- **C++**: C++11 or later
- **C**: POSIX-compliant system (Linux/macOS)
- **C Compiler**: GCC/Clang
- **C++ Compiler**: GCC/Clang/MSVC

### Compilation

#### Data Structures (C++)
```bash
# GCC/Clang
g++ -std=c++11 -o linked_list data_structure/linked_list.cpp
./linked_list

# MSVC (Visual Studio)
cl /EHsc /std:c++14 data_structure\linked_list.cpp
linked_list.exe
```

#### Network Servers (C)
```bash
# Select server
gcc -o select_server server_development/select.c
./select_server

# Epoll server (Linux only)
gcc -o epoll_server server_development/epoll.c
./epoll_server lt    # Level Triggered
./epoll_server et    # Edge Triggered

# Reactor server (Linux only)
gcc -o reactor_server server_development/reactor.c -lpthread
./reactor_server
```

### Testing with Netcat

```bash
# Connect to any server
nc localhost 8080

# Send messages and observe echo response
```

## Learning Objectives

### C++ Data Structures
- [x] Pointer manipulation and memory management
- [x] Rule of Three (destructor, copy constructor, copy assignment)
- [x] Exception safety and strong exception guarantee
- [x] Resource Acquisition Is Initialization (RAII)
- [x] Class design and encapsulation

### Network Programming (C)
- [x] Socket API (socket, bind, listen, accept)
- [x] I/O Multiplexing: `select()` vs `epoll()`
- [x] Blocking vs Non-blocking I/O
- [x] Level Triggered vs Edge Triggered modes
- [x] Thread-safe programming with pthreads
- [x] Reactor pattern implementation
- [x] Callback-based event handling

### System Call Comparison

| Aspect | select() | poll() | epoll() |
|--------|----------|--------|---------|
| Max FDs | FD_SETSIZE (typically 1024) | Unlimited | Unlimited |
| Performance | O(n) per call | O(n) per call | O(1) per call |
| Scalability | Poor for high connections | Moderate | Excellent |
| Portability | POSIX | POSIX | Linux-only |

---

## Contributing

This is an educational project. Feel free to submit issues or pull requests for:
- Bug fixes
- Additional data structures
- Documentation improvements
- Performance optimizations

## License

This project is open source and available for educational purposes.
