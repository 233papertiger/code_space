/**
 * C++11 override 和 final 关键字详解
 *
 * 学习目标：
 * 1. 理解 override 关键字的作用和好处
 * 2. 理解 final 关键字在类和函数上的作用
 * 3. 掌握如何防止意外的函数重写和类继承
 *
 * ==================== 核心知识点总结 ====================
 *
 * 【重写 vs 重载】
 * - 重写(override)：派生类重新定义基类的虚函数，函数签名必须完全一致
 * - 重载(overload)：同一作用域内，函数名相同但参数列表不同
 *
 * 【虚函数重写的要求】
 * - 基类：必须有 virtual 修饰
 * - 子类：override 关键字是可选的（但强烈建议写，让编译器检查）
 *
 * 【override 位置规则】
 * void func() const volatile noexcept override/final;
 *           ↑      ↑        ↑         ↑
 *         const  volatile noexcept  override/final
 * 顺序：const → volatile → noexcept → override/final
 *
 * 【普通虚函数 vs 纯虚函数】
 * ┌─────────────────┬──────────────────────┬──────────────────┐
 * │      类型        │      基类写法         │ 子类是否必须实现   │
 * ├─────────────────┼──────────────────────┼──────────────────┤
 * │ 普通虚函数       │ virtual void f() {}   │ 可选（可直接继承） │
 * │ 纯虚函数         │ virtual void f() = 0; │ 必须（除非也是抽象类）│
 * └─────────────────┴──────────────────────┴──────────────────┘
 */

#include <iostream>
#include <string>

using namespace std;

// ==================== 1. 虚函数基础概念 ====================

/*
 * 【重写三要素】
 * 1. 基类函数必须有 virtual 关键字
 * 2. 子类函数签名必须完全一致（包括 const）
 * 3. override 关键字可选，但强烈建议写
 *
 * 【override 的必要性示例】
 * - 有 override：签名错误时编译器报错
 * - 无 override：签名错误时静默创建新函数（严重 bug！）
 */

// 基类：演示普通虚函数和纯虚函数的区别
class AnimalBase {
public:
    // 普通虚函数：有默认实现，子类可以选择不重写
    virtual void sleep() {
        cout << "AnimalBase::sleep() - 默认睡觉方式" << endl;
    }

    // 纯虚函数：没有实现，子类必须重写（否则子类也是抽象类）
    virtual void makeSound() = 0;

    virtual ~AnimalBase() = default;
};

// 正确实现：重写了纯虚函数，普通虚函数使用默认实现
class Dog : public AnimalBase {
public:
    // 必须实现纯虚函数
    void makeSound() override {
        cout << "Dog: Woof!" << endl;
    }

    // sleep() 不写，直接使用基类的默认实现
};

// 错误示例：不实现纯虚函数会导致编译错误
/*
class Cat : public AnimalBase {
    // 没有实现 makeSound()，Cat 也是抽象类，不能实例化
    // Cat cat;  // ✗ 编译错误
};
*/

// ==================== 2. override 关键字 ====================

/*
 * override 关键字的作用：
 * 1. 明确表示该函数是要重写基类的虚函数
 * 2. 让编译器帮助检查函数签名是否正确匹配
 * 3. 提高代码可读性，一眼就能看出这是重写
 *
 * 【override 的位置规则】
 * void func() const override;     // ✓ 正确
 * void func() override const;     // ✗ 错误！const 必须在前
 *
 * 正确顺序：返回值 → 函数名 → (参数) → const → volatile → noexcept → override/final
 */

class Base {
public:
    // 基类虚函数
    virtual void func1() {
        cout << "Base::func1()" << endl;
    }

    virtual void func2(int x) {
        cout << "Base::func2(int): " << x << endl;
    }

    virtual void func3() const {
        cout << "Base::func3() const" << endl;
    }

    virtual ~Base() {
        cout << "Base destructor" << endl;
    }
};

// 正确使用 override
class DerivedCorrect : public Base {
public:
    // ✓ 正确：完全匹配基类函数签名
    void func1() override {
        cout << "DerivedCorrect::func1() - 正确重写" << endl;
    }

    void func2(int x) override {
        cout << "DerivedCorrect::func2(int): " << x << " - 正确重写" << endl;
    }

    void func3() const override {
        cout << "DerivedCorrect::func3() const - 正确重写" << endl;
    }
};

// 演示 override 的错误检查作用（这些会被编译器报错）
/*
class DerivedWrong : public Base {
public:
    // ✗ 错误：函数签名不匹配（多了 const）
    // void func1() const override {
    //     cout << "Wrong" << endl;
    // }

    // ✗ 错误：参数类型不匹配
    // void func2(double x) override {
    //     cout << "Wrong" << endl;
    // }

    // ✗ 错误：基类没有这个虚函数
    // void func4() override {
    //     cout << "Wrong" << endl;
    // }

    // 如果没有 override，这些会是新函数而不是重写！
    // 这会导致难以发现的 bug
};
*/

