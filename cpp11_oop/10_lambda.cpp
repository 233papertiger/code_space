/**
 * C++11 Lambda 表达式详解
 *
 * 学习目标：
 * 1. 理解 Lambda 表达式的语法和组成
 * 2. 掌握捕获列表的使用方式
 * 3. 理解 Lambda 与 STL 算法的配合
 * 4. 掌握 Lambda 作为返回值的用法
 */

#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>
#include <string>
#include <map>
using namespace std;

// ==================== 1. Lambda 基本语法 ====================

/*
 * Lambda 表达式语法：
 * [捕获列表](参数列表) mutable 异常说明 -> 返回类型 { 函数体 }
 *
 * 简化形式：
 * [捕获列表](参数列表) { 函数体 }  // 最常用
 *
 * 各部分说明：
 * - 捕获列表：如何从外部作用域捕获变量
 * - 参数列表：函数参数（可选）
 * - mutable：允许修改按值捕获的变量（可选）
 * - 异常说明：noexcept 等（可选）
 * - 返回类型：尾置返回类型（可选，可自动推导）
 * - 函数体：Lambda 代码
 */

void demoBasicSyntax() {
    cout << "=== Lambda 基本语法 ===" << endl;

    // 最简单的 Lambda
    []() {
        cout << "Hello from Lambda!" << endl;
    }();  // 立即调用

    // 带参数的 Lambda
    auto greet = [](const string& name) {
        cout << "Hello, " << name << "!" << endl;
    };
    greet("World");

    // 带返回值的 Lambda
    auto add = [](int a, int b) -> int {
        return a + b;
    };
    cout << "3 + 5 = " << add(3, 5) << endl;

    // 返回类型自动推导
    auto multiply = [](int a, int b) {
        return a * b;  // 自动推导为 int
    };
    cout << "4 * 6 = " << multiply(4, 6) << endl;

    cout << endl;
}

// ==================== 2. 捕获列表 ====================

/*
 * 捕获列表语法：
 * - []                  : 不捕获外部变量
 * - [var]               : 按值捕获 var
 * - [=]                 : 按值捕获所有外部变量
 * - [&var]              : 按引用捕获 var
 * - [&]                 : 按引用捕获所有外部变量
 * - [=, &var]           : 按值捕获所有，但 var 按引用
 * - [&, var]            : 按引用捕获所有，但 var 按值
 * - [this]              : 捕获当前类的 this 指针
 */

void demoCaptureList() {
    cout << "=== 捕获列表 ===" << endl;

    int x = 10;
    int y = 20;

    // 按值捕获
    auto valueCapture = [x](int n) {
        // x 是只读的副本
        return x + n;
        // x++;  // ✗ 错误：按值捕获的变量默认是 const
    };
    cout << "按值捕获 x + 5 = " << valueCapture(5) << endl;

    // 使用 mutable 修改按值捕获的变量
    auto mutableCapture = [x](int n) mutable {
        x += n;  // ✓ 可以修改
        return x;
    };
    cout << "mutable: x + 5 = " << mutableCapture(5) << endl;
    cout << "外部的 x 仍然是 " << x << "（按值捕获不修改原变量）" << endl;

    // 按引用捕获
    auto refCapture = [&x, &y]() {
        x += 1;
        y += 1;
        cout << "在 Lambda 中: x = " << x << ", y = " << y << endl;
    };
    refCapture();
    cout << "引用捕获后: x = " << x << ", y = " << y << endl;

    // 混合捕获
    int a = 1, b = 2, c = 3;
    auto mixedCapture = [=, &c]() {
        // a, b 按值捕获，c 按引用捕获
        cout << "a = " << a << ", b = " << b << ", c = " << c << endl;
        c = 100;  // 可以修改按引用捕获的变量
        // a = 10;  // ✗ 不能修改按值捕获的
    };
    mixedCapture();
    cout << "混合捕获后: a = " << a << ", b = " << b << ", c = " << c << endl;

    // 按值捕获所有，特定变量按引用
    auto defaultRef = [&, x]() {
        // x 按值，其他按引用
        cout << "x = " << x << ", y = " << y << endl;
    };
    defaultRef();

    cout << endl;
}

// ==================== 3. Lambda 与 STL 算法 ====================

