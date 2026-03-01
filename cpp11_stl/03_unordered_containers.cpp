/**
 * STL 无序容器（哈希表）详解
 *
 * 本文件涵盖：
 * 1. std::unordered_set - 无序集合（唯一）
 * 2. std::unordered_map - 无序键值对
 * 3. std::unordered_multiset - 无序集合（允许重复）
 * 4. std::unordered_multimap - 无序键值对（允许重复键）
 *
 * 编译：g++ -std=c++11 03_unordered_containers.cpp -o unordered_containers
 */

#include <iostream>
#include <unordered_set>
#include <unordered_map>
#include <string>
#include <functional>
#include <vector>

using namespace std;

// ==================== 1. std::unordered_set 无序集合 ====================

/*
 * unordered_set 特点：
 * - 基于哈希表实现
 * - 元素无序（不保证顺序）
 * - 元素唯一
 * - 平均 O(1) 的插入/删除/查找
 * - 最坏情况 O(n)（哈希冲突严重时）
 * - 常用方法：
 *   insert(), erase(), find(), count()
 *   load_factor(), max_load_factor(), rehash(), reserve()
 *   size(), empty(), clear()
 *   begin(), end() - 注意迭代顺序可能变化
 */

void demoUnorderedSet() {
    cout << "=== std::unordered_set 示例 ===" << endl;

    // 1. 创建和初始化
    unordered_set<int> us = {5, 2, 8, 1, 9, 2};

    cout << "unordered_set (无序): ";
    for (int x : us) cout << x << " ";
    cout << endl;

    // 2. 基本操作
    us.insert(10);
    us.insert(20);

    auto it = us.find(8);
    if (it != us.end()) {
        cout << "found 8: " << *it << endl;
    }

    cout << "count(5): " << us.count(5) << endl;
    cout << "count(100): " << us.count(100) << endl;

    // 3. 删除
    us.erase(8);

    // 4. 哈希表参数
    cout << "\n哈希表参数:" << endl;
    cout << "bucket count: " << us.bucket_count() << endl;
    cout << "max bucket count: " << us.max_bucket_count() << endl;
    cout << "load factor: " << us.load_factor() << endl;
    cout << "max load factor: " << us.max_load_factor() << endl;

    // 5. 实际应用：去重
    vector<int> vec = {1, 2, 2, 3, 3, 3, 4, 4, 4, 4};
    unordered_set<int> unique(vec.begin(), vec.end());

    cout << "\n原始数组: ";
    for (int x : vec) cout << x << " ";
    cout << "\n去重后: ";
    for (int x : unique) cout << x << " ";
    cout << endl;

    // 6. 实际应用：检测重复
    vector<int> data = {1, 2, 3, 4, 5, 2};
    unordered_set<int> seen;
    bool hasDuplicate = false;

    for (int x : data) {
        if (seen.count(x)) {
            cout << "\n发现重复元素: " << x << endl;
            hasDuplicate = true;
            break;
        }
        seen.insert(x);
    }

    if (!hasDuplicate) {
        cout << "\n没有重复元素" << endl;
    }

    // 7. 预留空间优化
    unordered_set<int> optimized;
    optimized.reserve(1000);  // 预留空间，减少 rehash
    for (int i = 0; i < 1000; ++i) {
        optimized.insert(i);
    }
}

// ==================== 2. std::unordered_map 无序键值对 ====================

/*
 * unordered_map 特点：
 * - 基于哈希表的键值对容器
 * - 键无序
 * - 平均 O(1) 的插入/删除/查找
 * - 常用方法：
 *   operator[], at() - 访问元素
 *   insert(), erase(), find(), count()
 *   与 unordered_set 类似的接口
 */

void demoUnorderedMap() {
    cout << "\n=== std::unordered_map 示例 ===" << endl;

    // 1. 创建和初始化
    unordered_map<string, int> um = {
        {"apple", 5},
        {"banana", 3},
        {"cherry", 8}
    };

    // 2. 访问和修改
    um["apple"] = 10;
    um["date"] = 15;

    cout << "map 内容:" << endl;
    for (const auto& p : um) {
        cout << "  " << p.first << " -> " << p.second << endl;
    }

    // 3. 查找
    auto it = um.find("banana");
    if (it != um.end()) {
        cout << "\nfound: " << it->first << " -> " << it->second << endl;
    }

    // 4. 实际应用：缓存/备忘录
    cout << "\n=== 斐波那契缓存示例 ===" << endl;

    unordered_map<int, long long> fibCache;
    fibCache[0] = 0;
    fibCache[1] = 1;

    function<long long(int)> fibonacci = [&](int n) -> long long {
        auto it = fibCache.find(n);
        if (it != fibCache.end()) {
            return it->second;  // 缓存命中
        }
        long long result = fibonacci(n - 1) + fibonacci(n - 2);
        fibCache[n] = result;  // 存入缓存
        return result;
    };

    cout << "fib(40) = " << fibonacci(40) << endl;
    cout << "缓存大小: " << fibCache.size() << endl;

    // 5. 实际应用：计数器
    cout << "\n=== 字符计数 ===" << endl;
    string str = "hello world";
    unordered_map<char, int> charCount;

    for (char c : str) {
        charCount[c]++;
    }

    for (const auto& p : charCount) {
        cout << "'" << p.first << "': " << p.second << endl;
    }

    // 6. 实际应用：图表示（邻接表）
    cout << "\n=== 图的邻接表表示 ===" << endl;

    unordered_map<string, vector<string>> graph = {
        {"A", {"B", "C"}},
        {"B", {"D", "E"}},
        {"C", {"F"}},
        {"D", {}},
        {"E", {"F"}},
        {"F", {}}
    };

    cout << "图结构:" << endl;
    for (const auto& node : graph) {
        cout << node.first << " -> ";
        for (const auto& neighbor : node.second) {
            cout << neighbor << " ";
        }
        cout << endl;
    }
}

