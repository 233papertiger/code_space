# C++11 OOP 关键字学习指南

本目录包含一系列完整的 C++11 面向对象编程关键字学习示例，帮助深入理解现代 C++ 的 OOP 特性。

## 📁 文件结构

### OOP 核心关键字

| 文件 | 描述 | 主要内容 |
|------|------|----------|
| [`01_override_final.cpp`](01_override_final.cpp) | override 和 final | 虚函数重写控制、类继承控制、设计模式应用 |
| [`02_protected_member.cpp`](02_protected_member.cpp) | protected 成员变量 | protected 与 public/private 的区别、访问规则、使用场景 |
| [`03_protected_inheritance.cpp`](03_protected_inheritance.cpp) | protected 继承 | 三种继承方式对比、权限传播、实际应用场景 |

### 现代 C++ 特性

| 文件 | 描述 | 主要内容 |
|------|------|----------|
| [`04_noexcept.cpp`](04_noexcept.cpp) | noexcept 异常规范 | 异常规范、性能优化、vector 扩容优化 |
| [`05_constexpr.cpp`](05_constexpr.cpp) | constexpr 编译期计算 | 编译期常量、编译期函数、模板元编程、查找表 |
| [`06_default_delete.cpp`](06_default_delete.cpp) | =default 和 =delete | 特殊成员函数控制、Rule of Five、单例模式 |
| [`07_decltype_auto.cpp`](07_decltype_auto.cpp) | decltype 和 auto | 类型推导、尾置返回类型、完美转发、Lambda |

### 现代 C++ 核心特性

| 文件 | 描述 | 主要内容 |
|------|------|----------|
| [`08_smart_pointers.cpp`](08_smart_pointers.cpp) | 智能指针 | unique_ptr、shared_ptr、weak_ptr、RAII、内存管理 |
| [`09_move_semantics.cpp`](09_move_semantics.cpp) | 移动语义 | 左值/右值、右值引用、移动构造、std::move、完美转发 |
| [`10_lambda.cpp`](10_lambda.cpp) | Lambda 表达式 | Lambda 语法、捕获列表、STL 算法、std::function |

## 🚀 快速开始

### 编译运行

```bash
cd cpp11_oop

# 编译单个文件
g++ -std=c++11 01_override_final.cpp -o override_final
./override_final

# 编译所有文件
for f in *.cpp; do
    g++ -std=c++11 "$f" -o "${f%.cpp}"
done
```

## 📚 学习路径

### 第一阶段：OOP 基础强化

从 [`01_override_final.cpp`](01_override_final.cpp) 开始，掌握：
- `override` 关键字如何防止意外创建新函数
- `final` 如何阻止类被继承或函数被重写
- 虚函数表的底层实现原理

### 第二阶段：访问控制深入

学习 [`02_protected_member.cpp`](02_protected_member.cpp) 和 [`03_protected_inheritance.cpp`](03_protected_inheritance.cpp)：
- 三种访问修饰符的区别
- protected 在成员变量和继承中的不同表现
- 何时使用 public/protected/private 继承

### 第三阶段：现代 C++ 核心特性

掌握现代 C++ 的关键特性：
- [`04_noexcept.cpp`](04_noexcept.cpp) - 异常规范和性能优化
- [`05_constexpr.cpp`](05_constexpr.cpp) - 编译期计算
- [`06_default_delete.cpp`](06_default_delete.cpp) - 特殊成员函数控制
- [`07_decltype_auto.cpp`](07_decltype_auto.cpp) - 类型推导

### 第四阶段：现代 C++ 三大支柱（必学）

- [`08_smart_pointers.cpp`](08_smart_pointers.cpp) - 智能指针（现代 C++ 内存管理核心）
- [`09_move_semantics.cpp`](09_move_semantics.cpp) - 移动语义（性能优化的关键）
- [`10_lambda.cpp`](10_lambda.cpp) - Lambda 表达式（函数式编程基础）

## 🎯 核心概念速查

### override 关键字