// 对比：不使用 override 可能导致的问题
class Danger {
public:
    virtual void process() {
        cout << "Danger::process()" << endl;
    }
};

class DangerousDerived : public Danger {
public:
    // 没有使用 override，如果不小心写错了函数签名
    // 编译器不会报错，而是创建了一个新函数！
    void process() const {  // 注意多了 const
        cout << "DangerousDerived::process() const - 这是一个新函数，不是重写！" << endl;
    }
};

// ==================== 2. final 关键字 ====================

/*
 * final 关键字有两种用法：
 * 1. 修饰函数：阻止该函数被进一步重写
 * 2. 修饰类：阻止该类被继承
 */

// 2.1 final 修饰函数
class Animal {
public:
    virtual void speak() {
        cout << "Animal speaks" << endl;
    }

    // final 函数：派生类不能再重写
    virtual void breathe() final {
        cout << "Animal breathes - 子类不能重写此函数" << endl;
    }
};

class Puppy : public Animal {
public:
    void speak() override {
        cout << "Puppy barks" << endl;
    }

    // ✗ 编译错误：不能重写 final 函数
    // void breathe() override {
    //     cout << "Puppy breathes differently" << endl;
    // }
};

// 2.2 final 修饰类
class FinalClass final {
public:
    void doSomething() {
        cout << "FinalClass::doSomething()" << endl;
    }
};

// ✗ 编译错误：不能继承 final 类
/*
class DerivedFromFinal : public FinalClass {
public:
    void doSomething() {
        cout << "Cannot inherit from final class" << endl;
    }
};
*/

// ==================== 3. 综合示例：设计不可变接口 ====================

class Shape {
public:
    virtual double area() const = 0;
    virtual double perimeter() const = 0;

    // 某些关键函数不应该被修改，标记为 final
    virtual void printInfo() final {
        cout << "Shape Info - Area: " << area()
             << ", Perimeter: " << perimeter() << endl;
    }

    virtual ~Shape() = default;
};

class Circle : public Shape {
private:
    double radius;
public:
    Circle(double r) : radius(r) {}

    double area() const override {
        return 3.14159 * radius * radius;
    }

    double perimeter() const override {
        return 2 * 3.14159 * radius;
    }

    // ✗ 不能重写 final 函数
    // void printInfo() override { ... }
};

// ==================== 4. 实用模式：Template Method + final ====================

class DataProcessor {
public:
    // 模板方法，定义算法骨架，标记为 final 防止修改
    virtual void process() final {
        validate();
        doProcess();
        cleanup();
    }

    virtual ~DataProcessor() = default;

protected:
    // 子类必须实现的具体处理逻辑
    virtual void doProcess() = 0;

private:
    void validate() {
        cout << "Validating data..." << endl;
    }

    void cleanup() {
        cout << "Cleaning up resources..." << endl;
    }
};

class CSVProcessor : public DataProcessor {
protected:
    void doProcess() override {
        cout << "Processing CSV data..." << endl;
    }
};

// 不能重写 process()，因为它是 final
/*
class BadProcessor : public CSVProcessor {
public:
    void process() {
        cout << "Trying to change the algorithm..." << endl;
    }
};
*/

// ==================== 主函数测试 ====================

void testOverride() {
    cout << "=== 测试 override 关键字 ===" << endl;

    Base* base = new DerivedCorrect();
    base->func1();    // 调用 DerivedCorrect::func1()
    base->func2(42);  // 调用 DerivedCorrect::func2(int)
    base->func3();    // 调用 DerivedCorrect::func3() const

    cout << "\n测试不使用 override 的危险情况：" << endl;
    Danger* d = new DangerousDerived();
    d->process();  // 调用的是 Danger::process()，而不是 DangerousDerived 的！

    delete base;
    delete d;
    cout << endl;
}

void testFinal() {
    cout << "=== 测试 final 关键字 ===" << endl;

    Puppy dog;
    dog.speak();    // OK，可以调用重写的函数
    dog.breathe();  // OK，调用 Animal 的 final 函数

    cout << "\n测试 final 类：" << endl;
    FinalClass fc;
    fc.doSomething();

    cout << "\n测试 Shape 类：" << endl;
    Circle circle(5.0);
    circle.printInfo();  // 调用 Shape 的 final 函数

    cout << "\n测试模板方法模式：" << endl;
    CSVProcessor csv;
    csv.process();  // 完整的算法流程

    cout << endl;
}

int main() {
    testOverride();
    testFinal();

    cout << "=== 总结 ===" << endl;
    cout << "1. override 关键字：" << endl;
    cout << "   - 明确表示函数重写" << endl;
    cout << "   - 编译器检查签名匹配" << endl;
    cout << "   - 防止意外创建新函数" << endl;
    cout << "\n2. final 关键字：" << endl;
    cout << "   - final 函数：阻止被重写" << endl;
    cout << "   - final 类：阻止被继承" << endl;
    cout << "   - 用于设计不可变接口" << endl;

    return 0;
}

/*
 * 编译运行：
 * g++ -std=c++11 01_override_final.cpp -o override_final
 * ./override_final
 */
