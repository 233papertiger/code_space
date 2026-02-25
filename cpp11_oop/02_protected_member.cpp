/**
 * C++ protected 访问修饰符详解 - 成员变量篇
 *
 * 学习目标：
 * 1. 理解 protected 成员与 public/private 的区别
 * 2. 掌握 protected 成员的访问规则
 * 3. 理解为什么需要 protected（何时使用）
 */

#include <iostream>
#include <string>

using namespace std;

// ==================== 1. 三种访问修饰符对比 ====================

class AccessDemo {
public:    // 公共成员：任何人都可以访问
    int publicVar = 100;
    void publicMethod() {
        cout << "AccessDemo::publicMethod()" << endl;
    }

protected: // 保护成员：只有本类和派生类可以访问
    int protectedVar = 200;
    void protectedMethod() {
        cout << "AccessDemo::protectedMethod()" << endl;
    }

private:   // 私有成员：只有本类可以访问
    int privateVar = 300;
    void privateMethod() {
        cout << "AccessDemo::privateMethod()" << endl;
    }

public:
    // 公共方法用于演示访问权限
    void demoAccessInClass() {
        cout << "=== 在类内部访问 ===" << endl;
        // ✓ 类内部可以访问所有成员
        cout << "publicVar: " << publicVar << endl;
        cout << "protectedVar: " << protectedVar << endl;
        cout << "privateVar: " << privateVar << endl;

        publicMethod();
        protectedMethod();
        privateMethod();
    }
};

// ==================== 2. 在派生类中访问 protected 成员 ====================

class DerivedClass : public AccessDemo {
public:
    void demoAccessInDerived() {
        cout << "=== 在派生类中访问基类成员 ===" << endl;

        // ✓ 可以访问 public 成员
        cout << "publicVar: " << publicVar << endl;
        publicMethod();

        // ✓ 可以访问 protected 成员（这是 protected 的核心作用！）
        cout << "protectedVar: " << protectedVar << endl;
        protectedMethod();

        // ✗ 不能访问 private 成员
        // cout << privateVar;      // 编译错误
        // privateMethod();         // 编译错误
    }

    // 演示：protected 允许派生类直接操作基类的内部状态
    void modifyProtected() {
        cout << "\n=== 派生类修改基类 protected 成员 ===" << endl;
        protectedVar = 999;  // ✓ 可以修改
        cout << "修改后 protectedVar: " << protectedVar << endl;
    }
};

// ==================== 3. 在类外部访问的规则 ====================

void demoAccessOutside() {
    AccessDemo obj;
    DerivedClass derivedObj;

    cout << "=== 在类外部访问 ===" << endl;

    // ✓ 可以访问 public 成员
    cout << "obj.publicVar: " << obj.publicVar << endl;
    obj.publicMethod();

    // ✗ 不能访问 protected 成员
    // cout << obj.protectedVar;    // 编译错误
    // obj.protectedMethod();       // 编译错误

    // ✗ 不能访问 private 成员
    // cout << obj.privateVar;      // 编译错误
    // obj.privateMethod();         // 编译错误
}

// ==================== 4. 实际应用场景：基类提供工具方法 ====================

class Animal {
protected:
    string name;
    int age;

    // protected 工具方法：供派生类使用，外部不可调用
    void logAction(const string& action) {
        cout << "[" << name << "] " << action << endl;
    }

    // protected 验证方法
    bool isValidAge(int a) {
        return a > 0 && a < 100;
    }

public:
    Animal(const string& n, int a) : name(n), age(a) {}

    void introduce() {
        cout << "I'm " << name << ", " << age << " years old." << endl;
    }

    virtual void makeSound() = 0;
    virtual ~Animal() = default;
};

class Dog : public Animal {
private:
    string breed;

public:
    Dog(const string& n, int a, const string& b)
        : Animal(n, a), breed(b) {}

    void makeSound() override {
        // ✓ 可以使用基类的 protected 工具方法
        logAction("barks: Woof! Woof!");
    }

    void fetch() {
        // ✓ 可以访问和修改 protected 成员
        logAction("is fetching the ball");
    }

