# C/C++ 开发岗面试突击指南 - 医疗行业专项

> 准备时间：3天 | 目标岗位：xxx医疗 C/C++ 开发 | 经验要求：2年

---

## 📋 目录

1. [C/C++ 语言基础](#1-cc-语言基础)
   - 1.1 C与C++的核心区别
   - 1.2 指针与引用
   - 1.3 const 关键字的深层理解
   - 1.4 sizeof 与 strlen 的区别
2. [面向对象与设计模式](#2-面向对象与设计模式)
   - 2.1 三大核心概念详解
   - 2.2 虚函数与虚函数表
   - 2.3 菱形继承（虚继承）
   - 2.4 构造函数与析构函数
   - 2.5 拷贝控制：拷贝构造与赋值运算符
   - 2.6 常用设计模式
     - 单例模式 (Singleton)
     - 工厂模式 (Factory)
     - 观察者模式 (Observer)
     - 迭代器模式 (Iterator)
3. [内存管理与资源管理](#3-内存管理与资源管理)
   - 3.1 内存分区详解
   - 3.2 new/malloc 与 delete/free 的区别
   - 3.3 智能指针（现代 C++ 必备）
   - 3.4 内存泄漏检测与预防
4. [并发与多线程编程](#4-并发与多线程编程)
   - 4.1 进程与线程
   - 4.2 线程创建与管理
   - 4.3 互斥锁与同步
   - 4.4 死锁及其预防
5. [数据结构与算法](#5-数据结构与算法)
   - 5.1 时间复杂度速查
   - 5.2 手写代码模板
   - 5.3 STL 常用容器
6. [系统编程与网络编程](#6-系统编程与网络编程)
   - 6.1 静态库与动态库（.lib/.a/.dll/.so）
   - 6.2 文件 I/O
   - 6.3 Socket 编程基础
   - 6.4 WebSocket 连接建立（WS/WSS）
7. [医疗设备软件开发特点](#7-医疗设备软件开发特点)
   - 7.1 医疗软件的特殊要求
   - 7.2 相关技术要点
8. [行为面试与项目经验](#8-行为面试与项目经验)
   - 8.1 常见行为问题
   - 8.2 技术追问准备
9. [高频面试题速记](#9-高频面试题速记)
   - 9.1 必背概念
   - 9.2 常见陷阱

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

### 2.3 菱形继承（虚继承）

**问题：什么是菱形继承？有什么问题？如何解决？**

**标准答案：**

**菱形继承结构：**
```
      A (基类)
     / \
    B   C (都继承自 A)
     \ /
      D (多继承 B 和 C)
```

```cpp
// 问题示例
class A {
public:
    int data;
};

class B : public A {};
class C : public A {};
class D : public B, public C {};  // 菱形继承

D d;
d.data = 10;  // ❌ 编译错误！二义性
```

**问题：二义性 & 数据冗余**

| 问题 | 说明 |
|------|------|
| **二义性** | `d.data` 编译器不知道是 `B::data` 还是 `C::data` |
| **数据冗余** | D 对象中包含两份 A 的副本，浪费内存 |

**内存布局（虚继承前）：**
```
D 对象内存:
┌─────────────┐
│  A (来自 B) │  data
├─────────────┤
│  B 部分     │
├─────────────┤
│  A (来自 C) │  data ← 冗余！两份 A
├─────────────┤
│  C 部分     │
├─────────────┤
│  D 部分     │
└─────────────┘
```

**解决方案：虚继承（virtual inheritance）**

```cpp
class A {
public:
    int data;
};

class B : virtual public A {};  // 虚继承
class C : virtual public A {};  // 虚继承
class D : public B, public C {};

D d;
d.data = 10;  // ✅ 正确！只有一份 A
```

**内存布局（虚继承后）：**
```
D 对象内存:
┌─────────────┐
│  B 部分     │  (包含指向 A 的指针)
├─────────────┤
│  C 部分     │  (包含指向 A 的指针)
├─────────────┤
│  D 部分     │
├─────────────┤
│  A (共享)   │  data ← 只有一份 A
└─────────────┘
```

**面试高频问题：**

| 问题 | 答案 |
|------|------|
| 虚继承的代价？ | 增加指针间接访问，性能稍降 |
| 构造顺序？ | 先构造虚基类 A，再 B、C，最后 D |
| sizeof(D) 是多少？ | 包含两个虚继承指针 + 数据成员 |
| 虚基类构造几次？ | 只构造一次，由最远派生类调用 |

**关键记忆点：**
- 虚基类只构造一次，在所有普通基类之前
- 访问虚基类成员需要通过指针间接访问，有性能开销

### 2.4 构造函数与析构函数

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

### 2.5 拷贝控制：拷贝构造与赋值运算符

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

### 2.6 常用设计模式

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

#### 观察者模式 (Observer)

**问题：如何实现一对多的依赖关系，当一对象状态改变时，所有依赖它的对象都得到通知？**

**标准答案：**

观察者模式定义对象间的一对多依赖，当一个对象改变状态时，所有依赖者都会收到通知。

```
        Subject (主题/被观察者)
                 │
        ┌────────┼────────┐
        ▼        ▼        ▼
    Observer1 Observer2 Observer3
    (观察者)  (观察者)  (观察者)
```

```cpp
#include <iostream>
#include <vector>
#include <functional>
#include <string>

// 观察者接口
using Observer = std::function<void(const std::string& message)>;

// Subject (被观察者)
class NewsAgency {
private:
    std::vector<Observer> observers_;
    std::string latestNews_;

public:
    // 订阅（注册观察者）
    void subscribe(Observer observer) {
        observers_.push_back(observer);
    }

    // 取消订阅
    void unsubscribe(Observer observer) {
        // 简化版：实际中可以用 ID 或更复杂的移除逻辑
        // 这里仅作示例
    }

    // 发布新闻（通知所有观察者）
    void publish(const std::string& news) {
        latestNews_ = news;
        for (auto& observer : observers_) {
            observer(news);  // 调用观察者的回调
        }
    }
};

// 具体观察者
class NewsSubscriber {
private:
    std::string name_;

public:
    NewsSubscriber(const std::string& name) : name_(name) {}

    Observer getCallback() {
        // 返回一个 lambda 作为观察者回调
        return [this](const std::string& news) {
            std::cout << name_ << " 收到新闻: " << news << std::endl;
        };
    }
};

// 使用示例
void demoObserver() {
    NewsAgency agency;

    // 订阅
    NewsSubscriber sub1("订阅者A");
    NewsSubscriber sub2("订阅者B");

    agency.subscribe(sub1.getCallback());
    agency.subscribe(sub2.getCallback());

    // 发布新闻
    agency.publish("C++11 新特性发布！");

    // 输出:
    // 订阅者A 收到新闻: C++11 新特性发布！
    // 订阅B 收到新闻: C++11 新特性发布！
}
```

**应用场景：**
- GUI 事件处理（按钮点击、键盘输入）
- 消息队列/事件总线
- MVC 架构中的 Model-View 通信
- 发布-订阅系统

---

#### 迭代器模式 (Iterator)

**问题：如何遍历一个聚合对象，而又不暴露其内部表示？**

**标准答案：**

迭代器模式提供一种方法顺序访问聚合对象中的各个元素，而不暴露其底层表示。

```cpp
#include <iostream>
#include <vector>

// 自定义容器
class IntContainer {
private:
    std::vector<int> data_;

public:
    IntContainer(const std::initializer_list<int>& list) : data_(list) {}

    // 迭代器类
    class Iterator {
    private:
        std::vector<int>::iterator it_;
        std::vector<int>::iterator end_;

    public:
        Iterator(std::vector<int>::iterator it, std::vector<int>::iterator end)
            : it_(it), end_(end) {}

        // 解引用
        int& operator*() { return *it_; }

        // 箭头运算符
        Iterator* operator->() { return this; }

        // 前置 ++
        Iterator& operator++() {
            ++it_;
            return *this;
        }

        // 后置 ++
        Iterator operator++(int) {
            Iterator temp = *this;
            ++it_;
            return temp;
        }

        // 相等比较
        bool operator!=(const Iterator& other) const {
            return it_ != other.it_;
        }

        bool operator==(const Iterator& other) const {
            return it_ == other.it_;
        }
    };

    // 返回迭代器
    Iterator begin() { return Iterator(data_.begin(), data_.end()); }
    Iterator end()   { return Iterator(data_.end(), data_.end()); }
};

// 使用示例
void demoIterator() {
    IntContainer container = {1, 2, 3, 4, 5};

    // 范围 for 循环（需要 begin()/end()）
    for (IntContainer::Iterator it = container.begin(); it != container.end(); ++it) {
        std::cout << *it << " ";
    }
    // 输出: 1 2 3 4 5

    // 也可以这样写
    for (auto it = container.begin(); it != container.end(); ++it) {
        std::cout << *it << " ";
    }
}
```

**STL 风格的迭代器类型：**
| 类型 | 说明 | 示例 |
|------|------|------|
| **InputIterator** | 只读，单向 | `istream_iterator` |
| **OutputIterator** | 只写，单向 | `ostream_iterator` |
| **ForwardIterator** | 可读写，多遍历 | `forward_list` |
| **BidirectionalIterator** | 双向遍历 | `list`, `set` |
| **RandomAccessIterator** | 随机访问 | `vector`, `array` |

**应用场景：**
- STL 容器统一遍历接口
- 范围 for 循环
- 算法与容器解耦

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

#### 5.3.1 容器对比速查

| 容器 | 底层结构 | 查找 | 插入 | 删除 | 迭代器 | 使用场景 |
|------|---------|------|------|------|--------|----------|
| **vector** | 动态数组 | O(n) | O(n)* | O(n)* | 随机访问 | 默认首选，频繁随机访问 |
| **deque** | 双端队列 | O(n) | O(1) | O(1) | 随机访问 | 头尾频繁插入删除 |
| **list** | 双向链表 | O(n) | O(1)* | O(1)* | 双向 | 中间频繁插入删除 |
| **map** | 红黑树 | O(log n) | O(log n) | O(log n) | 双向 | 有序键值对 |
| **unordered_map** | 哈希表 | O(1) | O(1) | O(1) | 前向 | 无序键值对，高频查找 |
| **set** | 红黑树 | O(log n) | O(log n) | O(log n) | 双向 | 有序去重集合 |
| **unordered_set** | 哈希表 | O(1) | O(1) | O(1) | 前向 | 无序去重集合，高频查找 |

\* 表示如果已有位置指针则为 O(1)

---

#### 5.3.2 vector 常用操作

```cpp
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int main() {
    // 1. 创建和初始化
    vector<int> vec1 = {1, 2, 3, 4, 5};          // 列表初始化
    vector<int> vec2(10);                        // 10 个元素，默认值 0
    vector<int> vec3(10, 42);                    // 10 个元素，值都是 42

    // 2. 元素访问
    cout << "vec1[0] = " << vec1[0] << endl;     // 不检查越界
    cout << "vec1.at(0) = " << vec1.at(0) << endl; // 检查越界，抛异常
    cout << "front: " << vec1.front() << endl;   // 第一个元素
    cout << "back: " << vec1.back() << endl;     // 最后一个元素

    // 3. 容量操作
    cout << "size: " << vec1.size() << endl;     // 元素个数
    cout << "capacity: " << vec1.capacity() << endl;  // 容量
    cout << "empty: " << vec1.empty() << endl;   // 是否为空
    vec1.reserve(100);                           // 预留容量（不改变 size）

    // 4. 修改操作
    vec1.push_back(6);                           // 尾部添加
    vec1.pop_back();                             // 尾部删除
    vec1.insert(vec1.begin() + 2, 99);           // 在位置 2 插入
    vec1.erase(vec1.begin() + 2);                // 删除位置 2 的元素
    vec1.clear();                                // 清空

    // 5. 遍历
    vector<int> vec = {10, 20, 30, 40, 50};
    for (auto it = vec.begin(); it != vec.end(); ++it) {
        cout << *it << " ";
    }
    cout << endl;

    // 范围 for 循环（推荐）
    for (const auto& elem : vec) {
        cout << elem << " ";
    }
    cout << endl;

    return 0;
}
```

#### 5.3.3 map 常用操作

```cpp
#include <iostream>
#include <map>
#include <string>

using namespace std;

int main() {
    // 1. 创建和初始化
    map<string, int> mp;

    // 2. 插入元素
    mp["apple"] = 5;                             // [] 运算符（如果不存在则插入）
    mp.insert({"banana", 3});                    // insert 方法
    mp.insert(make_pair("cherry", 8));

    // 3. 元素访问
    cout << "apple: " << mp["apple"] << endl;    // [] 运算符（不存在会插入默认值）
    cout << "banana: " << mp.at("banana") << endl; // at() 方法（不存在抛异常）

    // 4. 查找操作
    auto it = mp.find("banana");                 // 查找，返回迭代器
    if (it != mp.end()) {
        cout << "found: " << it->first << " -> " << it->second << endl;
    }

    int count = mp.count("cherry");              // count（map 中是 0 或 1）
    cout << "count cherry: " << count << endl;

    // 5. 删除操作
    mp.erase("apple");                           // 按键删除

    // 6. 遍历（自动按 key 排序）
    for (const auto& pair : mp) {
        cout << pair.first << " -> " << pair.second << endl;
    }

    // 7. 容量操作
    cout << "size: " << mp.size() << endl;
    cout << "empty: " << mp.empty() << endl;

    return 0;
}
```

#### 5.3.4 unordered_map 常用操作

```cpp
#include <iostream>
#include <unordered_map>
#include <string>

using namespace std;

int main() {
    // 操作与 map 类似，但遍历时无序
    unordered_map<string, int> ump;

    // 插入
    ump["apple"] = 5;
    ump.insert({"banana", 3});
    ump["cherry"] = 8;

    // 查找
    auto it = ump.find("banana");
    if (it != ump.end()) {
        cout << "found: " << it->first << " -> " << it->second << endl;
    }

    // 遍历（顺序不确定）
    for (const auto& pair : ump) {
        cout << pair.first << " -> " << pair.second << endl;
    }

    // bucket 操作（哈希表特有）
    cout << "bucket count: " << ump.bucket_count() << endl;
    cout << "max bucket count: " << ump.max_bucket_count() << endl;
    cout << "load factor: " << ump.load_factor() << endl;

    return 0;
}
```

#### 5.3.5 set 常用操作

```cpp
#include <iostream>
#include <set>

using namespace std;

int main() {
    // 1. 创建和初始化（自动去重和排序）
    set<int> s1;
    s1.insert(5);
    s1.insert(2);
    s1.insert(8);
    s1.insert(1);
    s1.insert(9);
    s1.insert(2);  // 重复值不会被插入

    // 2. 查找
    auto it = s1.find(5);
    if (it != s1.end()) {
        cout << "found: " << *it << endl;
    }

    bool exists = s1.count(8) > 0;  // count 返回 0 或 1
    cout << "8 exists: " << exists << endl;

    // 3. 范围查询（set 特有优势）
    auto lower = s1.lower_bound(5);   // >= 5 的第一个
    auto upper = s1.upper_bound(8);   // > 8 的第一个

    cout << "elements in [5, 8]: ";
    for (auto it = lower; it != upper; ++it) {
        cout << *it << " ";
    }
    cout << endl;

    // 4. 删除
    s1.erase(5);              // 按值删除
    s1.erase(s1.begin());     // 按迭代器删除

    // 5. 遍历（有序）
    for (const auto& elem : s1) {
        cout << elem << " ";
    }
    cout << endl;

    return 0;
}
```

#### 5.3.6 list 常用操作

```cpp
#include <iostream>
#include <list>
#include <algorithm>

using namespace std;

int main() {
    list<int> lst = {1, 2, 3, 4, 5};

    // 1. 两端插入（list 优势：O(1)）
    lst.push_front(0);      // 头部插入
    lst.push_back(6);       // 尾部插入

    // 2. 中间插入（需要先移动迭代器）
    auto it = lst.begin();
    advance(it, 3);         // 移动迭代器到位置 3
    lst.insert(it, 99);     // 在位置 3 插入

    // 3. 删除
    lst.pop_front();        // 删除头部
    lst.pop_back();         // 删除尾部
    lst.remove(99);         // 删除所有值为 99 的元素

    // 4. 特有操作（list 独有）
    lst.sort();             // 排序（list 独有，优于通用算法）
    lst.reverse();          // 反转
    lst.unique();           // 删除连续重复元素

    // 5. 遍历
    for (const auto& elem : lst) {
        cout << elem << " ";
    }
    cout << endl;

    return 0;
}
```

#### 5.3.7 deque 常用操作

```cpp
#include <iostream>
#include <deque>

using namespace std;

int main() {
    deque<int> dq;

    // 1. 两端操作（deque 优势）
    dq.push_back(1);    // 尾部
    dq.push_back(2);
    dq.push_front(0);   // 头部
    dq.push_front(-1);

    // 2. 随机访问（deque 支持，list 不支持）
    cout << "dq[2] = " << dq[2] << endl;
    cout << "dq.at(2) = " << dq.at(2) << endl;

    // 3. 遍历
    for (const auto& elem : dq) {
        cout << elem << " ";
    }
    cout << endl;

    // 4. 两端删除
    dq.pop_front();
    dq.pop_back();

    return 0;
}
```

#### 5.3.8 容器适配器

```cpp
#include <iostream>
#include <stack>
#include <queue>
#include <vector>
#include <functional>

using namespace std;

int main() {
    // 1. stack（后进先出）
    cout << "--- stack ---" << endl;
    stack<int> stk;
    stk.push(1);
    stk.push(2);
    stk.push(3);
    while (!stk.empty()) {
        cout << stk.top() << " ";  // 查看顶部
        stk.pop();                 // 弹出
    }
    cout << endl;

    // 2. queue（先进先出）
    cout << "--- queue ---" << endl;
    queue<int> q;
    q.push(1);
    q.push(2);
    q.push(3);
    while (!q.empty()) {
        cout << q.front() << " ";  // 查看队首
        q.pop();                   // 出队
    }
    cout << endl;

    // 3. priority_queue（优先级队列，大顶堆）
    cout << "--- priority_queue (大顶堆) ---" << endl;
    priority_queue<int> pq;  // 默认大顶堆
    pq.push(3);
    pq.push(1);
    pq.push(4);
    pq.push(2);
    while (!pq.empty()) {
        cout << pq.top() << " ";  // 每次取最大
        pq.pop();
    }
    cout << endl;

    // 4. 小顶堆
    cout << "--- priority_queue (小顶堆) ---" << endl;
    priority_queue<int, vector<int>, greater<int>> minPq;
    minPq.push(3);
    minPq.push(1);
    minPq.push(4);
    minPq.push(2);
    while (!minPq.empty()) {
        cout << minPq.top() << " ";  // 每次取最小
        minPq.pop();
    }
    cout << endl;

    return 0;
}
```

---

#### 5.3.9 实战示例一：两数之和（LeetCode 1）

**题目：** 给定一个整数数组 `nums` 和一个目标值 `target`，找出和为目标值的两个整数，返回它们的索引。

```cpp
#include <iostream>
#include <vector>
#include <unordered_map>

using namespace std;

vector<int> twoSum(vector<int>& nums, int target) {
    unordered_map<int, int> mp;  // 值 -> 索引

    for (int i = 0; i < nums.size(); ++i) {
        int complement = target - nums[i];

        // 查找补数是否已存在
        if (mp.find(complement) != mp.end()) {
            return {mp[complement], i};
        }

        // 将当前值和索引存入 map
        mp[nums[i]] = i;
    }

    return {};  // 未找到
}

int main() {
    vector<int> nums = {2, 7, 11, 15};
    int target = 9;

    vector<int> result = twoSum(nums, target);

    cout << "索引: [" << result[0] << ", " << result[1] << "]" << endl;
    cout << "值: " << nums[result[0]] << " + " << nums[result[1]] << " = " << target << endl;

    return 0;
}
```

**输出：**
```
索引: [0, 1]
值: 2 + 7 = 9
```

**关键点：**
- 使用 `unordered_map` 实现 O(n) 时间复杂度
- `find()` 查找补数是否存在
- 遍历时边查找边插入，避免重复使用同一元素

---

#### 5.3.10 实战示例二：词频统计

**题目：** 统计文本中每个单词出现的频率，并按频率从高到低排序输出。

```cpp
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <cctype>

using namespace std;

// 手动分割字符串为单词
vector<string> splitWords(const string& text) {
    vector<string> words;
    string word;

    for (size_t i = 0; i < text.size(); ++i) {
        char c = text[i];

        if (isalpha(c) || c == '\'') {
            // 字母或单引号，构建单词
            word += c;
        } else if (!word.empty()) {
            // 遇到分隔符且单词非空，保存单词
            words.push_back(word);
            word.clear();
        }
    }

    // 处理最后一个单词
    if (!word.empty()) {
        words.push_back(word);
    }

    return words;
}

// 统计词频并排序
vector<pair<string, int>> countWordFrequency(const string& text) {
    unordered_map<string, int> wordCount;

    // 分割单词
    vector<string> words = splitWords(text);

    // 统计频率（区分大小写）
    for (const auto& word : words) {
        if (!word.empty()) {
            wordCount[word]++;
        }
    }

    // 转换为 vector 并按频率排序
    vector<pair<string, int>> freqList(wordCount.begin(), wordCount.end());

    sort(freqList.begin(), freqList.end(),
         [](const pair<string, int>& a, const pair<string, int>& b) {
             // 频率相同按字母序，不同按频率降序
             if (a.second != b.second) {
                 return a.second > b.second;
             }
             return a.first < b.first;
         });

    return freqList;
}

int main() {
    // 示例文本
    string text = "C++ is a powerful programming language. C++ supports multiple "
                  "paradigms. Programming in C++ requires understanding of memory "
                  "management. The C++ STL provides powerful containers and algorithms. "
                  "c++ and C++ are the same language.";

    // 统计词频
    vector<pair<string, int>> result = countWordFrequency(text);

    // 输出结果
    cout << "=== 词频统计结果（区分大小写）===" << endl;
    cout << "单词总数: " << result.size() << endl;
    cout << endl;

    for (const auto& pair : result) {
        cout << pair.first << ": " << pair.second << endl;
    }

    return 0;
}
```

**输出：**
```
=== 词频统计结果（区分大小写）===
单词总数: 24

C++: 4
powerful: 2
Programming: 1
programming: 1
...
```

**关键点：**
- 手动实现 `splitWords()` 函数，不使用 `<sstream>`
- 遍历字符，根据字母和非字母边界分割单词
- 保留单词大小写，`C++` 和 `c++` 被视为不同单词
- 使用 `unordered_map` 统计频率，O(n) 时间复杂度
- 转换为 `vector<pair>` 后按频率排序

---

#### 5.3.11 实战示例三：合并区间（LeetCode 56）

**题目：** 合并所有重叠的区间。

```cpp
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

struct Interval {
    int start;
    int end;
    Interval(int s, int e) : start(s), end(e) {}
};

vector<Interval> merge(vector<Interval>& intervals) {
    if (intervals.empty()) return {};

    // 1. 按起始时间排序
    sort(intervals.begin(), intervals.end(),
         [](const Interval& a, const Interval& b) {
             return a.start < b.start;
         });

    vector<Interval> result;
    result.push_back(intervals[0]);

    // 2. 遍历合并
    for (size_t i = 1; i < intervals.size(); ++i) {
        Interval& last = result.back();

        // 如果当前区间与上一个重叠，合并
        if (intervals[i].start <= last.end) {
            last.end = max(last.end, intervals[i].end);
        } else {
            result.push_back(intervals[i]);
        }
    }

    return result;
}

int main() {
    vector<Interval> intervals = {
        Interval(1, 3),
        Interval(2, 6),
        Interval(8, 10),
        Interval(15, 18)
    };

    vector<Interval> merged = merge(intervals);

    cout << "合并后的区间:" << endl;
    for (const auto& interval : merged) {
        cout << "[" << interval.start << ", " << interval.end << "] ";
    }
    cout << endl;

    return 0;
}
```

**输出：**
```
合并后的区间:
[1, 6] [8, 10] [15, 18]
```

---

#### 5.3.12 实战示例四：前K个高频元素（LeetCode 347）

**题目：** 给定一个非空数组，返回前 k 个出现频率最高的元素。

```cpp
#include <iostream>
#include <vector>
#include <unordered_map>
#include <queue>
#include <utility>

using namespace std;

vector<int> topKFrequent(vector<int>& nums, int k) {
    // 1. 统计频率
    unordered_map<int, int> freqMap;
    for (int num : nums) {
        freqMap[num]++;
    }

    // 2. 使用小顶堆维护前 k 个高频元素
    // pair<频率, 值>，小顶堆按频率排序
    using Pair = pair<int, int>;
    priority_queue<Pair, vector<Pair>, greater<Pair>> minHeap;

    for (const auto& entry : freqMap) {
        minHeap.push({entry.second, entry.first});

        // 保持堆大小为 k
        if (minHeap.size() > k) {
            minHeap.pop();  // 移除频率最低的
        }
    }

    // 3. 提取结果
    vector<int> result;
    while (!minHeap.empty()) {
        result.push_back(minHeap.top().second);
        minHeap.pop();
    }

    return result;
}

int main() {
    vector<int> nums = {1, 1, 1, 2, 2, 3};
    int k = 2;

    vector<int> result = topKFrequent(nums, k);

    cout << "前 " << k << " 个高频元素: ";
    for (int num : result) {
        cout << num << " ";
    }
    cout << endl;

    return 0;
}
```

**输出：**
```
前 2 个高频元素: 1 2
```

---

#### 5.3.13 实战示例五：有效的括号（LeetCode 20）

**题目：** 给定一个只包括 `'('`，`')'`，`'{'`，`'}'`，`'['`，`']'` 的字符串，判断字符串是否有效。

```cpp
#include <iostream>
#include <string>
#include <stack>
#include <unordered_map>

using namespace std;

bool isValid(string s) {
    stack<char> stk;

    // 定义括号匹配规则
    unordered_map<char, char> matching = {
        {')', '('},
        {']', '['},
        {'}', '{'}
    };

    for (char c : s) {
        // 如果是左括号，入栈
        if (c == '(' || c == '[' || c == '{') {
            stk.push(c);
        }
        // 如果是右括号
        else {
            // 栈为空或栈顶不匹配
            if (stk.empty() || stk.top() != matching[c]) {
                return false;
            }
            stk.pop();
        }
    }

    // 栈为空则全部匹配
    return stk.empty();
}

int main() {
    vector<string> tests = {
        "()",
        "()[]{}",
        "(]",
        "([)]",
        "{[]}"
    };

    for (const auto& s : tests) {
        cout << s << " -> " << (isValid(s) ? "有效" : "无效") << endl;
    }

    return 0;
}
```

**输出：**
```
() -> 有效
()[]{} -> 有效
(] -> 无效
([)] -> 无效
{[]} -> 有效
```

---

#### 5.3.14 迭代器类型速查

| 迭代器类型 | 能力 | 支持操作 | 典型容器 |
|-----------|------|---------|----------|
| **输入迭代器** | 只读，单向 | `*it`, `++it`, `==`, `!=` | `istream_iterator` |
| **输出迭代器** | 只写，单向 | `*it =`, `++it` | `ostream_iterator` |
| **前向迭代器** | 可读写，多遍历 | 上述 + `->` | `forward_list`, `unordered_map` |
| **双向迭代器** | 双向遍历 | 上述 + `--it` | `list`, `map`, `set` |
| **随机访问迭代器** | 随机访问 | 上述 + `it[n]`, `it + n`, `it - n` | `vector`, `deque`, `array` |

---

#### 5.3.15 常用算法速查

```cpp
#include <algorithm>
#include <functional>
#include <numeric>

// ========== 排序相关 ==========
sort(vec.begin(), vec.end());                              // 升序排序
sort(vec.begin(), vec.end(), greater<int>());              // 降序排序
stable_sort(vec.begin(), vec.end());                       // 稳定排序
partial_sort(vec.begin(), vec.begin() + 5, vec.end());     // 部分排序
nth_element(vec.begin(), vec.begin() + n, vec.end());      // 第 n 大元素

// ========== 查找相关 ==========
auto it = find(vec.begin(), vec.end(), value);             // 查找值
auto it = find_if(vec.begin(), vec.end(), pred);           // 查找满足条件的
bool exists = binary_search(vec.begin(), vec.end(), value); // 二分查找（需先排序）
int count = count(vec.begin(), vec.end(), value);          // 计数
int cnt = count_if(vec.begin(), vec.end(), pred);          // 条件计数

// ========== 修改相关 ==========
reverse(vec.begin(), vec.end());                           // 反转
fill(vec.begin(), vec.end(), value);                       // 填充
generate(vec.begin(), vec.end(), gen);                     // 生成值
random_shuffle(vec.begin(), vec.end());                    // 随机打乱（C++11前可用）

// ========== 去重（需要先排序）==========
sort(vec.begin(), vec.end());
auto last = unique(vec.begin(), vec.end());                // 移到末尾
vec.erase(last, vec.end());                                // 删除

// ========== 堆操作 ==========
make_heap(vec.begin(), vec.end());                         // 建堆
push_heap(vec.begin(), vec.end());                         // 入堆
pop_heap(vec.begin(), vec.end());                          // 出堆
sort_heap(vec.begin(), vec.end());                         // 堆排序

// ========== 数值算法（<numeric>）==========
int sum = accumulate(vec.begin(), vec.end(), 0);           // 求和
int prod = accumulate(vec.begin(), vec.end(), 1, multiplies<int>());  // 乘积
adjacent_difference(vec.begin(), vec.end(), out.begin());   // 相邻差
```

---

#### 5.3.16 容器选择建议

```
┌────────────────────────────────────────────────────────────┐
│                    容器选择决策树                           │
├────────────────────────────────────────────────────────────┤
│                                                            │
│  需要键值对存储？                                           │
│     ├── 是 → 需要排序？                                     │
│     │        ├── 是 → map / set                            │
│     │        └── 否 → unordered_map / unordered_set        │
│     │                                                      │
│     └── 否 → 需要频繁在中间插入/删除？                       │
│              ├── 是 → list                                 │
│              └── 否 → 需要两端操作？                        │
│                       ├── 是 → deque                       │
│                       └── 否 → vector（默认首选）          │
│                                                            │
└────────────────────────────────────────────────────────────┘
```

| 场景 | 推荐容器 | 理由 |
|------|---------|------|
| 默认选择 | **vector** | 随机访问高效，内存连续，缓存友好 |
| 需要两端频繁操作 | **deque** | 两端插入删除 O(1) |
| 需要中间频繁插入删除 | **list** | 已知位置插入删除 O(1) |
| 有序键值对 | **map** | 自动排序，范围查询高效 |
| 高频查找（无序） | **unordered_map** | O(1) 平均查找 |
| 有序去重集合 | **set** | 自动排序去重 |
| 无序去重集合 | **unordered_set** | O(1) 平均查找插入 |
| LIFO 场景 | **stack** | 后进先出 |
| FIFO 场景 | **queue** | 先进先出 |
| 动态获取最值 | **priority_queue** | 堆维护，O(log n) 插入删除 |

---

## 6. 系统编程与网络编程

### 6.1 静态库与动态库

**问题：Windows 和 Linux 平台的库文件有什么区别？**

**标准答案：**

**平台对比：**
```
┌─────────────────────────────────────────────────────────┐
│                    静态库 vs 动态库                       │
├─────────────────────────────────────────────────────────┤
│                    静态库          │         动态库      │
├─────────────────────────────────────────────────────────┤
│  Windows     │    .lib           │        .dll          │
│  Linux       │    .a             │        .so           │
└─────────────────────────────────────────────────────────┘
```

**详细对比：**

| 特性 | Windows .lib | Linux .a | Windows .dll | Linux .so |
|------|-------------|---------|-------------|-----------|
| **类型** | 静态库 OR 导入库 | 静态库 | 动态库 | 动态库 |
| **代码** | 完整对象代码 | 完整对象代码 | 运行时加载 | 运行时加载 |
| **链接时** | 编译时链接到 exe | 编译时链接到 exe | 需要 .lib 导入库 | 直接链接 .so |
| **运行时** | 不需要 | 不需要 | 需要 .dll 在 PATH | 需要 .so 在 LD_LIBRARY_PATH |

**关键点解释：**

**1. Windows .lib 的双重身份**

```cpp
// 情况 1: .lib 作为纯静态库
// 编译: cl /c mylib.c → lib mylib.obj
// 链接: link main.obj mylib.lib → main.exe (代码被复制进 exe)

// 情况 2: .lib 作为 DLL 的导入库（Import Library）
// 编译: cl /LD mylib.c → mylib.dll + mylib.lib
// 链接: link main.obj mylib.lib → main.exe (运行时需要 mylib.dll)
//        mylib.lib 只包含符号信息，不含实际代码
```

**2. 编译链接过程对比**

```
静态链接 (Windows .lib / Linux .a):
┌──────────────┐     ┌──────────────┐     ┌──────────────┐
│  main.obj    │ +   │  mylib.a/.lib│  →  │   exe (大)   │
└──────────────┘     └──────────────┘     └──────────────┘
                         ↑
                    代码被复制进 exe，运行时不需要库文件

动态链接 (Windows .dll / Linux .so):
┌──────────────┐     ┌──────────────┐     ┌──────────────┐
│  main.obj    │ +   │  import.lib  │  →  │  exe (小)    │
└──────────────┘     └──────────────┘     └──────────────┘
                                              │
                                        运行时需要 .dll/.so
```

**3. 命名习惯**

| 平台 | 静态库命名 | 动态库命名 |
|------|-----------|-----------|
| **Windows** | `name.lib` | `name.dll` |
| **Linux** | `libname.a` | `libname.so` |

**4. 导出符号的差异**

```cpp
// Windows: 需要显式导出
#define EXPORT __declspec(dllexport)
EXPORT void myFunction() {}

// Linux: 默认导出所有符号
void myFunction() {}  // 自动导出
```

**5. 运行时查找**

```bash
# Windows: DLL 搜索顺序
# 1. 程序目录
# 2. 当前工作目录
# 3. System32 目录
# 4. PATH 环境变量

# Linux: .so 搜索路径
# 1. LD_LIBRARY_PATH 环境变量
# 2. /etc/ld.so.cache 缓存
# 3. /lib 和 /usr/lib
```

**面试高频问题：**

| 问题 | 答案 |
|------|------|
| .lib 是静态还是动态？ | 两者都可能，静态库包含代码，DLL导入库只含符号 |
| 为什么需要导入库？ | Windows DLL 需要显式导出，导入库提供链接信息 |
| Linux 为什么不需要导入库？ | .so 本身包含符号表，链接时直接使用 |
| 动态库的优点？ | 节省内存、便于更新、减少 exe 体积 |
| 动态库的缺点？ | 版本兼容问题（DLL Hell）、发布时需带库文件 |

### 6.2 文件 I/O

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

### 6.3 Socket 编程基础

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

### 6.4 WebSocket 连接建立

**问题：WebSocket 和 WSS 的连接建立过程？**

**标准答案：**

WebSocket 是建立在 TCP 之上的全双工通信协议，连接建立分为三个阶段。

#### 一、WS（明文）连接建立过程

```
客户端                                          服务器
   │                                                │
   │ ────────── TCP 三次握手 ─────────────────→      │
   │   SYN → SYN+ACK → ACK                             │
   │                                                │
   │ ═══════════════════════════════════════════     │
   │         TCP 连接建立完成                           │
   │ ═══════════════════════════════════════════     │
   │                                                │
   │ ──── HTTP GET 握手请求 ─────────────────→       │
   │     GET /chat HTTP/1.1                            │
   │     Upgrade: websocket                           │
   │     Connection: Upgrade                          │
   │     Sec-WebSocket-Key: xxx                       │
   │                                                │
   │ ←─── HTTP 101 握手响应 ─────────────────         │
   │     HTTP/1.1 101 Switching Protocols             │
   │     Sec-WebSocket-Accept: yyy                    │
   │                                                │
   │ ═══════════════════════════════════════════     │
   │      WebSocket 连接建立完成，可双向通信              │
   │ ═══════════════════════════════════════════     │
```

#### 二、WSS（加密）连接建立过程

```
客户端                                          服务器
   │                                                │
   │ ────────── TCP 三次握手 ─────────────────→      │
   │                                                │
   │ ═══════════════════════════════════════════     │
   │         TCP 连接建立完成                           │
   │ ═══════════════════════════════════════════     │
   │                                                │
   │ ────────── TLS 握手 ───────────────────→       │
   │   Client Hello → Server Hello                    │
   │   ← Certificate → Key Exchange                   │
   │   Change Cipher Spec ↔ Finished                  │
   │                                                │
   │ ═══════════════════════════════════════════     │
   │      TLS 加密通道建立完成                           │
   │ ═══════════════════════════════════════════     │
   │                                                │
   │ ╔══════════════════════════════════════════╗     │
   │ ║  加密的 HTTP WebSocket 握手（通过 TLS 通道）     ║     │
   │ ╚══════════════════════════════════════════╝     │
   │                                                │
   │ ──── GET /chat (加密) ─────────────────→         │
   │ ←─── HTTP 101 (加密) ──────────────────          │
   │                                                │
   │ ═══════════════════════════════════════════     │
   │    WebSocket 连接建立完成（加密双向通信）            │
   │ ═══════════════════════════════════════════     │
```

#### 三、WebSocket 握手关键点

| 阶段 | 关键字段/状态码 | 说明 |
|------|----------------|------|
| **TCP 握手** | SYN, SYN+ACK, ACK | 建立可靠连接 |
| **TLS 握手**（WSS） | ClientHello, Certificate | 建立加密通道 |
| **WebSocket 握手** | `Upgrade: websocket` | 请求协议升级 |
| **握手完成** | HTTP 101 | 协议切换成功 |

#### 四、WebSocket 帧格式

```
 0                   1                   2                   3
 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
+-+-+-+-+-------+-+-------------+-------------------------------+
|F|R|R|R| opcode|M| Payload len |    Extended payload length    |
|I|S|S|S|  (4)  |A|     (7)     |             (16/64)           |
|N|V|V|V|       |S|             |   (if payload len==126/127)   |
| |1|2|3|       |K|             |                               |
+-+-+-+-+-------+-+-------------+ - - - - - - - - - - - - - - - +
|     Extended payload length continued, if payload len == 127 |
+ - - - - - - - - - - - - - - - +-------------------------------+
|                               |Masking-key, if MASK set to 1  |
+-------------------------------+-------------------------------+
| Masking-key (continued)       |          Payload Data         |
+-------------------------------- - - - - - - - - - - - - - - - +
:                     Payload Data continued ...                :
+---------------------------------------------------------------+
```

| 字段 | 说明 |
|------|------|
| **FIN** | 是否最后一帧 |
| **Opcode** | 帧类型（0x1=文本, 0x2=二进制, 0x8=关闭） |
| **MASK** | 客户端发送必须为 1 |
| **Payload len** | 载荷长度 |
| **Payload data** | 实际数据 |

#### 五、WS vs WSS 对比

| 特性 | WS | WSS |
|------|-----|-----|
| **URL** | `ws://` | `wss://` |
| **默认端口** | 80 | 443 |
| **安全性** | 明文传输 | TLS 加密 |
| **握手过程** | TCP + HTTP Upgrade | TCP + TLS + HTTP Upgrade |
| **应用场景** | 内网、测试环境 | 公网、生产环境 |

#### 六、面试高频问题

| 问题 | 答案 |
|------|------|
| **为什么 TCP 需要三次握手？** | 防止失效连接、同步序列号 |
| **HTTP 101 状态码表示什么？** | Switching Protocols，协议切换成功 |
| **WebSocket 能通过 HTTP 代理吗？** | 可以，这就是为什么先用 HTTP 握手 |
| **为什么客户端必须掩码？** | 防止缓存投毒攻击 |
| **WSS 的 TLS 握手在哪一步？** | TCP 连接后、WebSocket 握手前 |
| **心跳机制如何实现？** | Ping/Pong 帧（Opcode 0x9/0xA） |

**关键记忆点：**
```
WS 连接：  TCP → HTTP Upgrade → WebSocket
WSS 连接： TCP → TLS → HTTP Upgrade(加密) → WebSocket(加密)
状态码 101：协议切换成功的关键标识
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
- [ ] 理解菱形继承和虚继承原理
- [ ] 了解 Windows .lib 和 Linux .a/.so 的区别
- [ ] 理解 WebSocket/WSS 连接建立过程
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
