/**
 * C++11 移动语义和右值引用详解
 *
 * 学习目标：
 * 1. 理解左值、右值、左值引用、右值引用的概念
 * 2. 理解移动语义的原理和优势
 * 3. 掌握移动构造函数和移动赋值运算符的编写
 * 4. 理解 std::move 和完美转发
 */

#include <iostream>
#include <vector>
#include <string>
#include <utility>
using namespace std;

// ==================== 1. 左值与右值 ====================

/*
 * 基本概念：
 * - 左值（lvalue）：有名字、有持久存储的对象，可以取地址
 * - 右值（rvalue）：临时对象、字面量，即将销毁的对象
 *
 * 记忆方法：
 * - 左值 = 可以出现在赋值号左边（有持久存储）
 * - 右值 = 只能出现在赋值号右边（临时值）
 */

void demoValueCategories() {
    cout << "=== 左值与右值 ===" << endl;

    int x = 10;  // x 是左值，10 是右值

    // 左值：有名字，可以取地址
    cout << "x 是左值，&x = " << &x << endl;

    // 右值：临时对象，字面量
    int y = x + 5;  // x + 5 的结果是右值
    // &(x + 5);    // ✗ 错误：不能对右值取地址

    // 左值引用
    int& lref = x;       // ✓ 左值引用绑定到左值
    // int& lref2 = 10; // ✗ 左值引用不能绑定到右值

    // 右值引用（C++11）
    int&& rref = 10;     // ✓ 右值引用绑定到右值
    int&& rref2 = x + 5; // ✓ 右值引用绑定到右值
    // int&& rref3 = x;  // ✗ 右值引用不能绑定到左值

    cout << endl;
}

// ==================== 2. 左值引用与右值引用 ====================

void takeLValueRef(int& x) {
    cout << "接受左值引用: " << x << endl;
}

void takeRValueRef(int&& x) {
    cout << "接受右值引用: " << x << endl;
}

void demoReferences() {
    cout << "=== 左值引用与右值引用 ===" << endl;

    int a = 10;

    takeLValueRef(a);    // ✓ 左值
    // takeLValueRef(10); // ✗ 右值

    // takeRValueRef(a);  // ✗ 左值
    takeRValueRef(10);   // ✓ 右值
    takeRValueRef(a + 5); // ✓ 右值

    // const 左值引用可以绑定到右值（这是 C++98 就有的特性）
    const int& cref = 10;  // ✓
    cout << "const 左值引用可以绑定到右值: " << cref << endl;

    cout << endl;
}

// ==================== 3. 为什么需要移动语义？ ====================

/*
 * 传统拷贝的问题：
 * - 深拷贝昂贵（尤其是大型容器）
 * - 临时对象拷贝后立即销毁，浪费资源
 *
 * 移动语义的优势：
 * - "窃取"资源而非拷贝
 * - 临时对象反正要销毁，不如复用其资源
 * - 大幅提升性能
 */

class BigData {
private:
    size_t size;
    int* data;

public:
    // 构造函数
    BigData(size_t s) : size(s), data(new int[s]) {
        cout << "构造 BigData，size = " << size << endl;
    }

    // 拷贝构造函数（深拷贝，昂贵）
    BigData(const BigData& other) : size(other.size), data(new int[other.size]) {
        cout << "拷贝构造 BigData，深拷贝 " << size << " 个元素" << endl;
        copy(other.data, other.data + size, data);
    }

    // 移动构造函数（C++11，高效）
    BigData(BigData&& other) noexcept
        : size(other.size), data(other.data) {
        cout << "移动构造 BigData，窃取资源，无需拷贝" << endl;
        other.size = 0;
        other.data = nullptr;
    }

    // 拷贝赋值运算符
    BigData& operator=(const BigData& other) {
        if (this != &other) {
            cout << "拷贝赋值 BigData" << endl;
            delete[] data;
            size = other.size;
            data = new int[size];
            copy(other.data, other.data + size, data);
        }
        return *this;
    }

    // 移动赋值运算符（C++11）
    BigData& operator=(BigData&& other) noexcept {
        if (this != &other) {
            cout << "移动赋值 BigData" << endl;
            delete[] data;
            size = other.size;
            data = other.data;
            other.size = 0;
            other.data = nullptr;
        }
        return *this;
    }

    ~BigData() {
        cout << "析构 BigData";
        if (data) {
            cout << "，释放 " << size << " 个元素的内存";
            delete[] data;
        }
        cout << endl;
    }

    size_t getSize() const { return size; }
};