```cpp
class Base {
public:
    virtual void func() = 0;
};

class Derived : public Base {
public:
    // ✓ 使用 override 让编译器检查签名
    void func() override {
        // ...
    }

    // ✗ 错误：函数签名不匹配会被编译器发现
    // void func() const override { }
};
```

### final 关键字

```cpp
// 阻止函数被重写
class Base {
public:
    virtual void func() final { }  // 派生类不能重写
};

// 阻止类被继承
class FinalClass final {
    // 不能被继承
};

// ✗ 编译错误
// class Derived : public FinalClass { };
```

### protected 访问

```cpp
class Base {
protected:
    int protectedVar;  // 派生类可访问
};

class Derived : public Base {
public:
    void demo() {
        protectedVar = 10;  // ✓ 可以访问
    }
};

// 外部不能访问
// Base b;
// b.protectedVar;  // ✗ 编译错误
```

### noexcept

```cpp
// 承诺不抛出异常
int safeFunction() noexcept {
    return 42;
}

// 条件 noexcept
template<typename T>
void conditionalNoexcept(const T& t) noexcept(noexcept(t.toString())) {
    // ...
}
```

### constexpr

```cpp
// 编译期常量
constexpr int square(int x) {
    return x * x;
}

// 编译期计算
constexpr int result = square(5);  // 编译期计算为 25

// 用于数组大小
int arr[square(4)];  // 长度为 16
```

### =default 和 =delete

```cpp
class Example {
public:
    // 使用默认构造
    Example() = default;

    // 禁止拷贝
    Example(const Example&) = delete;
    Example& operator=(const Example&) = delete;

    // 禁止特定类型转换
    Example(double) = delete;
};
```

### decltype 和 auto

```cpp
// auto - 类型推导（忽略引用和 const）
auto x = 42;  // int
auto& y = x;  // int& (保留引用)

// decltype - 保留完整类型
decltype(x) z = x;  // int

// 尾置返回类型
template<typename T, typename U>
auto add(T t, U u) -> decltype(t + u) {
    return t + u;
}

// decltype(auto) (C++14)
decltype(auto) getValue() {
    return someValue;  // 保留引用和 const
}
```

### 智能指针

```cpp
// unique_ptr - 独占所有权
std::unique_ptr<MyClass> p = std::make_unique<MyClass>();

// shared_ptr - 共享所有权
std::shared_ptr<MyClass> p1 = std::make_shared<MyClass>();
std::shared_ptr<MyClass> p2 = p1;  // 引用计数 +1

// weak_ptr - 弱引用（解决循环引用）
std::weak_ptr<MyClass> weak = p1;
if (auto locked = weak.lock()) {
    // 使用 locked...
}
```

### 移动语义

```cpp
// 左值与右值
int x = 10;        // x 是左值，10 是右值
int&& rref = 10;   // 右值引用

// std::move - 将左值转换为右值引用
std::string str1 = "Hello";
std::string str2 = std::move(str1);  // 移动而非拷贝

// 移动构造函数
class MyClass {
public:
    MyClass(MyClass&& other) noexcept
        : data(std::move(other.data)) {
        // 窃取资源
    }
};
```

### Lambda 表达式

```cpp
// 基本语法
[capture](params) -> return_type { body }

// 捕获列表示例
[]() { }              // 不捕获
[x]() { return x + 1; }  // 按值捕获 x
[&x]() { x++; }       // 按引用捕获 x
[=]() { }             // 按值捕获所有
[&]() { }             // 按引用捕获所有

// 与 STL 算法配合
std::vector<int> v = {1, 2, 3, 4, 5};
std::for_each(v.begin(), v.end(), [](int n) {
    std::cout << n << " ";
});
```

## 🔑 访问权限对照表

### 类成员访问权限

| 访问位置 | public | protected | private |
|---------|--------|-----------|---------|
| 类内部 | ✓ | ✓ | ✓ |
| 派生类 | ✓ | ✓ | ✗ |
| 外部 | ✓ | ✗ | ✗ |

