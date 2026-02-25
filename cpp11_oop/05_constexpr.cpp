/**
 * C++11 constexpr 关键字详解
 *
 * 学习目标：
 * 1. 理解 constexpr 与 const 的区别
 * 2. 掌握 constexpr 函数和变量的使用
 * 3. 理解编译期计算的意义和限制
 */

#include <iostream>
#include <array>
using namespace std;

// ==================== 1. constexpr vs const ====================

/*
 * const: 运行期常量，值在运行期确定
 * constexpr: 编译期常量，值在编译期计算
 */

void demoConstVsConstexpr() {
    cout << "=== const vs constexpr ===" << endl;

    // const 变量（运行期常量）
    const int constVar = 42;  // 编译期值，但在技术上是运行期常量
    cout << "constVar: " << constVar << endl;

    // constexpr 变量（编译期常量）
    constexpr int constexprVar = 42;  // 必须在编译期能确定值
    cout << "constexprVar: " << constexprVar << endl;

    // 关键区别：constexpr 可以用于需要编译期常量的地方
    int arr1[constVar];     // 可能是编译器扩展，不是标准保证
    int arr2[constexprVar]; // ✓ 标准保证，编译期常量

    cout << "数组大小: " << sizeof(arr2)/sizeof(arr2[0]) << endl;
}

// ==================== 2. constexpr 函数 ====================

// constexpr 函数：如果参数是编译期常量，结果也是编译期常量
constexpr int square(int x) {
    return x * x;
}

// constexpr 函数的限制（C++11）：
// - 只能有一条 return 语句
// - 不能有循环、局部变量、条件语句（后来放宽了）

constexpr int factorial(int n) {
    return n <= 1 ? 1 : n * factorial(n - 1);
}

void demoConstexprFunctions() {
    cout << "\n=== constexpr 函数 ===" << endl;

    // 编译期计算
    constexpr int sq = square(5);  // 编译期计算
    cout << "square(5) = " << sq << endl;

    constexpr int fact = factorial(5);  // 编译期递归计算
    cout << "factorial(5) = " << fact << endl;

    // 运行期也可以调用
    int x = 10;
    cout << "square(10) = " << square(x) << endl;  // 运行期计算

    // 编译期常量可以用于数组大小
    int arr[factorial(4)];  // 24 个元素
    cout << "factorial(4) = " << factorial(4) << " (数组大小)" << endl;
}

// ==================== 3. constexpr 用于模板和元编程 ====================

// 编译期判断素数
constexpr bool isPrimeRecursive(int n, int d) {
    return d * d > n ? true : (n % d == 0 ? false : isPrimeRecursive(n, d + 1));
}

constexpr bool isPrime(int n) {
    return n < 2 ? false : isPrimeRecursive(n, 2);
}

// 编译期选择
template<int N>
struct CompileTimeCheck {
    static constexpr bool value = isPrime(N);
    static void print() {
        cout << N << " 是" << (value ? "" : "不") << "是素数" << endl;
    }
};

void demoCompileTimeChecks() {
    cout << "\n=== 编译期计算 ===" << endl;

    // 编译期判断
    constexpr bool prime17 = isPrime(17);
    constexpr bool prime18 = isPrime(18);

    cout << "17 是素数: " << prime17 << endl;
    cout << "18 是素数: " << prime18 << endl;

    // 编译期用于模板
    CompileTimeCheck<17>::print();
    CompileTimeCheck<18>::print();

    // 编译期作为数组大小
    constexpr int size = isPrime(7) ? 100 : 200;
    int arr[size];
    cout << "数组大小: " << sizeof(arr)/sizeof(arr[0]) << endl;
}

// ==================== 4. constexpr 类和成员函数 ====================

class Point {
    double x_, y_;

public:
    // constexpr 构造函数（C++11）
    constexpr Point(double x, double y) : x_(x), y_(y) {}

    // constexpr 成员函数
    constexpr double x() const { return x_; }
    constexpr double y() const { return y_; }

    // constexpr 成员函数可以在编译期计算
    constexpr double distance() const {
        return x_ * x_ + y_ * y_;
    }
};

void demoConstexprClass() {
    cout << "\n=== constexpr 类和成员函数 ===" << endl;

    // 编译期构造 Point
    constexpr Point p(3.0, 4.0);

    // 编译期调用成员函数
    constexpr double dist = p.distance();
    cout << "Point(" << p.x() << ", " << p.y() << ") 距离平方: " << dist << endl;

    // 编译期创建数组
    constexpr Point points[] = {
        Point(0.0, 0.0),
        Point(1.0, 1.0),
        Point(2.0, 2.0)
    };
    cout << "编译期 Point 数组大小: " << (sizeof(points) / sizeof(points[0])) << endl;
}

// ==================== 5. constexpr 和字面类型 ====================

/*
 * C++11 中的字面类型（Literal Type）：
 * - 标量类型（int, float, char 等）
 * - 引用类型
 * - 具有 constexpr 构造函数和析构函数的类
 * - 数组 of literal types
 */

// 用户自定义字面类型
class Complex {
    double real_, imag_;

public:
    constexpr Complex(double r, double i) : real_(r), imag_(i) {}

