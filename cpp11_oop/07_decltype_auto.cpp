/**
 * C++11 decltype 和 auto 关键字详解
 *
 * 学习目标：
 * 1. 理解 decltype 和 auto 的区别与联系
 * 2. 掌握类型推导的规则和应用场景
 * 3. 理解 decltype(auto) 和返回类型推导
 */

#include <iostream>
#include <vector>
#include <string>
#include <typeinfo>
using namespace std;

// ==================== 1. auto 关键字 ====================

/*
 * auto 的作用：
 * 1. 自动推导变量类型
 * 2. 简化代码，提高可读性
 * 3. 处理复杂类型（如迭代器）
 */

void demoAutoBasic() {
    cout << "=== auto 基本用法 ===" << endl;

    // 基本类型推导
    auto i = 42;           // int
    auto d = 3.14;         // double
    auto s = string("hello");  // string

    cout << "i: " << i << " (type: int)" << endl;
    cout << "d: " << d << " (type: double)" << endl;
    cout << "s: " << s << " (type: string)" << endl;

    // 引用和 const
    int x = 10;
    const int cx = x;
    int& rx = x;

    auto a1 = x;   // int（忽略引用）
    auto a2 = cx;  // int（忽略 const）
    auto a3 = rx;  // int（忽略引用）

    cout << "\na1 类型: int" << endl;
    cout << "a2 类型: int (const 被忽略)" << endl;
    cout << "a3 类型: int (引用被忽略)" << endl;

    // 保留引用和 const
    auto& a4 = x;   // int&
    auto& a5 = cx;  // const int&
    const auto& a6 = x;  // const int&

    cout << "a4: int& (保留引用)" << endl;
    cout << "a5: const int& (保留 const 和引用)" << endl;
}

// ==================== 2. decltype 关键字 ====================

/*
 * decltype 的作用：
 * 1. 推导表达式的类型
 * 2. 保留引用和 const 限定符
 * 3. 用于泛型编程和模板
 */

void demoDecltypeBasic() {
    cout << "\n=== decltype 基本用法 ===" << endl;

    int x = 10;
    const int cx = x;
    int& rx = x;

    // decltype 推导
    decltype(x) d1 = x;     // int
    decltype(cx) d2 = cx;   // const int
    decltype(rx) d3 = x;    // int& (引用被保留！)

    cout << "d1 类型: int" << endl;
    cout << "d2 类型: const int (保留 const)" << endl;
    cout << "d3 类型: int& (保留引用)" << endl;

    // decltype vs auto 的关键区别
    auto a = rx;      // int (引用被忽略)
    decltype(rx) d = rx;  // int& (引用被保留)

    cout << "\na 类型: int (auto 忽略引用)" << endl;
    cout << "d 类型: int& (decltype 保留引用)" << endl;
}

// ==================== 3. auto 和 decltype 对比 ====================

struct Widget {
    int x = 42;
};

Widget getWidget() {
    return Widget{};
}

const Widget& getConstWidgetRef() {
    static Widget w;
    return w;
}

void demoAutoVsDecltype() {
    cout << "\n=== auto vs decltype 对比 ===" << endl;

    // 场景 1：函数返回值
    auto w1 = getWidget();           // Widget (拷贝)
    decltype(getWidget()) w2 = getWidget(); // Widget (拷贝)

    auto w3 = getConstWidgetRef();           // Widget (拷贝，引用和 const 被忽略)
    decltype(getConstWidgetRef()) w4 = getConstWidgetRef(); // const Widget& (保留引用和 const)

    cout << "w1: Widget (auto 拷贝)" << endl;
    cout << "w3: Widget (auto 拷贝，忽略引用和 const)" << endl;
    cout << "w4: const Widget& (decltype(auto) 保留引用和 const)" << endl;
}

// ==================== 4. decltype 用于表达式 ====================

void demoDecltypeExpression() {
    cout << "\n=== decltype 用于表达式 ===" << endl;

    int x = 10;
    int y = 20;

    // 推导表达式的结果类型
    decltype(x + y) sum = x + y;  // int
    decltype(x * 1.0) product = x * 1.0;  // double (int * double)

    cout << "sum: " << sum << " (int)" << endl;
    cout << "product: " << product << " (double)" << endl;

    // 推解引用的类型
    int* p = &x;
    decltype(*p) ref = x;  // int& (解引用得到引用)

    cout << "ref: " << ref << " (int&)" << endl;
    ref = 100;
    cout << "修改后 x: " << x << endl;
}

// ==================== 5. 尾置返回类型 ====================

/*
 * C++11 引入尾置返回类型语法：
 * - 允许使用 decltype 推导返回类型
 * - 特别适用于模板函数
 */

// 传统写法（难以表达）
// template<typename T, typename U>
// ??? add(T t, U u) { return t + u; }  // 返回类型是什么？

// 尾置返回类型
template<typename T, typename U>
auto add(T t, U u) -> decltype(t + u) {
    return t + u;
}

void demoTrailingReturnType() {
    cout << "\n=== 尾置返回类型 ===" << endl;

    auto result1 = add(10, 20);         // int
    auto result2 = add(1.5, 2);         // double
    auto result3 = add(string("a"), string("b"));  // string

    cout << "add(10, 20) = " << result1 << " (int)" << endl;
    cout << "add(1.5, 2) = " << result2 << " (double)" << endl;
    cout << "add(\"a\", \"b\") = " << result3 << " (string)" << endl;
}

