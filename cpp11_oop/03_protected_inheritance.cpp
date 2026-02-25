/**
 * C++ protected 继承详解
 *
 * 学习目标：
 * 1. 理解 public/protected/private 三种继承方式的区别
 * 2. 掌握 protected 继承的使用场景和效果
 * 3. 理解不同继承方式下访问权限的变化
 */

#include <iostream>
#include <string>

using namespace std;

// ==================== 1. 基类定义 ====================

class Base {
public:
    int publicVar = 1;
    void publicFunc() {
        cout << "Base::publicFunc()" << endl;
    }

protected:
    int protectedVar = 2;
    void protectedFunc() {
        cout << "Base::protectedFunc()" << endl;
    }

private:
    int privateVar = 3;
    void privateFunc() {
        cout << "Base::privateFunc()" << endl;
    }
};

// ==================== 2. 三种继承方式对比 ====================

// 2.1 public 继承
class PublicDerived : public Base {
public:
    void demonstrate() {
        cout << "=== Public 继承 ===" << endl;

        // 基类 public 成员 → 在派生类中是 public
        cout << "publicVar: " << publicVar << endl;  // ✓
        publicFunc();  // ✓

        // 基类 protected 成员 → 在派生类中是 protected
        cout << "protectedVar: " << protectedVar << endl;  // ✓
        protectedFunc();  // ✓

        // 基类 private 成员 → 在派生类中不可访问
        // cout << privateVar;  // ✗
        // privateFunc();  // ✗
    }
};

// 2.2 protected 继承
class ProtectedDerived : protected Base {
public:
    void demonstrate() {
        cout << "=== Protected 继承 ===" << endl;

        // 基类 public 成员 → 在派生类中变成 protected
        cout << "publicVar: " << publicVar << endl;  // ✓（但在派生类内部是 protected）
        publicFunc();  // ✓

        // 基类 protected 成员 → 在派生类中仍是 protected
        cout << "protectedVar: " << protectedVar << endl;  // ✓
        protectedFunc();  // ✓

        // 基类 private 成员 → 不可访问
        // cout << privateVar;  // ✗
    }
};

// 2.3 private 继承
class PrivateDerived : private Base {
public:
    void demonstrate() {
        cout << "=== Private 继承 ===" << endl;

        // 基类 public 成员 → 在派生类中变成 private
        cout << "publicVar: " << publicVar << endl;  // ✓（但在派生类内部是 private）
        publicFunc();  // ✓

        // 基类 protected 成员 → 在派生类中变成 private
        cout << "protectedVar: " << protectedVar << endl;  // ✓
        protectedFunc();  // ✓

        // 基类 private 成员 → 不可访问
        // cout << privateVar;  // ✗
    }
};

// ==================== 3. 访问权限变化表 ====================

/*
 * 继承方式对基类成员访问级别的影响：
 *
 * ┌──────────────┬──────────┬──────────┬──────────┐
 * │  Base 类成员 │  public  │protected │  private  │
 * ├──────────────┼──────────┼──────────┼──────────┤
 * │ public 继承  │  public  │protected │ 不可访问 │
 * │protected 继承│protected │protected │ 不可访问 │
 * │ private 继承 │  private │  private │ 不可访问 │
 * └──────────────┴──────────┴──────────┴──────────┘
 */

// ==================== 4. 多层继承中的权限传播 ====================

class GrandBase {
public:
    int pub = 1;
protected:
    int prot = 2;
private:
    int priv = 3;
};

// 使用 public 继承
class Level1_Public : public GrandBase {
    // pub 是 public, prot 是 protected
};

class Level2_FromPublic : public Level1_Public {
    void demo() {
        cout << pub << endl;   // ✓ 继承来的 public
        cout << prot << endl;  // ✓ 继承来的 protected
        // cout << priv;  // ✗
    }
};

// 使用 protected 继承
class Level1_Protected : protected GrandBase {
    // pub 变成 protected, prot 仍是 protected
};

class Level2_FromProtected : public Level1_Protected {
    void demo() {
        cout << pub << endl;   // ✓ 继承来的 protected
        cout << prot << endl;  // ✓ 继承来的 protected
    }
};

// ==================== 5. 在类外部访问 ====================

