# C/C++ 开发岗面试突击指南 - 医疗行业专项

> 准备时间：3天 | 目标岗位：xxx医疗 C/C++ 开发 | 经验要求：2年

---

## 📋 目录

1. [C/C++ 语言基础](#1-cc-语言基础)
2. [面向对象与设计模式](#2-面向对象与设计模式)
3. [内存管理与资源管理](#3-内存管理与资源管理)
4. [并发与多线程编程](#4-并发与多线程编程)
5. [数据结构与算法](#5-数据结构与算法)
6. [系统编程与网络编程](#6-系统编程与网络编程)
7. [医疗设备软件开发特点](#7-医疗设备软件开发特点)
8. [行为面试与项目经验](#8-行为面试与项目经验)

---

## 1. C/C++ 语言基础

### 1.1 C与C++的核心区别

**问题：C和C++的主要区别是什么？**

**标准答案：**

| 特性 | C | C++ |
|------|---|-----|
| **编程范式** | 面向过程 | 多范式（面向过程、面向对象、泛型） |
| **封装性** | struct 仅数据聚合 | class 支持数据 + 函数 + 访问控制 |
| **内存管理** | malloc/free | new/delete + 析构函数 + RAII |
| **类型安全** | 较弱，隐式转换 | 强类型，static_cast 等显式转换 |
| **错误处理** | 返回值 + errno | 异常机制 (try-catch) |
| **代码复用** | 函数 + 宏 | 模板 + 继承 + 组合 |

**关键点补充：**
- C++ 的 RAII (Resource Acquisition Is Initialization) 是资源管理的核心思想，C 语言需要手动管理资源生命周期
- C++ 的模板提供编译期多态，C 的宏是文本替换，不安全
- C++ 标准库提供 STL 容器、算法、智能指针等，C 需要自己实现

### 1.2 指针与引用

**问题：指针和引用的区别？什么时候使用引用？**

**标准答案：**

```cpp
void func1(int* p);      // 指针参数
void func2(int& r);      // 引用参数
```

| 特性 | 指针 | 引用 |
|------|------|------|
| **本质** | 存储地址的变量 | 对象的别名 |
| **初始化** | 可不初始化 | 必须初始化 |
| **重新绑定** | 可以改变指向 | 不可改变指向 |
| **空值** | 可以为 nullptr | 不存在空引用 |
| ** sizeof** | 指针大小（8字节64位） | 被引用对象的大小 |
| **间接访问** | 需要解引用 (*) | 直接使用 |

**使用场景：**
- **使用引用**：函数参数、函数返回值、运算符重载
- **使用指针**：需要重新指向、需要表示"无值"、实现数据结构（链表节点）

**面试加分项：**
```cpp
// const 引用避免拷贝，提高性能
void process(const std::string& str);  // 推荐

// 指针表示"可选"参数
void setConfig(Config* cfg = nullptr); // cfg 可能为空
```

### 1.3 const 关键字的深层理解

**问题：const 在 C++ 中的各种用法？**

**标准答案：**

```cpp
// 1. 常量：不可修改的变量
const int MAX_SIZE = 100;

// 2. 指针与 const 的组合
int x = 10;
const int* p1 = &x;   // 指向常量的指针，*p1 不可改
int* const p2 = &x;   // 常量指针，p2 不可改
const int* const p3 = &x; // 都不可改

// 3. 函数参数：防止修改
void func(const std::string& str);

// 4. 成员函数：不修改对象状态
class MyClass {
    int getValue() const;  // const 成员函数
};

// 5. 函数返回值：防止修改返回的对象
const std::string& getName();

// 6. const 与 constexpr 的区别
const int c = 10;        // 运行期常量
constexpr int ce = 10;   // 编译期常量，可用于数组大小
int arr[ce];             // 合法
// int arr[c];           // 非标准扩展
```

**记忆口诀：** 从右往左读
- `const int* p` → p is a pointer to int that is const
- `int* const p` → p is a const pointer to int

### 1.4 sizeof 与 strlen 的区别

```cpp
char str[] = "hello";

sizeof(str);   // 6 (包括 '\0')，编译期计算
strlen(str);   // 5 (不包括 '\0')，运行期计算

char* p = str;
sizeof(p);     // 指针大小（8字节），不是字符串长度！
```

---

## 2. 面向对象与设计模式

### 2.1 三大核心概念详解

**问题：封装、继承、多态的作用和实现方式？**

#### 封装 (Encapsulation)

**目的：** 隐藏实现细节，提供接口

```cpp
class BankAccount {
private:
    double balance;  // 私有数据，外部不可直接访问

public:
    void deposit(double amount) {
        if (amount > 0)  // 数据验证
            balance += amount;
    }

    double getBalance() const {
        return balance;  // 只读访问
    }
};
```

**好处：**
- 数据保护：防止外部随意修改
- 实现细节可变：内部实现可改变而不影响使用者
- 降低耦合：模块间依赖减少

#### 继承 (Inheritance)

**目的：** 代码复用和层次化建模

```cpp
class Animal {
protected:
    std::string name;
public:
    void eat() { std::cout << "eating...\n"; }
    virtual void makeSound() = 0;  // 纯虚函数
};

class Dog : public Animal {  // public 继承
public:
    void makeSound() override {  // override 关键字 C++11
        std::cout << "Woof!\n";
    }
    void fetch() { std::cout << "fetching ball\n"; }
};
```

**继承类型：**
- `public`：基类 public 成员在派生类中仍是 public
- `protected`：基类 public 成员在派生类中变成 protected
- `private`：基类所有成员在派生类中都变成 private

#### 多态 (Polymorphism)

**实现方式：**
1. **编译期多态**：函数重载、运算符重载、模板
2. **运行期多态**：虚函数 + 继承

```cpp
// 运行期多态示例
void interact(Animal* animal) {
    animal->makeSound();  // 根据 animal 实际类型调用
    animal->eat();        // 非虚函数，根据指针类型调用
}

Animal* a = new Dog();
interact(a);  // 输出 "Woof!" 然后是 "eating..."

delete a;     // 警告：如果 Animal 的析构函数不是虚函数，
              // 会导致内存泄漏！
```

**重要规则：**
- 多态基类应该声明 **虚析构函数**
- `final` 关键字阻止继承或重写
- `override` 确保正确重写虚函数

### 2.2 虚函数与虚函数表

**问题：虚函数的底层实现原理？虚函数表的结构？**

**标准答案：**

**内存布局：**
```
对象内存：
+------------------+
| vptr (虚表指针)   |  <- 每个对象都有，指向类的虚函数表
+------------------+
| 成员变量 data     |
+------------------+

虚函数表 (vtable) - 每个类一份：
+------------------+
| &Base::func1     |  虚函数地址数组
| &Base::func2     |
+------------------+
```

**示例代码：**
```cpp
class Base {
public:
    virtual void f1() { cout << "Base::f1\n"; }
    virtual void f2() { cout << "Base::f2\n"; }
    int x;
};

class Derived : public Base {
public:
    void f1() override { cout << "Derived::f1\n"; }
    int y;
};

// 调用过程：
Base* b = new Derived();
b->f1();  // 1. 通过 vptr 找到 vtable
          // 2. 从 vtable 第一个槽位获取函数地址
          // 3. 调用 Derived::f1()
```

**性能影响：**
- 虚函数调用比普通函数多一次间接访问（查表）
- 现代 CPU 的分支预测可优化
- 性能损失约 10-20%，但在大多数场景可接受

### 2.3 构造函数与析构函数

**问题：构造函数的初始化列表有什么用？析构函数什么时候调用？**

**初始化列表 vs 赋值：**

```cpp
class MyClass {
    const int ci;       // const 成员必须初始化
    std::string& ref;   // 引用成员必须初始化
    std::string str;
public:
    // 正确：初始化列表
    MyClass(int i, std::string& r)
        : ci(i), ref(r), str("hello") {}  // 直接初始化

    // 错误：const 和引用不能用赋值
    // MyClass(int i, std::string& r) {
    //     ci = i;      // 编译错误！
    //     ref = r;     // 编译错误！
    //     str = "hello";  // 先默认构造，再赋值（低效）
    // }
};
```

**初始化顺序：**
1. 按照成员**声明顺序**初始化（不是初始化列表的顺序！）
2. 父类先于子类
3. 成员先于构造函数体

**析构函数调用顺序：**
1. 构造函数体执行完成 → 栈展开开始
2. 派生类析构函数
3. 成员变量析构函数（按声明逆序）
4. 基类析构函数

### 2.4 拷贝控制：拷贝构造与赋值运算符

**问题：为什么要同时实现拷贝构造和赋值运算符？**

**标准答案：**

**Rule of Three（三法则）：**
如果需要自定义以下任何一个，则需要全部实现：
1. 拷贝构造函数
2. 拷贝赋值运算符
3. 析构函数

```cpp
class SmartArray {
private:
    int* data;
    size_t size;

public:
    // 构造函数
    SmartArray(size_t n) : size(n), data(new int[n]) {}

    // 析构函数
    ~SmartArray() {
        delete[] data;  // 释放资源
    }

    // 拷贝构造函数（深拷贝）
    SmartArray(const SmartArray& other) : size(other.size) {
        data = new int[size];
        std::copy(other.data, other.data + size, data);
    }

    // 拷贝赋值运算符（深拷贝）
    SmartArray& operator=(const SmartArray& other) {
        if (this == &other)  // 自我赋值检查
            return *this;

        // 释放旧资源
        delete[] data;

        // 分配新资源并拷贝
        size = other.size;
        data = new int[size];
        std::copy(other.data, other.data + size, data);

        return *this;
    }
};
```

**现代 C++ (C++11) 的 Rule of Five：**
额外增加：
4. 移动构造函数
5. 移动赋值运算符

```cpp
// 移动构造函数（窃取资源）
SmartArray(SmartArray&& other) noexcept
    : data(other.data), size(other.size) {
    other.data = nullptr;  // 置空，避免析构时释放
    other.size = 0;
}

// 移动赋值运算符
SmartArray& operator=(SmartArray&& other) noexcept {
    if (this != &other) {
        delete[] data;      // 释放自己的资源

        data = other.data;  // 窃取
        size = other.size;
        other.data = nullptr;
        other.size = 0;
    }
    return *this;
}
```

### 2.5 常用设计模式

#### 单例模式 (Singleton)

**问题：如何实现线程安全的单例？**

```cpp
// C++11 之后，推荐使用 Magic Static
class Singleton {
public:
    // 删除拷贝和移动
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;

    static Singleton& getInstance() {
        static Singleton instance;  // 线程安全，C++11 保证
        return instance;
    }

private:
    Singleton() = default;  // 私有构造
    ~Singleton() = default;
};
```

#### 工厂模式 (Factory)

```cpp
// 抽象产品
interface Shape {
    virtual void draw() = 0;
    virtual ~Shape() = default;
};

// 具体产品
class Circle : public Shape {
public:
    void draw() override { cout << "Drawing Circle\n"; }
};

class Rectangle : public Shape {
public:
    void draw() override { cout << "Drawing Rectangle\n"; }
};

// 简单工厂
enum class ShapeType { CIRCLE, RECTANGLE };

class ShapeFactory {
public:
    static std::unique_ptr<Shape> createShape(ShapeType type) {
        switch (type) {
            case ShapeType::CIRCLE:
                return std::make_unique<Circle>();
            case ShapeType::RECTANGLE:
                return std::make_unique<Rectangle>();
            default:
                return nullptr;
        }
    }
};

// 使用
auto shape = ShapeFactory::createShape(ShapeType::CIRCLE);
shape->draw();
```

---

## 3. 内存管理与资源管理

### 3.1 内存分区详解

**问题：C/C++ 程序的内存布局？栈和堆的区别？**

**标准答案：**

```
高地址
    +-------------------+
    |     内核空间       |
    +-------------------+
    |     栈 Stack       |  ↓ 自动增长，存放局部变量、函数参数
    |         ↓          |
    |                    |
    |        ↑           |
    |     堆 Heap        |  ↑ 手动分配，动态内存
    +-------------------+
    |   全局/静态区      |  全局变量、static 变量
    +-------------------+
    |   只读常量区       |  字符串常量
    +-------------------+
    |   代码区 .text     |  二进制指令
低地址 +-------------------+
```

| 特性 | 栈 (Stack) | 堆 (Heap) |
|------|-----------|-----------|
| **分配方式** | 自动（编译器） | 手动（new/malloc） |
| **分配速度** | 快（只需移动指针） | 慢（需查找空闲块） |
| **空间大小** | 小（通常 1-8MB） | 大（受物理内存限制） |
| **生命周期** | 作用域结束自动释放 | 需手动释放 |
| **碎片问题** | 无 | 有（内碎片、外碎片） |
| **线程安全** | 每线程独立 | 需要同步（多线程共享） |

### 3.2 new/malloc 与 delete/free 的区别

```cpp
// C 风格
int* p1 = (int*)malloc(sizeof(int) * 10);
free(p1);

// C++ 风格
int* p2 = new int[10];
delete[] p2;  // 注意数组要用 delete[]
```

| 特性 | malloc/free | new/delete |
|------|-------------|------------|
| **语言** | C 库函数 | C++ 运算符 |
| **类型安全** | 需要强制转换，不安全 | 类型安全，自动计算大小 |
| **初始化** | 不初始化（内容随机） | 可调用构造函数 |
| **失败处理** | 返回 nullptr | 抛出 std::bad_alloc |
| **重载** | 不可重载 | 可重载（自定义内存管理） |
| **数组** | 需手动计算大小 | new[], delete[] 配对 |

### 3.3 智能指针（现代 C++ 必备）

**问题：智能指针的类型和使用场景？**

**标准答案：**

```cpp
#include <memory>

// 1. unique_ptr：独占所有权，不可拷贝，可移动
std::unique_ptr<MyClass> p1 = std::make_unique<MyClass>();
// std::unique_ptr<MyClass> p2 = p1;  // 编译错误！
std::unique_ptr<MyClass> p3 = std::move(p1);  // OK，p1 变空

// 2. shared_ptr：共享所有权，引用计数
std::shared_ptr<MyClass> sp1 = std::make_shared<MyClass>();
std::shared_ptr<MyClass> sp2 = sp1;  // 引用计数 +1
// sp1, sp2 都释放时才真正删除

// 3. weak_ptr：解决循环引用，不增加引用计数
std::weak_ptr<MyClass> wp = sp1;
if (auto locked = wp.lock()) {  // 尝试获取 shared_ptr
    locked->doSomething();
}
```

**使用原则：**
- 默认使用 `unique_ptr`
- 需要共享所有权时用 `shared_ptr`
- `shared_ptr` 可能循环引用时用 `weak_ptr` 打破

**性能对比：**
- `unique_ptr`：零开销，与裸指针相同
- `shared_ptr`：额外维护引用计数，有原子操作开销

### 3.4 内存泄漏检测与预防

**常见泄漏场景：**

```cpp
// 1. 忘记释放
void func() {
    int* p = new int(100);
    // 忘记 delete p
}

// 2. 异常导致跳过释放
void func() {
    int* p = new int(100);
    someFunction();  // 如果抛异常，下面的 delete 不执行
    delete p;
}

// 3. 循环引用
struct Node {
    std::shared_ptr<Node> next;
};
auto n1 = std::make_shared<Node>();
auto n2 = std::make_shared<Node>();
n1->next = n2;
n2->next = n1;  // 循环引用，永远不会释放
```

**解决方案：**

```cpp
// 1. 使用智能指针
void func() {
    auto p = std::make_unique<int>(100);
    // 自动释放
}

// 2. 使用 weak_ptr 打破循环
struct Node {
    std::shared_ptr<Node> next;
    std::weak_ptr<Node> prev;  // 使用 weak_ptr
};

// 3. 使用 RAII 包装器
class ScopedGuard {
    FILE* f;
public:
    ScopedGuard(const char* name) : f(fopen(name, "w")) {}
    ~ScopedGuard() { if (f) fclose(f); }
};
```

**检测工具：**
- Valgrind (Linux)
- AddressSanitizer (ASan)：编译时加 `-fsanitize=address`
- Visual Studio Debugger + CRT 调试堆

---

## 4. 并发与多线程编程

### 4.1 进程与线程

| 特性 | 进程 | 线程 |
|------|------|------|
| **地址空间** | 独立 | 共享所属进程的地址空间 |
| **资源** | 独立拥有 | 共享进程资源 |
| **通信** | IPC（管道、共享内存等） | 直接访问共享变量 |
| **上下文切换** | 慢（需切换页表） | 快 |
| **创建开销** | 大 | 小 |
| **数据安全** | 天然隔离 | 需要同步机制 |

### 4.2 线程创建与管理

**问题：如何创建线程？如何传递参数？如何获取返回值？**

```cpp
#include <thread>
#include <future>
#include <iostream>

// 1. 基本线程创建
void hello() {
    std::cout << "Hello from thread\n";
}

std::thread t1(hello);
t1.join();  // 等待线程结束

// 2. 传递参数
void printMessage(const std::string& msg, int count) {
    for (int i = 0; i < count; ++i) {
        std::cout << msg << "\n";
    }
}

std::string text = "Hello";
std::thread t2(printMessage, std::cref(text), 5);  // 使用 cref 避免拷贝
t2.join();

// 3. 获取返回值：使用 std::future
int calculate(int x) {
    return x * x;
}

std::future<int> result = std::async(calculate, 10);
int value = result.get();  // 阻塞直到完成

// 4. 使用 lambda
std::thread t3([](int id) {
    std::cout << "Thread " << id << "\n";
}, 42);
t3.join();
```

### 4.3 互斥锁与同步

```cpp
#include <mutex>
#include <condition_variable>
#include <atomic>

class ThreadSafeCounter {
private:
    mutable std::mutex mutex_;
    std::condition_variable cv_;
    int count_ = 0;
    bool ready_ = false;

public:
    void increment() {
        std::lock_guard<std::mutex> lock(mutex_);  // RAII 自动解锁
        ++count_;
    }

    int get() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return count_;
    }

    void waitForReady() {
        std::unique_lock<std::mutex> lock(mutex_);
        cv_.wait(lock, [this] { return ready_; });  // 等待条件
    }

    void setReady() {
        {
            std::lock_guard<std::mutex> lock(mutex_);
            ready_ = true;
        }
        cv_.notify_one();  // 通知等待线程
    }
};

// 原子操作（无锁编程）
std::atomic<int> atomic_counter(0);

void atomicIncrement() {
    atomic_counter.fetch_add(1, std::memory_order_relaxed);
}
```

**锁的选择：**
- `std::mutex`：基本互斥锁
- `std::lock_guard`：RAII 封装，作用域自动解锁
- `std::unique_lock`：更灵活，支持手动 lock/unlock、条件变量
- `std::shared_mutex`：读写锁（C++17）

### 4.4 死锁及其预防

**死锁条件（四个必要条件）：**
1. 互斥：资源不能共享
2. 持有并等待：持有资源的同时等待其他资源
3. 不可剥夺：资源不能被强制释放
4. 循环等待：存在等待环路

**预防方法：**

```cpp
// 错误：可能导致死锁
std::mutex m1, m2;

void thread1() {
    std::lock_guard<std::mutex> lock1(m1);
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    std::lock_guard<std::mutex> lock2(m2);  // 等待 m2
}

void thread2() {
    std::lock_guard<std::mutex> lock2(m2);
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    std::lock_guard<std::mutex> lock1(m1);  // 等待 m1，死锁！
}

// 正确：使用 std::lock 同时获取多个锁
void thread1_safe() {
    std::lock(m1, m2);  // 原子地获取两个锁
    std::lock_guard<std::mutex> lock1(m1, std::adopt_lock);
    std::lock_guard<std::mutex> lock2(m2, std::adopt_lock);
}

void thread2_safe() {
    std::lock(m2, m1);  // 顺序不同也安全
    std::lock_guard<std::mutex> lock2(m2, std::adopt_lock);
    std::lock_guard<std::mutex> lock1(m1, std::adopt_lock);
}
```

---

## 5. 数据结构与算法

### 5.1 时间复杂度速查

| 操作 | 数组 | 链表 | 哈希表 | 平衡树 |
|------|------|------|--------|--------|
| 访问 | O(1) | O(n) | O(1) 平均 | O(log n) |
| 插入 | O(n) | O(1)* | O(1) 平均 | O(log n) |
| 删除 | O(n) | O(1)* | O(1) 平均 | O(log n) |
| 搜索 | O(n) | O(n) | O(1) 平均 | O(log n) |

* 假设已有位置指针

### 5.2 手写代码模板

#### 反转单链表（必考）

```cpp
struct ListNode {
    int val;
    ListNode* next;
    ListNode(int x) : val(x), next(nullptr) {}
};

// 迭代法
ListNode* reverseList(ListNode* head) {
    ListNode* prev = nullptr;
    ListNode* curr = head;

    while (curr != nullptr) {
        ListNode* next = curr->next;  // 保存下一个节点
        curr->next = prev;            // 反转指针
        prev = curr;                  // prev 前进
        curr = next;                  // curr 前进
    }

    return prev;  // 新的头节点
}
```

#### 快速排序

```cpp
void quickSort(int arr[], int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

int partition(int arr[], int low, int high) {
    int pivot = arr[high];  // 选择最右为基准
    int i = low - 1;        // i 是小于基准的区域的边界

    for (int j = low; j < high; j++) {
        if (arr[j] < pivot) {
            i++;
            std::swap(arr[i], arr[j]);
        }
    }

    std::swap(arr[i + 1], arr[high]);
    return i + 1;
}
```

#### 二分查找

```cpp
int binarySearch(const std::vector<int>& nums, int target) {
    int left = 0;
    int right = nums.size() - 1;

    while (left <= right) {
        int mid = left + (right - left) / 2;  // 防止溢出

        if (nums[mid] == target) {
            return mid;
        } else if (nums[mid] < target) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }

    return -1;  // 未找到
}
```

### 5.3 STL 常用容器

```cpp
// 序列容器
std::vector<int> vec = {1, 2, 3};      // 动态数组
std::deque<int> deq;                   // 双端队列
std::list<int> lst;                    // 双向链表

// 关联容器
std::map<std::string, int> m;          // 红黑树，有序
std::unordered_map<std::string, int> um;  // 哈希表，无序，更快

std::set<int> s;                       // 有序集合
std::unordered_set<int> us;            // 无序集合

// 容器适配器
std::stack<int> stk;                   // 栈
std::queue<int> q;                     // 队列
std::priority_queue<int> pq;           // 优先队列（大顶堆）

// 常用算法
std::sort(vec.begin(), vec.end());
auto it = std::find(vec.begin(), vec.end(), 3);
bool exists = std::binary_search(vec.begin(), vec.end(), 3);

// Lambda 表达式配合算法
std::sort(vec.begin(), vec.end(), [](int a, int b) {
    return a > b;  // 降序排序
});
```

---

## 6. 系统编程与网络编程

### 6.1 文件 I/O

```cpp
#include <fstream>
#include <sstream>

// 文本文件读写
std::ofstream outfile("data.txt");
if (outfile.is_open()) {
    outfile << "Hello, World!\n";
    outfile.close();
}

std::ifstream infile("data.txt");
std::string line;
while (std::getline(infile, line)) {
    std::cout << line << "\n";
}

// 二进制文件读写
struct Data {
    int id;
    double value;
};

std::ofstream binfile("data.bin", std::ios::binary);
Data data{42, 3.14};
binfile.write(reinterpret_cast<char*>(&data), sizeof(data));
```

### 6.2 Socket 编程基础

**TCP 服务器基本流程：**

```cpp
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

// 1. 创建 socket
int server_fd = socket(AF_INET, SOCK_STREAM, 0);

// 2. 绑定地址和端口
struct sockaddr_in address;
address.sin_family = AF_INET;
address.sin_addr.s_addr = INADDR_ANY;
address.sin_port = htons(8080);

bind(server_fd, (struct sockaddr*)&address, sizeof(address));

// 3. 监听
listen(server_fd, 3);

// 4. 接受连接
int addrlen = sizeof(address);
int new_socket = accept(server_fd, (struct sockaddr*)&address,
                       (socklen_t*)&addrlen);

// 5. 读写数据
char buffer[1024] = {0};
read(new_socket, buffer, 1024);
send(new_socket, "Hello", 5, 0);

// 6. 关闭
close(new_socket);
close(server_fd);
```

---

## 7. 医疗设备软件开发特点

### 7.1 医疗软件的特殊要求

xx医疗作为医疗设备厂商，其软件开发有以下特点：

**1. 安全性和可靠性**
- **高可靠性要求**：设备故障可能危及患者生命
- **IEC 62304 标准**：医疗设备软件生命周期过程
- **故障安全设计**：任何故障都要进入安全状态

**2. 实时性要求**
- **确定性响应**：图像采集、处理需要实时性能
- **优先级调度**：关键任务优先保证
- **延迟敏感**：成像延迟影响诊断和治疗效果

**3. 法规合规**
- **FDA 认证**（美国）
- **CE 认证**（欧盟）
- **NMPA 认证**（中国）
- 需要严格的文档和追溯

**4. 数据完整性**
- **DICOM 标准**：医学影像数据格式
- **患者数据隐私**：HIPAA 合规
- **数据审计**：完整的操作日志

### 7.2 相关技术要点

**DICOM 协议**
```cpp
// DICOM 文件包含：
// - 128 字节 preamble
// - "DICM" 标识
// - DICOM 数据集（标签 + 长度 + 值）

struct DICOMTag {
    uint16_t group;
    uint16_t element;
};
```

**图像处理**
- 多线程并行处理（GPU 加速）
- 内存映射文件处理大图像
- 流水线架构

---

## 8. 行为面试与项目经验

### 8.1 常见行为问题

**1. 介绍一个你解决的技术难题**

使用 STAR 法则：
- **S**ituation：背景和问题
- **T**ask：你的任务
- **A**ction：你采取的行动
- **R**esult：结果和收获

**示例回答框架：**
```
在 [项目名称] 中，我们遇到了 [具体问题，如内存泄漏、性能瓶颈]。
这是导致 [影响，如系统崩溃、响应慢]。

我的任务是 [你的职责]。

我采取了以下行动：
1. 分析问题根源（使用了什么工具/方法）
2. 设计解决方案（为什么选择这个方案）
3. 实施和测试（如何验证）

结果是 [量化指标，如性能提升 50%、内存使用降低 30%]，
这让我学到了 [技术或非技术收获]。
```

**2. 你如何处理代码审查中的不同意见？**

重点体现：
- 开放心态，重视技术讨论而非个人
- 用数据和事实说话
- 团队协作意识
- 对代码质量的追求

**3. 描述一次你学习新技术并将其应用的经历**

重点体现：
- 快速学习能力
- 学以致用的能力
- 技术选择的考虑因素
- 风险评估和控制

### 8.2 技术追问准备

**针对你项目可能被问的问题：**

1. **"你为什么使用智能指针？它有什么开销？"**
2. **"这个链表实现如何处理异常？如果 new 失败会怎样？"**
3. **"你的拷贝赋值运算符为什么要返回引用？"**
4. **"如果多线程同时访问这个链表，会发生什么？如何保证线程安全？"**

---

## 9. 高频面试题速记

### 9.1 必背概念

| 概念 | 关键点 |
|------|--------|
| **RAII** | 资源获取即初始化，构造获取，析构释放 |
| **虚函数** | 运行期多态，通过 vtable 实现 |
| **纯虚函数** | `virtual void func() = 0;` 使类成为抽象类 |
| **const 正确性** | const 成员函数不修改对象状态 |
| **内存对齐** | struct 大小是成员大小的倍数，优化访问速度 |
| **volatile** | 告诉编译器不要优化，每次从内存读取 |
| **explicit** | 防止单参数构造函数的隐式转换 |
| **friend** | 友元函数/类可访问私有成员 |

### 9.2 常见陷阱

```cpp
// 1. 数组退化
void func(int arr[]) {
    int size = sizeof(arr) / sizeof(arr[0]);  // 错误！
    // arr 是指针，不是数组
}

// 2. 切片问题
class Base { int x; };
class Derived : public Base { int y; };
Derived d;
Base b = d;  // 对象切片，y 丢失

// 3. 临时对象生命周期
const std::string& str = std::string("hello");
// 临时对象在语句结束就销毁，str 变成悬空引用！

// 4. 虚析构函数
Base* p = new Derived();
delete p;  // 如果 Base 析构函数不是虚的，未定义行为
```

---

## 10. 三天学习计划

### Day 1：基础巩固
- [ ] C/C++ 语言基础（指针、引用、const）
- [ ] 面向对象三大特性
- [ ] 虚函数与多态原理
- [ ] 内存布局与内存管理

### Day 2：进阶主题
- [ ] STL 容器与算法
- [ ] 智能指针与 RAII
- [ ] 多线程编程基础
- [ ] 数据结构与手写代码

### Day 3：实战准备
- [ ] 手写常见算法题
- [ ] 准备项目经验 STAR 故事
- [ ] 了解医疗设备软件开发特点
- [ ] 模拟面试，练习表达

---

## 📚 参考资源

**书籍：**
- 《C++ Primer》（必读经典）
- 《Effective C++》（进阶必读）
- 《深度探索 C++ 对象模型》（底层原理）

**在线资源：**
- cppreference.com（权威参考）
- Compiler Explorer（在线编译测试）

**面试题库：**
- LeetCode Hot 100
- 牛客网 C++ 面试题

---

## ✅ 面试前检查清单

- [ ] 理解虚函数表的实现机制
- [ ] 能够手写快速排序、二分查找、链表反转
- [ ] 掌握智能指针的使用场景
- [ ] 理解 RAII 和异常安全
- [ ] 能够解释多线程同步机制
- [ ] 了解 DICOM 和医疗设备特点
- [ ] 准备 2-3 个项目经验故事
- [ ] 准备向面试官提问的问题

---

## 🎯 向面试官提问示例

1. "团队目前在开发什么类型的医疗设备？"
2. "团队的代码质量保证流程是怎样的？"
3. "这个岗位的技术挑战主要在哪些方面？"
4. "团队对新技术的采用态度如何？"
5. "公司对员工技术成长的投入和支持？"

---

**祝你面试成功！**