// ==================== 6. decltype(auto) - C++14 ====================

/*
 * decltype(auto) (C++14)：
 * - 使用 decltype 规则推导 auto
 * - 保留引用和 const
 */

// C++11 方式
template<typename T>
auto getRef11(T& t) -> decltype(t) {
    return t;  // 返回 T&
}

// C++14 decltype(auto) 方式
#if __cplusplus >= 201402L
decltype(auto) getRef14(auto& t) {
    return t;  // 返回 decltype(t) 类型
}
#endif

void demoDecltypeAuto() {
    cout << "\n=== decltype(auto) ===" << endl;

    int x = 42;
    const int cx = x;

    auto a1 = x;       // int
    decltype(x) a2 = x;  // int

    auto a3 = cx;      // int (const 被忽略)
    decltype(cx) a4 = cx;  // const int (保留 const)

    cout << "a3: int (auto 忽略 const)" << endl;
    cout << "a4: const int (decltype(auto) 保留 const)" << endl;
}

// ==================== 7. 实际应用：完美转发 ====================

#include <utility>

void process(int& x) {
    cout << "处理左值引用: " << x << endl;
}

void process(int&& x) {
    cout << "处理右值引用: " << x << endl;
}

template<typename T>
void perfectForward(T&& arg) {
    // std::forward 依赖于 decltype
    process(std::forward<T>(arg));
}

void demoPerfectForward() {
    cout << "\n=== 完美转发示例 ===" << endl;

    int x = 10;
    perfectForward(x);   // 传递左值
    perfectForward(42);  // 传递右值
}

// ==================== 8. 实际应用：容器迭代器 ====================

void demoContainerIteration() {
    cout << "\n=== 容器迭代与类型推导 ===" << endl;

    vector<int> vec = {1, 2, 3, 4, 5};

    // 传统写法（冗长）
    // vector<int>::iterator it = vec.begin();

    // auto 写法（简洁）
    auto it = vec.begin();

    // 基于范围的 for 循环 + auto
    cout << "遍历 vector: ";
    for (auto& elem : vec) {  // 使用引用避免拷贝
        elem *= 2;
    }

    for (const auto& elem : vec) {
        cout << elem << " ";
    }
    cout << endl;
}

// ==================== 9. 函数指针与 auto ====================

int addFunc(int a, int b) { return a + b; }
int mulFunc(int a, int b) { return a * b; }

void demoFunctionPointer() {
    cout << "\n=== 函数指针与 auto ===" << endl;

    // 传统写法
    int (*fp1)(int, int) = addFunc;

    // auto 写法
    auto fp2 = mulFunc;

    cout << "fp1(3, 4) = " << fp1(3, 4) << endl;
    cout << "fp2(3, 4) = " << fp2(3, 4) << endl;

    // decltype 获取函数类型
    decltype(addFunc) *fp3 = addFunc;
    cout << "fp3(5, 6) = " << fp3(5, 6) << endl;
}

// ==================== 10. Lambda 与 auto ====================

void demoLambda() {
    cout << "\n=== Lambda 与 auto ===" << endl;

    // lambda 的类型是唯一的，无法直接写出
    auto lambda = [](int x) { return x * x; };

    cout << "lambda(5) = " << lambda(5) << endl;

    // decltype 获取 lambda 类型
    decltype(lambda) sameLambda = lambda;
    cout << "sameLambda(6) = " << sameLambda(6) << endl;
}

// ==================== 主函数测试 ====================

int main() {
    cout << "==================== C++11 auto 和 decltype 详解 ====================" << endl;

    demoAutoBasic();
    demoDecltypeBasic();
    demoAutoVsDecltype();
    demoDecltypeExpression();
    demoTrailingReturnType();
    demoDecltypeAuto();
    demoPerfectForward();
    demoContainerIteration();
    demoFunctionPointer();
    demoLambda();

    cout << "\n==================== 总结 ====================" << endl;
    cout << R"(
auto 和 decltype 总结：

1. auto 关键字：
   - 自动推导变量类型
   - 忽略引用和 const 限定符
   - 使用 auto&/const auto& 保留引用/const
   - 简化复杂类型的声明

2. decltype 关键字：
   - 推导表达式的确切类型
   - 保留引用和 const 限定符
   - 用于尾置返回类型
   - 用于模板元编程

3. 对比：
   特性              | auto         | decltype
   ------------------|-------------|-------------
   推导依据          | 初始化表达式 | 任意表达式
   保留引用          | 否           | 是
   保留 const        | 否           | 是
   主要用途          | 变量声明     | 类型推导

4. decltype(auto) (C++14):
   - 使用 decltype 规则推导 auto
   - 完美保留引用和 const

5. 实际应用：
   - 简化迭代器类型
   - Lambda 表达式
   - 函数指针
   - 尾置返回类型
   - 完美转发
   - 模板元编程

6. 最佳实践：
   - 局部变量优先使用 auto
   - 需要保留引用时使用 auto&
   - 复杂表达式使用 decltype
   - 模板返回值使用尾置返回类型或 decltype(auto)
)" << endl;

    return 0;
}

/*
 * 编译运行：
 * g++ -std=c++11 07_decltype_auto.cpp -o decltype_auto
 * ./decltype_auto
 *
 * 注：decltype(auto) 需要 C++14，使用 -std=c++14 编译
 */
