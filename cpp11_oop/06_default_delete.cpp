/**
 * C++11 =default 和 =delete 详解
 *
 * 学习目标：
 * 1. 理解 =default 和 =delete 的作用
 * 2. 掌握何时显式声明默认/删除的特殊成员函数
 * 3. 理解 Rule of Five/Zero 和现代 C++ 类设计
 */

#include <iostream>
#include <memory>
#include <type_traits>
using namespace std;

// ==================== 1. =default 语法 ====================

/*
 * =default 的作用：
 * 1. 显式要求编译器生成默认版本
 * 2. 可以用于构造函数、析构函数、拷贝/移动操作
 * 3. 可以让函数有更好的语义（即使是编译器生成的）
 */

class DefaultDemo {
public:
    // 显式要求编译器生成默认构造函数
    DefaultDemo() = default;

    // 显式要求编译器生成拷贝构造函数
    DefaultDemo(const DefaultDemo&) = default;

    // 显式要求编译器生成析构函数
    ~DefaultDemo() = default;

    // 拷贝赋值运算符
    DefaultDemo& operator=(const DefaultDemo&) = default;

    // C++11: 移动构造函数和移动赋值运算符
    DefaultDemo(DefaultDemo&&) noexcept = default;
    DefaultDemo& operator=(DefaultDemo&&) noexcept = default;

    int value = 0;
};

void demoDefaultSyntax() {
    cout << "=== =default 语法 ===" << endl;

    DefaultDemo d1;
    cout << "d1.value: " << d1.value << endl;

    // 使用编译器生成的拷贝构造
    DefaultDemo d2 = d1;
    cout << "d2.value (拷贝后): " << d2.value << endl;

    // 使用编译器生成的移动构造
    DefaultDemo d3 = std::move(d1);
    cout << "d3.value (移动后): " << d3.value << endl;
}

// ==================== 2. =delete 语法 ====================

/*
 * =delete 的作用：
 * 1. 禁止编译器生成默认函数
 * 2. 禁止特定类型的转换
 * 3. 禁止特定重载版本
 */

class DeleteDemo {
public:
    // 禁止默认构造
    DeleteDemo() = delete;

    // 允许带参数的构造
    DeleteDemo(int x) : value(x) {}

    // 禁止拷贝
    DeleteDemo(const DeleteDemo&) = delete;
    DeleteDemo& operator=(const DeleteDemo&) = delete;

    // 允许移动
    DeleteDemo(DeleteDemo&&) noexcept = default;
    DeleteDemo& operator=(DeleteDemo&&) noexcept = default;

    int value;
};

void demoDeleteSyntax() {
    cout << "\n=== =delete 语法 ===" << endl;

    // DeleteDemo d1;  // 编译错误：默认构造被删除

    DeleteDemo d2(42);
    cout << "d2.value: " << d2.value << endl;

    // DeleteDemo d3 = d2;  // 编译错误：拷贝构造被删除

    DeleteDemo d4 = std::move(d2);  // OK：移动构造可用
    cout << "d4.value: " << d4.value << endl;
}

// ==================== 3. 禁止特定参数类型 ====================

class TypeSafe {
public:
    TypeSafe(int x) : value(x) {
        cout << "TypeSafe(int) 被调用" << endl;
    }

    // 禁止用 double 构造（避免精度问题）
    TypeSafe(double) = delete;

    // 禁止用浮点数构造
    TypeSafe(float) = delete;

    int value;
};

void demoTypeSafety() {
    cout << "\n=== 禁止特定类型 ===" << endl;

    TypeSafe t1(42);         // OK
    // TypeSafe t2(3.14);   // 编译错误：double 构造被删除
    // TypeSafe t3(2.7f);   // 编译错误：float 构造被删除
}

// ==================== 4. 单例模式（使用 =delete）====================

class Singleton {
public:
    // 删除拷贝构造
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;

    // 删除移动构造
    Singleton(Singleton&&) = delete;
    Singleton& operator=(Singleton&&) = delete;

    static Singleton& getInstance() {
        static Singleton instance;
        return instance;
    }

    void doSomething() {
        cout << "Singleton::doSomething()" << endl;
    }

private:
    Singleton() {
        cout << "Singleton 构造" << endl;
    }
    ~Singleton() = default;
};

void demoSingleton() {
    cout << "\n=== 单例模式 ===" << endl;

    Singleton& s1 = Singleton::getInstance();
    s1.doSomething();

    Singleton& s2 = Singleton::getInstance();
    s2.doSomething();

    cout << "s1 和 s2 是同一个实例: " << (&s1 == &s2) << endl;

    // Singleton s3 = s1;  // 编译错误：拷贝被删除
}

// ==================== 5. Rule of Five/Zero ====================

/*
 * C++11 引入了移动语义，导致了 Rule of Five：
 * 如果你需要定义以下任何一个，应该定义全部：
 * 1. 析构函数
 * 2. 拷贝构造函数
 * 3. 拷贝赋值运算符
 * 4. 移动构造函数
 * 5. 移动赋值运算符
 *
 * 现代建议（Rule of Zero）：
 * - 如果类不需要管理资源，让编译器生成所有特殊成员函数
 * - 使用智能指针管理资源，避免手动管理
 */