    void birthday() {
        // ✓ 可以修改 protected 成员
        if (isValidAge(age + 1)) {
            age++;
            logAction("celebrates birthday! Now " + to_string(age) + " years old.");
        }
    }

    void displayDetails() {
        cout << "Breed: " << breed << ", Name: " << name << ", Age: " << age << endl;
    }
};

// ==================== 5. protected 的设计理念 ====================

/*
 * protected 的使用场景：
 * 1. 基类的某些数据/方法应该对派生类可见，但对外部隐藏
 * 2. 派生类需要直接访问或修改基类的某些内部状态
 * 3. 提供给派生类使用的工具方法
 */

class Window {
protected:
    int width, height;
    bool isVisible;

    // protected 方法：供派生类使用，封装窗口系统调用
    virtual void redraw() {
        cout << "Window::redraw() - basic redraw" << endl;
    }

    void invalidate() {
        cout << "Window::invalidate() - marking for redraw" << endl;
    }

public:
    Window(int w, int h) : width(w), height(h), isVisible(true) {}

    void resize(int newWidth, int newHeight) {
        width = newWidth;
        height = newHeight;
        invalidate();  // 调用 protected 方法
        redraw();       // 调用 protected 虚函数
    }

    void show() { isVisible = true; }
    void hide() { isVisible = false; }

    virtual ~Window() = default;
};

class Button : public Window {
private:
    string label;

public:
    Button(int w, int h, const string& l)
        : Window(w, h), label(l) {}

    // ✓ 可以访问基类 protected 成员
    void click() {
        if (isVisible) {
            cout << "Button [" << label << "] clicked!" << endl;
            // 可能需要触发重绘
            invalidate();  // 调用基类 protected 方法
        }
    }

protected:
    // 重写 protected 方法
    void redraw() override {
        cout << "Button::redraw() - drawing button: " << label
             << " (" << width << "x" << height << ")" << endl;
    }
};

// ==================== 6. 注意事项：友元函数 ====================

class ProtectedDemo {
protected:
    int secret = 42;

    // 声明友元函数
    friend void friendFunction(ProtectedDemo& obj);
};

// 友元函数可以访问 protected 成员
void friendFunction(ProtectedDemo& obj) {
    cout << "友元函数访问 protected: " << obj.secret << endl;
}

// ==================== 主函数测试 ====================

int main() {
    cout << "==================== 访问修饰符演示 ====================" << endl;

    AccessDemo demo;
    demo.demoAccessInClass();
    cout << endl;

    DerivedClass derived;
    derived.demoAccessInDerived();
    derived.modifyProtected();
    cout << endl;

    cout << "==================== Animal 类演示 ====================" << endl;
    Dog dog("Buddy", 3, "Golden Retriever");
    dog.introduce();
    dog.makeSound();   // 使用基类 protected 方法
    dog.fetch();
    dog.birthday();    // 修改基类 protected 成员
    dog.displayDetails();
    cout << endl;

    cout << "==================== Window 类演示 ====================" << endl;
    Button button(100, 30, "Submit");
    button.resize(120, 35);  // 调用包含 protected 方法的 public 方法
    button.click();
    button.hide();
    button.click();  // 不会响应，因为不可见
    cout << endl;

    cout << "==================== 友元函数演示 ====================" << endl;
    ProtectedDemo p;
    friendFunction(p);
    cout << endl;

    cout << "==================== 总结 ====================" << endl;
    cout << R"(
访问修饰符总结：
┌────────────┬──────────┬──────────┬──────────┐
│            │ 本类内部 │ 派生类   │ 外部     │
├────────────┼──────────┼──────────┼──────────┤
│ public     │    ✓    │    ✓    │    ✓    │
│ protected  │    ✓    │    ✓    │    ✗    │
│ private    │    ✓    │    ✗    │    ✗    │
└────────────┴──────────┴──────────┴──────────┘

使用建议：
1. 优先使用 private，提供 public 接口
2. 需要派生类访问时使用 protected
3. public 用于对外接口
4. protected 用于基类和派生类之间的协作
)" << endl;

    return 0;
}

/*
 * 编译运行：
 * g++ -std=c++11 02_protected_member.cpp -o protected_member
 * ./protected_member
 */