void demoWithSTL() {
    cout << "=== Lambda 与 STL 算法 ===" << endl;

    vector<int> vec = {5, 2, 8, 1, 9, 3, 7};

    // for_each
    cout << "遍历 vector: ";
    for_each(vec.begin(), vec.end(), [](int n) {
        cout << n << " ";
    });
    cout << endl;

    // sort
    sort(vec.begin(), vec.end(), [](int a, int b) {
        return a > b;  // 降序排序
    });
    cout << "降序排序后: ";
    for (const auto& n : vec) {
        cout << n << " ";
    }
    cout << endl;

    // find_if
    auto it = find_if(vec.begin(), vec.end(), [](int n) {
        return n > 5;
    });
    if (it != vec.end()) {
        cout << "第一个大于 5 的元素: " << *it << endl;
    }

    // count_if
    int count = count_if(vec.begin(), vec.end(), [](int n) {
        return n % 2 == 0;
    });
    cout << "偶数个数: " << count << endl;

    // transform
    vector<int> doubled;
    transform(vec.begin(), vec.end(), back_inserter(doubled), [](int n) {
        return n * 2;
    });
    cout << "翻倍后: ";
    for (const auto& n : doubled) {
        cout << n << " ";
    }
    cout << endl;

    // accumulate（需要 include <numeric>）
    int sum = 0;
    for_each(vec.begin(), vec.end(), [&sum](int n) {
        sum += n;
    });
    cout << "总和: " << sum << endl;

    cout << endl;
}

// ==================== 4. Lambda 与容器操作 ====================

void demoWithContainers() {
    cout << "=== Lambda 与容器操作 ===" << endl;

    // 删除满足条件的元素
    vector<int> vec = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    // 删除所有偶数
    vec.erase(
        remove_if(vec.begin(), vec.end(), [](int n) {
            return n % 2 == 0;
        }),
        vec.end()
    );

    cout << "删除偶数后: ";
    for (const auto& n : vec) {
        cout << n << " ";
    }
    cout << endl;

    // map 操作
    map<string, int> scores = {
        {"Alice", 85},
        {"Bob", 92},
        {"Charlie", 78}
    };

    // 查找分数最高的
    auto highest = max_element(scores.begin(), scores.end(),
        [](const pair<string, int>& a, const pair<string, int>& b) {
            return a.second < b.second;
        }
    );
    cout << "最高分: " << highest->first << " = " << highest->second << endl;

    cout << endl;
}

// ==================== 5. Lambda 作为函数参数 ====================

// 接受可调用对象的函数模板
template<typename Func>
void applyOperation(int x, int y, Func operation) {
    cout << "结果: " << operation(x, y) << endl;
}

void demoAsFunctionParameter() {
    cout << "=== Lambda 作为函数参数 ===" << endl;

    applyOperation(10, 5, [](int a, int b) {
        return a + b;
    });

    applyOperation(10, 5, [](int a, int b) {
        return a - b;
    });

    applyOperation(10, 5, [](int a, int b) {
        return a * b;
    });

    cout << endl;
}

// ==================== 6. Lambda 作为返回值 ====================

// 返回 Lambda 的函数
function<int(int)> makeMultiplier(int factor) {
    return [factor](int x) {
        return x * factor;
    };
}

void demoAsReturnValue() {
    cout << "=== Lambda 作为返回值 ===" << endl;

    auto times2 = makeMultiplier(2);
    auto times3 = makeMultiplier(3);

    cout << "5 * 2 = " << times2(5) << endl;
    cout << "5 * 3 = " << times3(5) << endl;

    cout << endl;
}

// ==================== 7. Lambda 捕获 this ====================

class Counter {
private:
    int count;

public:
    Counter() : count(0) {}

    void increment() {
        count++;
    }

    // 返回一个 Lambda，捕获 this
    function<void()> getIncrementer() {
        return [this]() {
            this->count++;
            cout << "Count = " << count << endl;
        };
    }

    // C++17 支持 [*this] 按值捕获对象
    // function<void()> getIncrementerCopy() {
    //     return [*this]() {
    //         this->count++;
    //         cout << "Count (copy) = " << count << endl;
    //     };
    // }

    int getCount() const { return count; }
};

void demoCaptureThis() {
    cout << "=== Lambda 捕获 this ===" << endl;

    Counter counter;
    counter.increment();
    counter.increment();

    auto inc = counter.getIncrementer();
    inc();  // 调用 Lambda
    inc();

    cout << "最终 count: " << counter.getCount() << endl;

    cout << endl;
}

// ==================== 8. Lambda 与 std::function ====================

void demoStdFunction() {
    cout << "=== Lambda 与 std::function ===" << endl;

    // std::function 可以存储任何可调用对象
    function<int(int, int)> func;

    // 赋值为 Lambda
    func = [](int a, int b) { return a + b; };
    cout << "func(3, 4) = " << func(3, 4) << endl;

    // 赋值为另一个 Lambda
    func = [](int a, int b) { return a * b; };
    cout << "func(3, 4) = " << func(3, 4) << endl;

    // 存储 Lambda 的容器
    vector<function<int(int)>> operations;
    operations.push_back([](int x) { return x * 2; });
    operations.push_back([](int x) { return x * x; });
    operations.push_back([](int x) { return x + 10; });

    cout << "操作结果: ";
    for (const auto& op : operations) {
        cout << op(5) << " ";
    }
    cout << endl;

    cout << endl;
}