// ==================== 3. 自定义哈希函数 ====================

// 自定义结构体
struct Person {
    string name;
    int age;

    bool operator==(const Person& other) const {
        return name == other.name && age == other.age;
    }
};

// 自定义哈希函数
namespace std {
    template<>
    struct hash<Person> {
        size_t operator()(const Person& p) const {
            return hash<string>()(p.name) ^ hash<int>()(p.age);
        }
    };
}

void demoCustomHash() {
    cout << "\n=== 自定义哈希函数示例 ===" << endl;

    unordered_set<Person> people;

    people.insert({"Alice", 25});
    people.insert({"Bob", 30});
    people.insert({"Charlie", 35});

    cout << "Person set 大小: " << people.size() << endl;

    Person p = {"Alice", 25};
    if (people.find(p) != people.end()) {
        cout << "found Alice" << endl;
    }

    // 使用 lambda 作为哈希函数（C++11 方式）
    auto personHash = [](const Person& p) {
        return hash<string>()(p.name) ^ hash<int>()(p.age);
    };

    auto personEqual = [](const Person& a, const Person& b) {
        return a.name == b.name && a.age == b.age;
    };

    using PersonSet = unordered_set<Person, decltype(personHash), decltype(personEqual)>;
    PersonSet customPeople(10, personHash, personEqual);

    customPeople.insert({"David", 40});
    cout << "自定义 set 大小: " << customPeople.size() << endl;
}

// ==================== 4. unordered_multiset 和 unordered_multimap ====================

/*
 * 允许重复键的无序容器
 * - 与 multiset/multimap 类似，但无序
 * - 使用哈希表实现
 */

void demoUnorderedMulti() {
    cout << "\n=== 无序多重容器示例 ===" << endl;

    // unordered_multiset
    unordered_multiset<int> ums = {1, 2, 2, 3, 3, 3};
    cout << "unordered_multiset: ";
    for (int x : ums) cout << x << " ";
    cout << endl;
    cout << "count(3): " << ums.count(3) << endl;

    // unordered_multimap
    unordered_multimap<string, int> umm;
    umm.insert({"A", 1});
    umm.insert({"A", 2});
    umm.insert({"B", 3});

    cout << "\nunordered_multimap:" << endl;
    for (const auto& p : umm) {
        cout << p.first << " -> " << p.second << endl;
    }

    cout << "count('A'): " << umm.count("A") << endl;

    // 获取所有 'A' 的值
    auto range = umm.equal_range("A");
    cout << "All values for 'A': ";
    for (auto it = range.first; it != range.second; ++it) {
        cout << it->second << " ";
    }
    cout << endl;
}

// ==================== 主函数 ====================

int main() {
    demoUnorderedSet();
    demoUnorderedMap();
    demoCustomHash();
    demoUnorderedMulti();

    cout << "\n=== 容器选择总结 ===" << endl;
    cout << R"(
┌──────────────────┬──────────┬──────────┬──────────┬──────────┐
│     容器         │   排序   │  唯一键  │ 允许重复 │ 平均复杂度│
├──────────────────┼──────────┼──────────┼──────────┼──────────┤
│ set              │   是     │   ✓      │   ✗      │  O(log n)│
│ map              │   是     │   ✓      │   ✗      │  O(log n)│
│ multiset         │   是     │   ✗      │   ✓      │  O(log n)│
│ multimap         │   是     │   ✗      │   ✓      │  O(log n)│
│ unordered_set    │   否     │   ✓      │   ✗      │   O(1)   │
│ unordered_map    │   否     │   ✓      │   ✗      │   O(1)   │
│ unordered_multiset│   否     │   ✗      │   ✓      │   O(1)   │
│ unordered_multimap│   否     │   ✗      │   ✓      │   O(1)   │
└──────────────────┴──────────┴──────────┴──────────┴──────────┘

选择建议：
1. 需要有序遍历：用 set/map/multiset/multimap
2. 只需要查找，不需要顺序：用 unordered_set/unordered_map（更快）
3. 需要键值对：用 map 系列
4. 只需要值：用 set 系列
5. 允许重复：用 multi 版本

性能优化：
- unordered 容器：使用 reserve() 预留空间
- 有序容器：批量插入后统一排序
- 自定义类型：提供高效的哈希函数
)" << endl;

    return 0;
}