### 继承方式对基类成员的影响

| 基类成员 | public 继承 | protected 继承 | private 继承 |
|---------|------------|---------------|-------------|
| public 成员 | public | protected | private |
| protected 成员 | protected | protected | private |
| private 成员 | 不可访问 | 不可访问 | 不可访问 |

## 💡 最佳实践

### 1. 虚函数声明

```cpp
// ✓ 推荐：多态基类使用虚析构函数
class Base {
public:
    virtual ~Base() = default;
    virtual void func() = 0;
};

// ✓ 推荐：使用 override 明确重写意图
class Derived : public Base {
public:
    void func() override {
        // 实现
    }
};
```

### 2. 访问控制选择

```cpp
// 默认使用 private
class MyClass {
private:
    int data;  // 私有数据

public:
    void setData(int d) {
        if (d > 0)  // 验证数据
            data = d;
    }
};

// 需要派生类访问时使用 protected
class Base {
protected:
    void helperFunction() { }  // 供派生类使用
};

// 对外接口使用 public
class Interface {
public:
    void publicMethod() { }  // 公共接口
};
```

### 3. 移动语义

```cpp
// 移动构造函数应该是 noexcept
class MyClass {
public:
    MyClass(MyClass&& other) noexcept
        : data(std::exchange(other.data, nullptr)) { }
};
```

### 4. Rule of Zero

```cpp
// 优先使用智能指针，避免手动资源管理
class ModernClass {
private:
    std::vector<int> data;        // 自动管理
    std::unique_ptr<int[]> array; // 自动释放

public:
    // 不需要声明特殊成员函数
    // 编译器生成的版本就是正确的
};
```

## 🎓 面试要点

### 常见问题

1. **override 的作用？**
   - 明确表示函数重写虚函数
   - 让编译器检查函数签名匹配
   - 提高代码可读性

2. **final 的两种用法？**
   - `final` 类：不能被继承
   - `final` 函数：不能被重写

3. **protected 和 private 的区别？**
   - protected：派生类可访问
   - private：只有本类可访问

4. **noexcept 的好处？**
   - 编译器优化机会
   - 标识接口不会抛异常
   - 异常发生时直接 terminate

5. **constexpr vs const？**
   - const：运行期常量
   - constexpr：编译期常量（可能）

6. **为什么要用 =delete？**
   - 禁止特定函数（如拷贝）
   - 禁止特定类型转换
   - 禁止堆分配

7. **shared_ptr 的循环引用问题？**
   - 两个对象互相持有 shared_ptr，引用计数永远不会归零
   - 解决方法：使用 weak_ptr 打破循环

8. **unique_ptr 和 shared_ptr 的区别？**
   - unique_ptr：独占所有权，不可拷贝，性能好
   - shared_ptr：共享所有权，可拷贝，有引用计数开销

9. **左值和右值的区别？**
   - 左值：有名字，可取地址，持久存在
   - 右值：临时对象，即将销毁，不能取地址

10. **std::move 做了什么？**
    - 不移动任何东西
    - 将左值强制转换为右值引用
    - 触发移动构造/移动赋值

11. **Lambda 的捕获列表？**
    - `[x]`：按值捕获 x
    - `[&x]`：按引用捕获 x
    - `[=]`：按值捕获所有
    - `[&]`：按引用捕获所有
    - `[this]`：捕获 this 指针

## 📚 参考资料

- **书籍**
  - 《C++ Primer》- 基础入门
  - 《Effective C++》- 进阶实践
  - 《Inside the C++ Object Model》- 底层原理

- **在线资源**
  - [cppreference](https://en.cppreference.com/w/)
  - [C++ Reference](https://www.cplusplus.com/)

## 📝 编译说明

- 所有代码使用 **C++11** 标准编写
- 某些特性（如 `decltype(auto)`）需要 C++14，已在注释中说明
- 推荐编译选项：`-std=c++11 -Wall -Wextra`

---

**Happy Coding! 🎉**