void demoOutsideAccess() {
    cout << "=== 在类外部访问不同继承方式的派生类对象 ===" << endl;

    PublicDerived pubD;
    ProtectedDerived protD;
    PrivateDerived privD;

    cout << "\n--- Public 继承的对象 ---" << endl;
    cout << "pubD.publicVar: " << pubD.publicVar << endl;  // ✓
    pubD.publicFunc();  // ✓
    // pubD.protectedVar;  // ✗ 外部不能访问 protected
    // pubD.privateVar;    // ✗ 外部不能访问 private

    cout << "\n--- Protected 继承的对象 ---" << endl;
    // protD.publicVar;  // ✗ public 变成了 protected，外部不能访问
    // protD.publicFunc();  // ✗

    cout << "\n--- Private 继承的对象 ---" << endl;
    // privD.publicVar;  // ✗ public 变成了 private，外部不能访问
    // privD.publicFunc();  // ✗
}

// ==================== 6. 何时使用 protected/private 继承 ====================

/*
 * 使用场景：
 *
 * 1. public 继承：最常用，表示 "is-a" 关系
 *    - 派生类是基类的一种
 *    - 例如：Dog is an Animal
 *
 * 2. protected 继承：较少用，表示 "is-implemented-in-terms-of"
 *    - 派生类使用基类的实现，但不想暴露基类接口
 *    - 用于阻止进一步向下传播基类接口
 *
 * 3. private 继承：表示 "has-a" 或 "is-implemented-in-terms-of"
 *    - 派生类使用基类的功能，但不是基类的一种
 *    - 通常优先使用组合（成员对象）代替 private 继承
 */

// ==================== 7. protected 继承的实际例子 ====================

class DataStream {
public:
    void open() { cout << "Stream opened" << endl; }
    void close() { cout << "Stream closed" << endl; }
    void read() { cout << "Reading data..." << endl; }
    void write() { cout << "Writing data..." << endl; }

protected:
    bool isValid() { return true; }
};

// Protected 继承：只想在内部使用 DataStream，不暴露其接口
class EncryptedStream : protected DataStream {
public:
    // 提供新的、更安全的接口
    void openEncrypted() {
        // ✓ 可以调用基类的 protected 方法
        open();
        cout << "Encryption enabled" << endl;
    }

    void secureRead() {
        if (isValid()) {  // 调用基类 protected 方法
            cout << "Decrypting and reading..." << endl;
            read();  // 调用基类 protected 方法
        }
    }

    // 外部不能直接调用 open(), read() 等原始方法
    // 必须使用我们提供的加密版本
};

// ==================== 8. 对比：使用组合（推荐）====================

class DataStream2 {
public:
    void open() { cout << "Stream2 opened" << endl; }
    void close() { cout << "Stream2 closed" << endl; }
};

// 使用组合替代 private/protected 继承（更清晰）
class SecureStream {
private:
    DataStream2 stream;  // 成员对象

public:
    void openSecure() {
        stream.open();
        cout << "Secure layer enabled" << endl;
    }

    void closeSecure() {
        cout << "Secure layer disabled" << endl;
        stream.close();
    }
};

// ==================== 主函数测试 ====================

int main() {
    cout << "==================== 继承方式对比 ====================" << endl;

    PublicDerived pubD;
    pubD.demonstrate();
    cout << endl;

    ProtectedDerived protD;
    protD.demonstrate();
    cout << endl;

    PrivateDerived privD;
    privD.demonstrate();
    cout << endl;

    demoOutsideAccess();
    cout << endl;

    cout << "==================== Protected 继承示例 ====================" << endl;
    EncryptedStream encStream;
    encStream.openEncrypted();
    encStream.secureRead();

    // 下面这些调用不能编译（protected 继承隐藏了基类接口）
    // encStream.open();  // ✗
    // encStream.read();  // ✗

    cout << "\n==================== 对比：组合方式 ====================" << endl;
    SecureStream secStream;
    secStream.openSecure();
    secStream.closeSecure();

    cout << "\n==================== 总结 ====================" << endl;
    cout << R"(
继承方式选择指南：

1. public 继承（最常用）
   - 表示 "is-a" 关系
   - 保留基类的访问级别
   - 例如：class Dog : public Animal

2. protected 继承（较少用）
   - 表示实现细节
   - 将基类 public 成员变为 protected
   - 阻止进一步暴露基类接口
   - 例如：class SecureStream : protected DataStream

3. private 继承（较少用）
   - 表示 "has-a" 或实现细节
   - 将所有基类成员变为 private
   - 通常优先考虑组合而非 private 继承
   - 例如：class Stack : private Vector

最佳实践：
- 默认使用 public 继承
- 需要 "has-a" 时优先使用组合
- private/protected 继承只在特殊情况下使用
)" << endl;

    return 0;
}

/*
 * 编译运行：
 * g++ -std=c++11 03_protected_inheritance.cpp -o protected_inheritance
 * ./protected_inheritance
 */