void demoCopyVsMove() {
    cout << "=== 拷贝 vs 移动 ===" << endl;

    cout << "--- 创建对象 ---" << endl;
    BigData d1(1000);

    cout << "\n--- 拷贝（昂贵）---" << endl;
    BigData d2 = d1;  // 调用拷贝构造

    cout << "\n--- 移动（高效）---" << endl;
    BigData d3 = std::move(d1);  // 调用移动构造

    cout << "\nd1.size = " << d1.getSize() << "（已被移动，现为空）" << endl;
    cout << "d3.size = " << d3.getSize() << endl;

    cout << "\n--- 函数返回值优化 ---" << endl;
    // C++11 之前：可能需要拷贝（或 RVO 优化）
    // C++11：自动使用移动构造
    auto createBigData = []() -> BigData {
        BigData temp(500);
        return temp;  // 移动而非拷贝
    };
    BigData d4 = createBigData();

    cout << endl;
}

// ==================== 4. std::move 详解 ====================

/*
 * std::move 的真相：
 * - 不"移动"任何东西
 * - 只是将左值强制转换为右值引用
 * - 让编译器选择移动构造函数而非拷贝构造函数
 */

void demoStdMove() {
    cout << "=== std::move 详解 ===" << endl;

    string str1 = "Hello, World!";

    cout << "str1: " << str1 << endl;

    // 使用 std::move
    string str2 = std::move(str1);  // 移动构造

    cout << "移动后 str1: \"" << str1 << "\"（可能为空或有效但未指定）" << endl;
    cout << "str2: " << str2 << endl;

    // str1 仍然有效，但状态未指定
    // 可以重新赋值
    str1 = "New content";
    cout << "重新赋值后 str1: " << str1 << endl;

    cout << endl;
}

// ==================== 5. 移动语义与容器 ====================

void demoMoveWithContainers() {
    cout << "=== 移动语义与容器 ===" << endl;

    vector<BigData> vec;

    cout << "添加元素到 vector..." << endl;
    vec.reserve(3);

    // 使用 emplace_back（原地构造，避免临时对象）
    cout << "\n--- emplace_back ---" << endl;
    vec.emplace_back(100);  // 直接在 vector 中构造

    // 使用 push_back + 移动
    cout << "\n--- push_back + std::move ---" << endl;
    BigData temp(200);
    vec.push_back(std::move(temp));  // 移动而非拷贝

    cout << "\nvector 容量: " << vec.capacity() << endl;
    cout << "vector 大小: " << vec.size() << endl;

    cout << endl;
}

// ==================== 6. 完美转发 ====================

/*
 * 完美转发（Perfect Forwarding）：
 * - 将参数"原封不动"地转发给另一个函数
 * - 保留参数的值类别（左值/右值）
 * - 使用 std::forward 实现
 */

// 接受左值的重载
void process(int& x) {
    cout << "处理左值引用: " << x << endl;
}

// 接受右值的重载
void process(int&& x) {
    cout << "处理右值引用: " << x << endl;
}

// 不完美的转发（总是左值）
template<typename T>
void imperfectForward(T t) {
    process(t);  // t 总是左值
}

// 完美的转发（保留值类别）
template<typename T>
void perfectForward(T&& t) {
    process(std::forward<T>(t));  // 完美转发
}

void demoPerfectForwarding() {
    cout << "=== 完美转发 ===" << endl;

    int a = 10;

    cout << "--- 不完美的转发 ---" << endl;
    imperfectForward(a);   // a 是左值
    imperfectForward(20);  // 20 是右值，但转发后变成左值

    cout << "\n--- 完美的转发 ---" << endl;
    perfectForward(a);   // 正确转发为左值
    perfectForward(20);  // 正确转发为右值

    cout << endl;
}

// ==================== 7. 万能引用（Universal Reference）====================

/*
 * 万能引用（T&&）：
 * - 可以绑定到左值和右值
 * - 仅在类型推导时有效（模板参数）
 * - 配合 std::forward 实现完美转发
 */

template<typename T>
void universalReference(T&& arg) {  // T&& 是万能引用
    cout << "收到万能引用，类型推导发生" << endl;
}

// 不是万能引用的情况
class MyClass {
public:
    template<typename T>
    void memberFunc(T&& arg) {  // ✓ 这是万能引用
        cout << "成员函数的万能引用" << endl;
    }

    // template<typename T>
    // void memberFunc2(const T&& arg) {  // ✗ 不是万能引用（有 const）
    // }
};

void demoUniversalReference() {
    cout << "=== 万能引用 ===" << endl;

    int a = 10;

    universalReference(a);   // T 推导为 int&，arg 类型为 int&
    universalReference(20);  // T 推导为 int，arg 类型为 int&&

    cout << endl;
}

