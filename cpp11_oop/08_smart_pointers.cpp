/**
 * C++11 智能指针详解
 *
 * 学习目标：
 * 1. 理解为什么需要智能指针
 * 2. 掌握 unique_ptr、shared_ptr、weak_ptr 的使用
 * 3. 理解智能指针的适用场景
 */

#include <iostream>
#include <memory>
#include <vector>
#include <string>
using namespace std;

// ==================== 1. 为什么需要智能指针？ ====================

/*
 * 传统裸指针的问题：
 * 1. 忘记 delete → 内存泄漏
 * 2. 异常发生 → 内存泄漏
 * 3. 多次 delete → 未定义行为
 * 4. 悬空指针 → 访问已释放的内存
 *
 * 智能指针的解决方案：
 * - RAII（资源获取即初始化）
 * - 自动管理内存
 * - 异常安全
 */

// 传统方式的问题示例
class BadExample {
private:
    int* data;  // 裸指针

public:
    BadExample(size_t size) : data(new int[size]) {
        cout << "BadExample: 分配内存" << endl;
    }

    // 如果忘记写析构函数，就会内存泄漏！
    ~BadExample() {
        delete[] data;
        cout << "BadExample: 释放内存" << endl;
    }

    // 还需要正确实现拷贝构造和拷贝赋值（否则会有问题）
};

// 使用智能指针的正确示例
class GoodExample {
private:
    unique_ptr<int[]> data;  // 智能指针

public:
    GoodExample(size_t size) : data(unique_ptr<int[]>(new int[size])) {
        cout << "GoodExample: 分配内存（自动管理）" << endl;
    }

    // 不需要写析构函数！智能指针自动释放
    ~GoodExample() {
        cout << "GoodExample: 自动释放内存" << endl;
    }

    // 不需要实现拷贝/移动操作，编译器生成的版本就是正确的
};

// ==================== 2. unique_ptr ====================

/*
 * unique_ptr 特点：
 * - 独占所有权
 * - 不可拷贝，只能移动
 * - 性能开销几乎为零
 * - 最常用的智能指针
 */

void demoUniquePtr() {
    cout << "=== unique_ptr 基本用法 ===" << endl;

    // 创建 unique_ptr
    unique_ptr<int> p1(new int(42));
    cout << "*p1 = " << *p1 << endl;

    // 推荐：使用 make_unique（C++14）
    // auto p2 = make_unique<int>(100);

    // unique_ptr 不可拷贝
    // unique_ptr<int> p3 = p1;  // ✗ 编译错误

    // unique_ptr 可以移动
    unique_ptr<int> p4 = move(p1);
    // cout << "*p1 = " << *p1;  // ✗ p1 已经是 nullptr
    cout << "*p4 = " << *p4 << endl;

    // 用于数组
    unique_ptr<int[]> arr(new int[5]{1, 2, 3, 4, 5});
    cout << "arr[0] = " << arr[0] << endl;

    // 自定义删除器
    auto fileDeleter = [](FILE* f) {
        if (f) {
            cout << "关闭文件" << endl;
            fclose(f);
        }
    };
    unique_ptr<FILE, decltype(fileDeleter)> file(fopen("test.txt", "w"), fileDeleter);

    cout << endl;
}

// ==================== 3. unique_ptr 与函数 ====================

unique_ptr<int> createInt() {
    return unique_ptr<int>(new int(100));  // 返回 unique_ptr
}

void processUniquePtr(unique_ptr<int> p) {
    cout << "处理值: " << *p << endl;
    // p 离开作用域时自动释放
}

void demoUniquePtrWithFunctions() {
    cout << "=== unique_ptr 与函数 ===" << endl;

    // 工厂函数返回 unique_ptr
    auto p = createInt();
    cout << "创建的值: " << *p << endl;

    // 传递所有权（使用 move）
    processUniquePtr(move(p));
    // cout << *p;  // ✗ p 已经是 nullptr

    cout << endl;
}

// ==================== 4. shared_ptr ====================

/*
 * shared_ptr 特点：
 * - 共享所有权
 * - 可以拷贝
 * - 内部使用引用计数
 * - 有一定性能开销
 */