    constexpr double real() const { return real_; }
    constexpr double imag() const { return imag_; }

    // constexpr 运算
    constexpr Complex operator+(const Complex& other) const {
        return Complex(real_ + other.real_, imag_ + other.imag_);
    }
};

void demoLiteralType() {
    cout << "\n=== 字面类型和 constexpr 运算 ===" << endl;

    constexpr Complex c1(1.0, 2.0);
    constexpr Complex c2(3.0, 4.0);
    constexpr Complex c3 = c1 + c2;  // 编译期计算

    cout << "c1 + c2 = " << c3.real() << " + " << c3.imag() << "i" << endl;
}

// ==================== 6. constexpr 的限制 ====================

/*
 * C++11 中 constexpr 函数的限制：
 * 1. 必须非常简单（单条 return）
 * 2. 不能有循环（用递归代替）
 * 3. 不能有局部变量（除了返回值）
 * 4. 不能有 if 语句（用三元运算符）
 * 5. 不能抛异常
 *
 * 注：C++14/17 放宽了这些限制
 */

// C++11 风格：用三元运算符代替 if
constexpr int absCxx11(int x) {
    return x < 0 ? -x : x;
}

// C++14 风格：可以使用局部变量和 if 语句
/*
constexpr int absCxx14(int x) {
    if (x < 0) {
        return -x;
    }
    return x;
}
*/

// C++11 风格：用递归代替循环
constexpr int powerCxx11(int base, int exp) {
    return exp == 0 ? 1 : base * powerCxx11(base, exp - 1);
}

void demoLimitations() {
    cout << "\n=== constexpr 限制和替代方案 ===" << endl;

    constexpr int a = absCxx11(-42);
    constexpr int p = powerCxx11(2, 10);

    cout << "abs(-42) = " << a << endl;
    cout << "2^10 = " << p << endl;
}

// ==================== 7. 实际应用：编译期查找表 ====================

// 编译期生成斐波那契数列
constexpr int fib(int n) {
    return n < 2 ? n : fib(n - 1) + fib(n - 2);
}

// 编译期生成查找表（C++11 兼容方式）
// 使用递归辅助函数生成数组
template<int N>
struct FibTable {
    int values[N];

    // 默认构造函数（非 constexpr，因为 C++11 不支持 constexpr 循环）
    FibTable() : values{} {
        for (int i = 0; i < N; ++i) {
            values[i] = fib(i);
        }
    }
};

void demoLookupTable() {
    cout << "\n=== 编译期查找表 ===" << endl;

    // 运行期生成斐波那契查找表（C++11 限制：不能有 constexpr 循环构造函数）
    const FibTable<10> table{};

    cout << "斐波那契数列（前10项）: ";
    for (int i = 0; i < 10; ++i) {
        cout << table.values[i] << " ";
    }
    cout << endl;

    // 编译期计算单个值
    constexpr int fib9 = fib(9);
    cout << "fib(9) = " << fib9 << endl;
}

// ==================== 8. constexpr 与标准库 ====================

// 自定义 constexpr max（C++11 中 std::max 不是 constexpr）
template<typename T>
constexpr T constexprMax(const T& a, const T& b) {
    return a < b ? b : a;
}

void demoStdLibrary() {
    cout << "\n=== constexpr 与标准库 ===" << endl;

    // std::array 的大小必须是编译期常量
    constexpr int size = 5;
    std::array<int, size> arr = {1, 2, 3, 4, 5};

    cout << "std::array 大小: " << arr.size() << endl;
    cout << "第一个元素: " << arr.front() << endl;

    // C++11 中 std::max 不是 constexpr，需要自定义
    constexpr int maxVal = constexprMax(10, 20);
    cout << "constexprMax(10, 20) = " << maxVal << endl;
}

// ==================== 主函数测试 ====================

int main() {
    cout << "==================== C++11 constexpr 详解 ====================" << endl;

    demoConstVsConstexpr();
    demoConstexprFunctions();
    demoCompileTimeChecks();
    demoConstexprClass();
    demoLiteralType();
    demoLimitations();
    demoLookupTable();
    demoStdLibrary();

    cout << "\n==================== 总结 ====================" << endl;
    cout << R"(
constexpr 关键字总结：

1. 基本概念：
   - constexpr: 编译期常量表达式
   - const: 运行期常量（大部分情况）
   - constexpr 可以用于需要编译期常量的地方

2. constexpr 函数：
   - 参数是编译期常量 → 结果是编译期常量
   - 参数是运行期值 → 运行期计算
   - C++11: 必须简单（单 return）
   - C++14+: 放宽限制

3. 使用场景：
   - 编译期计算（性能优化）
   - 模板元编程
   - 数组大小
   - 查找表生成

4. 限制（C++11）：
   - 不能有循环（用递归）
   - 不能有局部变量
   - 不能有异常
   - 只能调用 constexpr 函数

5. 最佳实践：
   - 对简单计算函数使用 constexpr
   - 对构造函数使用 constexpr（如果可能）
   - 利用编译期计算提高运行时性能
)" << endl;

    return 0;
}

/*
 * 编译运行：
 * g++ -std=c++11 05_constexpr.cpp -o constexpr
 * ./constexpr
 */