// ==================== 8. 引用折叠规则 ====================

/*
 * 引用折叠（Reference Collapse）：
 * - 在类型推导中，多层引用会折叠成单个引用
 * - 规则：& + & → &，& + && → &，&& + & → &，&& + && → &&
 * - 简化：只有 && + && 才折叠成 &&，其他都是 &
 */

template<typename T>
struct RefTraits;

// 特化：左值引用
template<typename T>
struct RefTraits<T&> {
    static constexpr bool isLValueRef = true;
    static constexpr bool isRValueRef = false;
    using type = T;
};

// 特化：右值引用
template<typename T>
struct RefTraits<T&&> {
    static constexpr bool isLValueRef = false;
    static constexpr bool isRValueRef = true;
    using type = T;
};

void demoReferenceCollapsing() {
    cout << "=== 引用折叠规则 ===" << endl;

    // 代码层面演示
    typedef int& T1;
    typedef T1&& T2;  // int& && → int&（引用折叠）

    cout << "int& && 折叠为 int&" << endl;

    cout << endl;
}

// ==================== 9. 移动语义的最佳实践 ====================

class Resource {
private:
    string name;
    size_t size;
    int* data;

public:
    // 构造
    Resource(const string& n, size_t s) : name(n), size(s), data(new int[s]) {
        cout << "Resource[" << name << "] 构造" << endl;
    }

    // 析构
    ~Resource() {
        cout << "Resource[" << name << "] 析构";
        if (data) {
            delete[] data;
            cout << "（已释放内存）";
        }
        cout << endl;
    }

    // 移动构造（noexcept 重要！）
    Resource(Resource&& other) noexcept
        : name(std::move(other.name)),
          size(other.size),
          data(other.data) {
        other.size = 0;
        other.data = nullptr;
        cout << "Resource[" << name << "] 移动构造" << endl;
    }

    // 移动赋值（noexcept 重要！）
    Resource& operator=(Resource&& other) noexcept {
        if (this != &other) {
            cout << "Resource[" << name << "] 移动赋值" << endl;
            delete[] data;
            name = std::move(other.name);
            size = other.size;
            data = other.data;
            other.size = 0;
            other.data = nullptr;
        }
        return *this;
    }

    // 禁止拷贝（可选）
    Resource(const Resource&) = delete;
    Resource& operator=(const Resource&) = delete;

    void display() const {
        cout << "Resource[" << name << "], size = " << size << endl;
    }
};

void demoBestPractices() {
    cout << "=== 移动语义最佳实践 ===" << endl;

    Resource r1("Data1", 100);
    Resource r2("Data2", 200);

    cout << "\n--- 移动 r1 到 r3 ---" << endl;
    Resource r3 = std::move(r1);

    cout << "\n--- 移动赋值 ---" << endl;
    r1 = std::move(r2);

    cout << endl;
}

// ==================== 主函数测试 ====================

int main() {
    cout << "==================== C++11 移动语义详解 ====================" << endl;

    demoValueCategories();
    demoReferences();
    demoCopyVsMove();
    demoStdMove();
    demoMoveWithContainers();
    demoPerfectForwarding();
    demoUniversalReference();
    demoReferenceCollapsing();
    demoBestPractices();

    cout << "==================== 总结 ====================" << endl;
    cout << R"(
移动语义总结：

1. 值类别：
   - 左值（lvalue）：有名字，可取地址
   - 右值（rvalue）：临时对象，即将销毁
   - 左值引用（T&）：绑定到左值
   - 右值引用（T&&）：绑定到右值

2. 移动语义：
   - "窃取"资源而非深拷贝
   - 大幅提升性能
   - 用于临时对象和不再需要的对象

3. std::move：
   - 不移动任何东西
   - 将左值转换为右值引用
   - 触发移动构造/移动赋值

4. 完美转发：
   - 使用 std::forward 保留参数的值类别
   - 配合万能引用（T&&）使用
   - 用于编写泛型代码

5. 万能引用：
   - 形式：T&&（类型推导时）
   - 可以绑定到左值和右值
   - 配合 std::forward 使用

6. 引用折叠：
   - && + && → &&
   - 其他组合 → &

7. 最佳实践：
   - 移动构造/赋值应该 noexcept
   - 优先使用移动而非拷贝
   - 使用 std::move 明确表示转移所有权
   - 移动后的对象处于有效但未指定的状态
)" << endl;

    return 0;
}

/*
 * 编译运行：
 * g++ -std=c++11 09_move_semantics.cpp -o move_semantics
 * ./move_semantics
 */
