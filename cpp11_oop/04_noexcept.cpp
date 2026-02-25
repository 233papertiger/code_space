/**
 * C++11 noexcept 关键字详解
 *
 * 学习目标：
 * 1. 理解 noexcept 的作用和意义
 * 2. 掌握 noexcept 的正确使用方式
 * 3. 理解 noexcept 对性能优化和代码设计的帮助
 */

#include <iostream>
#include <vector>
#include <stdexcept>
using namespace std;

// ==================== 1. 基本语法 ====================

/*
 * noexcept 关键字有两种形式：
 * 1. noexcept - 声明函数不会抛出异常
 * 2. noexcept(condition) - 条件 noexcept，根据条件决定
 */

// 普通函数，可能抛出异常
int mayThrow(int x) {
    if (x < 0) {
        throw runtime_error("Negative value");
    }
    return x * 2;
}

// noexcept 函数，承诺不抛出异常
int noThrow(int x) noexcept {
    return x * 2;  // 如果这里抛出异常，程序会调用 std::terminate
}

// 条件 noexcept
template<typename T>
void conditionalNoexcept(const T& t) noexcept(noexcept(t.toString())) {
    // 如果 t.toString() 不抛异常，这个函数也不抛异常
    // 常用于模板元编程
}

// ==================== 2. 为什么需要 noexcept？ ====================

/*
 * noexcept 的好处：
 * 1. 性能优化：编译器可以生成更高效的代码
 * 2. 标准库优化：某些容器操作（如 vector::push_back）在移动构造函数是
 *    noexcept 时会更高效
 * 3. 接口清晰：明确告诉调用者这个函数不会抛异常
 * 4. 错误处理：异常发生时直接 terminate，避免未定义行为
 */

// ==================== 3. noexcept 与移动语义 ====================

class MayThrowClass {
public:
    MayThrowClass() = default;
    MayThrowClass(const MayThrowClass&) { cout << "Copy constructor" << endl; }
    MayThrowClass(MayThrowClass&&) noexcept(false) { cout << "Move constructor (may throw)" << endl; }
};

class NoThrowClass {
public:
    NoThrowClass() = default;
    NoThrowClass(const NoThrowClass&) { cout << "Copy constructor" << endl; }
    NoThrowClass(NoThrowClass&&) noexcept { cout << "Move constructor (noexcept)" << endl; }
};

void demoVectorReallocation() {
    cout << "=== vector 扩容时的行为差异 ===" << endl;

    cout << "\n--- 使用 may throw 的移动构造 ---" << endl;
    vector<MayThrowClass> v1;
    v1.reserve(3);
    v1.emplace_back();
    v1.emplace_back();
    cout << "触发扩容（会使用拷贝而非移动，因为移动可能抛异常）：" << endl;
    v1.emplace_back();  // 触发扩容

    cout << "\n--- 使用 noexcept 的移动构造 ---" << endl;
    vector<NoThrowClass> v2;
    v2.reserve(3);
    v2.emplace_back();
    v2.emplace_back();
    cout << "触发扩容（会使用移动，更高效）：" << endl;
    v2.emplace_back();  // 触发扩容
}

// ==================== 4. noexcept 函数中的异常处理 ====================

/*
 * 如果 noexcept 函数真的抛出了异常：
 * 1. 不会正常传播异常
 * 2. 直接调用 std::terminate()
 * 3. 程序立即终止
 */

void dangerousNoThrow() noexcept {
    throw runtime_error("Oops!");  // 危险！会导致 terminate
}

// 正确的做法：在 noexcept 函数中捕获所有可能的异常
void safeNoThrow() noexcept {
    try {
        // 可能抛异常的代码
        if (false) {  // 模拟条件
            throw runtime_error("Something went wrong");
        }
    } catch (...) {
        // 捕获所有异常，不让它传播出去
        // 可以记录日志或进行清理
        cout << "Exception caught in noexcept function" << endl;
    }
}

// ==================== 5. noexcept 作为操作符 ====================

void throwsException() {
    throw runtime_error("Exception");
}

void doesNotThrow() noexcept {}