void demoSharedPtr() {
    cout << "=== shared_ptr 基本用法 ===" << endl;

    // 创建 shared_ptr
    shared_ptr<int> p1(new int(42));
    cout << "*p1 = " << *p1 << ", use_count = " << p1.use_count() << endl;

    // 推荐：使用 make_shared（更高效）
    shared_ptr<int> p2(new int(100));
    cout << "*p2 = " << *p2 << ", use_count = " << p2.use_count() << endl;

    // shared_ptr 可以拷贝
    auto p3 = p2;  // 引用计数 +1
    cout << "拷贝后 p2.use_count() = " << p2.use_count() << endl;

    {
        auto p4 = p2;  // 引用计数 +1
        cout << "再次拷贝后 p2.use_count() = " << p2.use_count() << endl;
    }  // p4 离开作用域，引用计数 -1

    cout << "p4 离开作用域后 p2.use_count() = " << p2.use_count() << endl;

    cout << endl;
}

// ==================== 5. shared_ptr 的陷阱 ====================

/*
 * shared_ptr 的常见问题：
 * 1. 循环引用 → 内存泄漏
 * 2. 不要用同一个裸指针创建多个 shared_ptr
 */

// 循环引用示例
class Node {
public:
    shared_ptr<Node> next;
    string name;

    Node(const string& n) : name(n) {
        cout << "Node " << name << " 构造" << endl;
    }

    ~Node() {
        cout << "Node " << name << " 析构" << endl;
    }
};

void demoCircularReference() {
    cout << "=== 循环引用问题 ===" << endl;

    auto node1 = make_shared<Node>("A");
    auto node2 = make_shared<Node>("B");

    node1->next = node2;  // node2 引用计数 = 2
    node2->next = node1;  // node1 引用计数 = 2

    cout << "node1.use_count() = " << node1.use_count() << endl;
    cout << "node2.use_count() = " << node2.use_count() << endl;

    // 离开作用域时，两个 node 的引用计数都变为 1
    // 不会析构！内存泄漏！

    cout << endl;
}

// ==================== 6. weak_ptr ====================

/*
 * weak_ptr 特点：
 * - 不增加引用计数
 * - 用于解决循环引用
 * - 不能直接访问对象，需要 lock()
 */

// 使用 weak_ptr 解决循环引用
class SafeNode {
public:
    shared_ptr<SafeNode> next;
    weak_ptr<SafeNode> prev;  // 使用 weak_ptr 打破循环
    string name;

    SafeNode(const string& n) : name(n) {
        cout << "SafeNode " << name << " 构造" << endl;
    }

    ~SafeNode() {
        cout << "SafeNode " << name << " 析构" << endl;
    }

    void showPrev() {
        if (auto p = prev.lock()) {  // 尝试获取 shared_ptr
            cout << name << " 的前一个节点: " << p->name << endl;
        } else {
            cout << name << " 没有前一个节点或已释放" << endl;
        }
    }
};

void demoWeakPtr() {
    cout << "=== weak_ptr 解决循环引用 ===" << endl;

    auto node1 = make_shared<SafeNode>("A");
    auto node2 = make_shared<SafeNode>("B");

    node1->next = node2;
    node2->prev = node1;  // weak_ptr 不增加引用计数

    cout << "node1.use_count() = " << node1.use_count() << endl;

    node2->showPrev();

    // 离开作用域时，正常析构！

    cout << endl;
}

// ==================== 7. 智能指针与容器 ====================

void demoSmartPointersWithContainers() {
    cout << "=== 智能指针与容器 ===" << endl;

    vector<unique_ptr<int>> vec;

    // 添加元素
    vec.emplace_back(unique_ptr<int>(new int(1)));
    vec.emplace_back(unique_ptr<int>(new int(2)));
    vec.emplace_back(unique_ptr<int>(new int(3)));

    // 访问元素
    cout << "vec[0] = " << *vec[0] << endl;

    // 移动元素
    auto p = move(vec.back());
    vec.pop_back();

    cout << "*p = " << *p << endl;
    cout << "vec size: " << vec.size() << endl;

    cout << endl;
}

// ==================== 8. 智能指针与多态 ====================

class Animal {
public:
    virtual void makeSound() = 0;
    virtual ~Animal() = default;
};