// Rule of Zero 示例：使用智能指针
class RuleOfZero {
private:
    std::string name;
    std::unique_ptr<int[]> data;
    size_t size;

public:
    RuleOfZero(const string& n, size_t s)
        : name(n), data(std::unique_ptr<int[]>(new int[s])), size(s) {}

    // 不需要声明任何特殊成员函数！
    // 编译器会生成正确的版本

    void display() const {
        cout << "Name: " << name << ", Size: " << size << endl;
    }
};

// 需要手动管理的示例
class ManagedResource {
private:
    int* data;
    size_t size;

public:
    // 构造函数
    ManagedResource(size_t s) : size(s), data(new int[s]) {
        cout << "分配资源" << endl;
    }

    // 析构函数
    ~ManagedResource() {
        delete[] data;
        cout << "释放资源" << endl;
    }

    // 拷贝构造（深拷贝）
    ManagedResource(const ManagedResource& other)
        : size(other.size), data(new int[other.size]) {
        std::copy(other.data, other.data + size, data);
        cout << "拷贝构造" << endl;
    }

    // 拷贝赋值
    ManagedResource& operator=(const ManagedResource& other) {
        if (this != &other) {
            delete[] data;
            size = other.size;
            data = new int[size];
            std::copy(other.data, other.data + size, data);
        }
        cout << "拷贝赋值" << endl;
        return *this;
    }

    // 移动构造
    ManagedResource(ManagedResource&& other) noexcept
        : data(other.data), size(other.size) {
        other.data = nullptr;
        other.size = 0;
        cout << "移动构造" << endl;
    }

    // 移动赋值
    ManagedResource& operator=(ManagedResource&& other) noexcept {
        if (this != &other) {
            delete[] data;
            data = other.data;
            size = other.size;
            other.data = nullptr;
            other.size = 0;
        }
        cout << "移动赋值" << endl;
        return *this;
    }
};

void demoRules() {
    cout << "\n=== Rule of Zero ===" << endl;
    RuleOfZero rz("test", 10);
    RuleOfZero rz2 = std::move(rz);  // 编译器生成的移动构造
    rz2.display();

    cout << "\n=== Rule of Five ===" << endl;
    ManagedResource mr1(10);
    ManagedResource mr2 = mr1;  // 拷贝
    ManagedResource mr3 = std::move(mr1);  // 移动
}

// ==================== 6. 禁止堆分配 ====================

class StackOnly {
public:
    StackOnly() = default;
    ~StackOnly() = default;

    // 禁止 new 操作符
    void* operator new(size_t) = delete;
    void* operator new[](size_t) = delete;

    void display() {
        cout << "StackOnly 对象" << endl;
    }
};

void demoStackOnly() {
    cout << "\n=== 只能栈分配的对象 ===" << endl;

    StackOnly s1;  // OK：栈分配
    s1.display();

    // StackOnly* p = new StackOnly();  // 编译错误：new 被删除
}

// ==================== 7. 禁止特定运算符重载 ====================

struct NoComparison {
    int value;

    // 禁止比较运算符
    bool operator<(const NoComparison&) = delete;
    bool operator>(const NoComparison&) = delete;
    bool operator==(const NoComparison&) = delete;

    void display() const {
        cout << "Value: " << value << endl;
    }
};

void demoOperatorDelete() {
    cout << "\n=== 禁止特定运算符 ===" << endl;

    NoComparison a{10}, b{20};

    // if (a < b) { }  // 编译错误：< 被删除

    cout << "a: ";
    a.display();
    cout << "b: ";
    b.display();
}

// ==================== 主函数测试 ====================

int main() {
    cout << "==================== C++11 =default 和 =delete 详解 ====================" << endl;

    demoDefaultSyntax();
    demoDeleteSyntax();
    demoTypeSafety();
    demoSingleton();
    demoRules();
    demoStackOnly();
    demoOperatorDelete();

    cout << "\n==================== 总结 ====================" << endl;
    cout << R"(
=default 和 =delete 总结：

1. =default 的使用：
   - 显式要求编译器生成默认实现
   - 用于构造函数、析构函数、拷贝/移动操作
   - 提高代码可读性，明确表达意图

2. =delete 的使用：
   - 禁止编译器生成特定函数
   - 禁止特定类型的转换
   - 禁止拷贝（单例模式）
   - 禁止堆分配（删除 new）

3. Rule of Five：
   - 需要手动管理资源时，实现五个特殊成员函数
   - 析构、拷贝构造、拷贝赋值、移动构造、移动赋值

4. Rule of Zero（推荐）：
   - 使用智能指针等 RAII 类型
   - 让编译器生成所有特殊成员函数
   - 减少手动资源管理

5. 实际应用：
   - 单例模式：删除拷贝和移动
   - 类型安全：删除危险的构造函数
   - 资源控制：禁止堆分配
   - 接口设计：禁止特定操作
)" << endl;

    return 0;
}

/*
 * 编译运行：
 * g++ -std=c++11 06_default_delete.cpp -o default_delete
 * ./default_delete
 */