void demoNoexceptOperator() {
    cout << "\n=== noexcept 操作符 ===" << endl;

    // noexcept 可以作为表达式，编译期判断
    cout << "throwsException() 是否 noexcept: " << noexcept(throwsException()) << endl;
    cout << "doesNotThrow() 是否 noexcept: " << noexcept(doesNotThrow()) << endl;

    // 用于模板条件判断
    auto lambda = []() noexcept { return 42; };
    cout << "lambda 是否 noexcept: " << noexcept(lambda()) << endl;
}

// ==================== 6. 实际应用：智能指针和资源管理 ====================

class Resource {
private:
    int* data;
public:
    Resource(int size) : data(new int[size]) { cout << "Resource acquired" << endl; }
    ~Resource() noexcept { delete[] data; cout << "Resource released" << endl; }

    // 析构函数默认是 noexcept（C++11 后隐式）
    // 明确声明 noexcept 是好习惯

    // 移动构造应该是 noexcept
    Resource(Resource&& other) noexcept : data(other.data) {
        other.data = nullptr;
    }
};

// ==================== 7. 什么时候应该使用 noexcept？ ====================

/*
 * 应该使用 noexcept 的情况：
 * 1. 移动构造函数和移动赋值运算符（如果可能的话）
 * 2. swap 函数（标准库要求）
 * 3. delete 操作符
 * 4. 析构函数（C++11 后默认就是）
 * 5. 简单的 getter 函数
 *
 * 不应该使用 noexcept 的情况：
 * 1. 函数内部可能抛出异常且无法处理
 * 2. 需要向调用者报告错误
 */

// 示例：swap 函数必须 noexcept
class MyBuffer {
    int* data;
    size_t size;

public:
    MyBuffer(size_t s) : data(new int[s]), size(s) {}
    ~MyBuffer() noexcept { delete[] data; }

    // swap 函数应该是 noexcept
    friend void swap(MyBuffer& a, MyBuffer& b) noexcept {
        using std::swap;
        swap(a.data, b.data);
        swap(a.size, b.size);
    }
};

// ==================== 8. 条件 noexcept 的实际应用 ====================

template<typename T>
class Container {
    T* items;
    size_t count;

public:
    Container(size_t n) : items(new T[n]), count(n) {}
    ~Container() noexcept(noexcept(std::declval<T>().~T())) {
        delete[] items;
    }

    // 移动构造函数的条件 noexcept
    Container(Container&& other) noexcept(noexcept(std::declval<T&>() = std::move(std::declval<T&>())))
        : items(other.items), count(other.count) {
        other.items = nullptr;
        other.count = 0;
    }
};

// ==================== 主函数测试 ====================

int main() {
    cout << "==================== noexcept 基础 ====================" << endl;

    cout << "调用 mayThrow(5): " << mayThrow(5) << endl;

    try {
        cout << "调用 mayThrow(-1): ";
        mayThrow(-1);
    } catch (const exception& e) {
        cout << "Caught: " << e.what() << endl;
    }

    cout << "\n调用 noThrow(5): " << noThrow(5) << endl;

    cout << "\n==================== 异常处理对比 ====================" << endl;

    safeNoThrow();  // 正常运行

    cout << "\n==================== vector 扩容优化 ====================" << endl;
    demoVectorReallocation();

    cout << "\n==================== noexcept 操作符 ====================" << endl;
    demoNoexceptOperator();

    cout << "\n==================== 总结 ====================" << endl;
    cout << R"(
noexcept 关键字总结：

1. 基本语法：
   - noexcept: 声明函数不抛异常
   - noexcept(expr): 条件 noexcept
   - noexcept(expr): 操作符，判断表达式是否不抛异常

2. 性能优势：
   - 编译器可以生成更优化的代码
   - std::vector 扩容时优先使用移动而非拷贝

3. 使用建议：
   - 移动构造/赋值、swap、析构函数应该用 noexcept
   - 简单的 getter 可以用 noexcept
   - 不确定时不要使用（除非你能保证不抛异常）

4. 注意事项：
   - noexcept 函数抛异常会导致 std::terminate
   - 在 noexcept 函数中要捕获所有可能的异常
   - C++11 后析构函数默认 noexcept
)" << endl;

    return 0;
}

/*
 * 编译运行：
 * g++ -std=c++11 04_noexcept.cpp -o noexcept
 * ./noexcept
 *
 * 注意：如果取消注释 dangerousNoThrow() 的调用，程序会直接终止
 */