class Dog : public Animal {
public:
    void makeSound() override {
        cout << "Dog: Woof!" << endl;
    }
};

class Cat : public Animal {
public:
    void makeSound() override {
        cout << "Cat: Meow!" << endl;
    }
};

void demoPolymorphism() {
    cout << "=== 智能指针与多态 ===" << endl;

    vector<shared_ptr<Animal>> zoo;

    zoo.push_back(make_shared<Dog>());
    zoo.push_back(make_shared<Cat>());
    zoo.push_back(make_shared<Dog>());

    for (const auto& animal : zoo) {
        animal->makeSound();
    }

    cout << endl;
}

// ==================== 9. this 返回 shared_ptr ====================

/*
 * enable_shared_from_this：
 * - 允许在成员函数中获取指向自己的 shared_ptr
 * - 避免用 this 创建新的 shared_ptr（会导致双重释放）
 */

class MyClass : public enable_shared_from_this<MyClass> {
public:
    shared_ptr<MyClass> getShared() {
        return shared_from_this();  // 正确！
        // return shared_ptr<MyClass>(this);  // ✗ 错误！会导致双重释放
    }

    ~MyClass() {
        cout << "MyClass 析构" << endl;
    }
};

void demoEnableSharedFromThis() {
    cout << "=== enable_shared_from_this ===" << endl;

    auto obj = make_shared<MyClass>();
    auto shared = obj->getShared();

    cout << "obj.use_count() = " << obj.use_count() << endl;

    cout << endl;
}

// ==================== 10. 最佳实践总结 ====================

void demoBestPractices() {
    cout << "=== 智能指针最佳实践 ===" << endl;

    // 1. 优先使用 unique_ptr（默认选择）
    auto unique = unique_ptr<int>(new int(42));

    // 2. 需要共享所有权时使用 shared_ptr
    auto shared = make_shared<int>(100);
    auto shared2 = shared;  // 多个所有者

    // 3. 循环引用时使用 weak_ptr
    auto node = make_shared<SafeNode>("test");
    weak_ptr<SafeNode> weak = node;  // 不增加引用计数

    // 4. 使用 make_shared/make_unique（更高效，异常安全）
    auto p1 = make_shared<int>(42);
    // auto p2 = shared_ptr<int>(new int(42));  // 不推荐

    // 5. 不要用同一个裸指针创建多个智能指针
    // int* raw = new int(42);
    // auto sp1 = shared_ptr<int>(raw);
    // auto sp2 = shared_ptr<int>(raw);  // ✗ 错误！

    cout << endl;
}

// ==================== 主函数测试 ====================

int main() {
    cout << "==================== C++11 智能指针详解 ====================" << endl;

    demoUniquePtr();
    demoUniquePtrWithFunctions();
    demoSharedPtr();
    demoCircularReference();
    demoWeakPtr();
    demoSmartPointersWithContainers();
    demoPolymorphism();
    demoEnableSharedFromThis();
    demoBestPractices();

    cout << "==================== 总结 ====================" << endl;
    cout << R"(
智能指针总结：

1. 为什么使用智能指针：
   - 自动管理内存，避免内存泄漏
   - 异常安全
   - 明确所有权语义

2. unique_ptr（独占所有权）：
   - 性能最好，无额外开销
   - 不可拷贝，只能移动
   - 默认首选

3. shared_ptr（共享所有权）：
   - 内部使用引用计数
   - 可以拷贝
   - 有一定性能开销
   - 注意循环引用

4. weak_ptr（弱引用）：
   - 不增加引用计数
   - 用于解决循环引用
   - 不能直接访问，需要 lock()

5. 选择指南：
   - 默认使用 unique_ptr
   - 需要共享时用 shared_ptr
   - 循环引用时配合 weak_ptr
   - 优先使用 make_xxx 函数创建

6. 注意事项：
   - 不要混用裸指针和智能指针
   - 不要用 this 创建 shared_ptr（使用 enable_shared_from_this）
   - 智能指针管理的对象应该尽量在堆上
)" << endl;

    return 0;
}

/*
 * 编译运行：
 * g++ -std=c++11 08_smart_pointers.cpp -o smart_pointers
 * ./smart_pointers
 *
 * 注：make_unique 需要 C++14，使用 -std=c++14 编译
 */