// ==================== 9. Lambda 的底层实现 ====================

/*
 * Lambda 的底层原理：
 * - 编译器将 Lambda 转换为一个 functor（函数对象）
 * - 捕获的变量成为 functor 的成员变量
 * - Lambda 的函数体成为 operator()
 *
 * 示例：
 *   auto lambda = [x](int y) { return x + y; };
 *
 * 编译器大致生成：
 *   class CompilerGeneratedName {
 *       int x;
 *   public:
 *       CompilerGeneratedName(int _x) : x(_x) {}
 *       int operator()(int y) const { return x + y; }
 *   };
 *   CompilerGeneratedName lambda(x);
 */

class ManualFunctor {
private:
    int x;

public:
    ManualFunctor(int _x) : x(_x) {}

    int operator()(int y) const {
        return x + y;
    }
};

void demoUnderTheHood() {
    cout << "=== Lambda 底层实现 ===" << endl;

    // Lambda（C++11 兼容写法：先捕获变量，再初始化）
    int x = 10;
    auto lambda = [x](int y) {
        return x + y;
    };
    cout << "Lambda: lambda(5) = " << lambda(5) << endl;

    // 手写的 functor
    ManualFunctor functor(10);
    cout << "Functor: functor(5) = " << functor(5) << endl;

    cout << "两者效果相同！" << endl;

    cout << endl;
}

// ==================== 10. 泛型 Lambda（C++14）====================

void demoGenericLambda() {
    cout << "=== 泛型 Lambda（C++14 提示）===" << endl;

#if __cplusplus >= 201402L
    // 使用 auto 参数的泛型 Lambda
    auto print = [](auto x) {
        cout << x << " ";
    };

    cout << "打印不同类型: ";
    print(42);        // int
    print(3.14);      // double
    print("Hello");   // const char*
    print(string("World"));  // string
    cout << endl;
#else
    cout << "需要 C++14 支持" << endl;
#endif

    cout << endl;
}

// ==================== 11. Lambda 递归 ====================

void demoRecursiveLambda() {
    cout << "=== Lambda 递归 ===" << endl;

    // 使用 std::function 实现递归
    function<int(int)> factorial = [&](int n) {
        return n <= 1 ? 1 : n * factorial(n - 1);
    };

    cout << "5! = " << factorial(5) << endl;

    // C++11 方式：使用 std::function 实现斐波那契
    function<int(int)> fibonacci = [&](int n) -> int {
        return n <= 1 ? n : fibonacci(n - 1) + fibonacci(n - 2);
    };

    cout << "fib(7) = " << fibonacci(7) << endl;
    cout << endl;
}

// ==================== 主函数测试 ====================

int main() {
    cout << "==================== C++11 Lambda 表达式详解 ====================" << endl;

    demoBasicSyntax();
    demoCaptureList();
    demoWithSTL();
    demoWithContainers();
    demoAsFunctionParameter();
    demoAsReturnValue();
    demoCaptureThis();
    demoStdFunction();
    demoUnderTheHood();
    demoGenericLambda();
    demoRecursiveLambda();

    cout << "==================== 总结 ====================" << endl;
    cout << R"(
Lambda 表达式总结：

1. 基本语法：
   [捕获列表](参数列表) -> 返回类型 { 函数体 }

2. 捕获列表：
   - []            : 不捕获
   - [x]           : 按值捕获 x
   - [&x]          : 按引用捕获 x
   - [=]           : 按值捕获所有
   - [&]           : 按引用捕获所有
   - [=, &x]       : 混合捕获

3. 捕获方式：
   - 按值捕获：创建副本，默认 const
   - 按引用捕获：引用原变量，可修改
   - mutable：允许修改按值捕获的变量

4. 与 STL 配合：
   - for_each, transform, find_if, count_if 等
   - 简化代码，提高可读性

5. std::function：
   - 可以存储 Lambda
   - 类型擦除的函数包装器

6. 底层实现：
   - Lambda 被转换为函数对象（functor）
   - 捕获变量成为成员变量

7. 使用场景：
   - 简短的一次性函数
   - STL 算法回调
   - 事件处理
   - 并发编程

8. 最佳实践：
   - 简短逻辑用 Lambda
   - 复杂逻辑用命名函数
   - 按值捕获避免悬空引用
   - 按引用捕获注意生命周期
)" << endl;

    return 0;
}

/*
 * 编译运行：
 * g++ -std=c++11 10_lambda.cpp -o lambda
 * ./lambda
 *
 * 注：泛型 Lambda 需要 C++14
 */
