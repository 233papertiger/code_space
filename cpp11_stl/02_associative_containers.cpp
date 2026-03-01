/**
 * STL 关联容器详解
 *
 * 本文件涵盖：
 * 1. std::set - 有序集合（唯一）
 * 2. std::map - 有序键值对
 * 3. std::multiset - 有序集合（允许重复）
 * 4. std::multimap - 有序键值对（允许重复键）
 *
 * 编译：g++ -std=c++11 02_associative_containers.cpp -o associative_containers
 */

#include <iostream>
#include <set>
#include <map>
#include <string>
#include <algorithm>

using namespace std;

// ==================== 1. std::set 有序集合 ====================

/*
 * set 特点：
 * - 基于红黑树实现
 * - 元素自动排序（默认升序）
 * - 元素唯一，自动去重
 * - 插入/删除/查找 O(log n)
 * - 常用方法：
 *   insert(), erase(), find(), count()
 *   lower_bound(), upper_bound(), equal_range()
 *   size(), empty(), clear()
 *   begin(), end(), rbegin(), rend()
 */

void demoSet() {
    cout << "=== std::set 示例 ===" << endl;

    // 1. 创建和初始化
    set<int> s1;                              // 空 set
    set<int> s2 = {5, 2, 8, 1, 9, 2};         // 自动去重和排序
    set<int> s3(s2.begin(), s2.end());        // 迭代器范围

    cout << "s2 (自动排序去重): ";
    for (int x : s2) cout << x << " ";
    cout << endl;

    // 2. 插入元素
    s1.insert(10);
    s1.insert(20);
    s1.insert(15);
    s1.insert(10);  // 重复元素不会被插入

    cout << "s1: ";
    for (int x : s1) cout << x << " ";
    cout << endl;

    // 3. 查找元素
    auto it = s1.find(15);
    if (it != s1.end()) {
        cout << "found 15" << endl;
    }

    // count 返回 0 或 1（set 中元素唯一）
    cout << "count(10): " << s1.count(10) << endl;
    cout << "count(100): " << s1.count(100) << endl;

    // 4. 删除元素
    s1.erase(15);  // 删除值为 15 的元素
    size_t removed = s1.erase(100);  // 删除不存在的元素
    cout << "removed count: " << removed << endl;

    // 5. 范围查询
    set<int> s = {1, 3, 5, 7, 9, 11, 13};

    // lower_bound: 第一个 >= x 的元素
    auto lb = s.lower_bound(5);
    cout << "lower_bound(5): " << *lb << endl;

    // upper_bound: 第一个 > x 的元素
    auto ub = s.upper_bound(5);
    cout << "upper_bound(5): " << *ub << endl;

    // 6. 自定义排序
    // 降序 set
    set<int, greater<int>> descSet = {1, 2, 3, 4, 5};
    cout << "降序 set: ";
    for (int x : descSet) cout << x << " ";
    cout << endl;

    // 7. 存储自定义对象
    struct Person {
        string name;
        int age;
        bool operator<(const Person& other) const {
            return age < other.age;
        }
    };

    set<Person> people = {{"Alice", 25}, {"Bob", 30}, {"Charlie", 20}};
    cout << "\n按年龄排序的 Person set:" << endl;
    for (const auto& p : people) {
        cout << p.name << ": " << p.age << endl;
    }
}

// ==================== 2. std::map 有序键值对 ====================

/*
 * map 特点：
 * - 键值对容器，键唯一且自动排序
 * - 基于红黑树实现
 * - 插入/删除/查找 O(log n)
 * - 常用方法：
 *   insert(), erase(), find(), count()
 *   operator[], at() - 访问元素
 *   lower_bound(), upper_bound()
 *   size(), empty(), clear()
 */

void demoMap() {
    cout << "\n=== std::map 示例 ===" << endl;

    // 1. 创建和初始化
    map<string, int> m1;
    map<string, int> m2 = {{"apple", 5}, {"banana", 3}, {"cherry", 8}};

    // 2. 插入元素
    m1["one"] = 1;           // operator[] 插入
    m1["two"] = 2;
    m1["three"] = 3;

    m1.insert({"four", 4});  // insert 插入
    m1.insert(make_pair("five", 5));

    cout << "map 内容:" << endl;
    for (const auto& p : m1) {
        cout << "  " << p.first << " -> " << p.second << endl;
    }

    // 3. 访问元素
    cout << "\nm1[\"two\"]: " << m1["two"] << endl;

    // at() 会检查键是否存在
    try {
        cout << "m1.at(\"one\"): " << m1.at("one") << endl;
        // cout << m1.at("nonexistent");  // 抛出 out_of_range 异常
    } catch (const out_of_range& e) {
        cout << "Key not found" << endl;
    }

    // 4. 查找元素
    auto it = m1.find("three");
    if (it != m1.end()) {
        cout << "found: " << it->first << " -> " << it->second << endl;
    }

    // count 返回 0 或 1
    cout << "count(\"one\"): " << m1.count("one") << endl;

    // 5. 修改值
    m1["one"] = 100;  // 修改已存在的键
    cout << "\n修改后 m1[\"one\"]: " << m1["one"] << endl;

    // 6. 删除元素
    m1.erase("two");

    // 7. 实际应用：词频统计
    cout << "\n=== 词频统计示例 ===" << endl;
    string text = "hello world hello c++ hello map";
    map<string, int> wordCount;

    string word;
    for (char c : text) {
        if (c == ' ') {
            if (!word.empty()) {
                wordCount[word]++;
                word.clear();
            }
        } else {
            word += c;
        }
    }
    if (!word.empty()) {
        wordCount[word]++;
    }

    cout << "词频统计结果:" << endl;
    for (const auto& p : wordCount) {
        cout << "  " << p.first << ": " << p.second << endl;
    }

    // 8. 实际应用：学生成绩管理
    map<string, map<string, int>> grades = {
        {"Alice", {{"Math", 90}, {"English", 85}}},
        {"Bob", {{"Math", 78}, {"English", 92}}}
    };

    cout << "\n学生成绩:" << endl;
    for (const auto& student : grades) {
        cout << student.first << ": ";
        for (const auto& course : student.second) {
            cout << course.first << "=" << course.second << " ";
        }
        cout << endl;
    }
}

// ==================== 3. std::multiset 允许重复的集合 ====================

/*
 * multiset 特点：
 * - 允许重复元素
 * - 自动排序
 * - count() 可以返回大于 1 的值
 * - find() 返回第一个匹配的元素
 */

void demoMultiset() {
    cout << "\n=== std::multiset 示例 ===" << endl;

    multiset<int> ms = {1, 2, 2, 3, 3, 3, 4};

    cout << "multiset (允许重复): ";
    for (int x : ms) cout << x << " ";
    cout << endl;

    // count 可以大于 1
    cout << "count(3): " << ms.count(3) << endl;

    // equal_range 获取所有等于某值的元素范围
    auto range = ms.equal_range(3);
    cout << "equal_range(3): ";
    for (auto it = range.first; it != range.second; ++it) {
        cout << *it << " ";
    }
    cout << endl;

    // 删除某个值的全部元素
    size_t count = ms.erase(3);
    cout << "删除了 " << count << " 个 3" << endl;

    cout << "after erase(3): ";
    for (int x : ms) cout << x << " ";
    cout << endl;

    // 实际应用：处理带时间戳的事件
    struct Event {
        int timestamp;
        string description;

        bool operator<(const Event& other) const {
            return timestamp < other.timestamp;
        }
    };

    multiset<Event> events = {
        {1, "start"},
        {2, "process A"},
        {2, "process B"},
        {3, "end"}
    };

    cout << "\n时间轴事件:" << endl;
    for (const auto& e : events) {
        cout << "t=" << e.timestamp << ": " << e.description << endl;
    }
}

// ==================== 4. std::multimap 允许重复键的映射 ====================

/*
 * multimap 特点：
 * - 允许重复的键
 * - 没有 operator[] 和 at()
 * - 常用 insert() 插入
 * - 使用 equal_range() 获取所有相同键的元素
 */

void demoMultimap() {
    cout << "\n=== std::multimap 示例 ===" << endl;

    multimap<string, string> phonebook;

    // 同一个人可以有多个电话号码
    phonebook.insert({"Alice", "123-456-7890"});
    phonebook.insert({"Bob", "987-654-3210"});
    phonebook.insert({"Alice", "111-222-3333"});
    phonebook.insert({"Alice", "444-555-6666"});

    cout << "电话簿内容:" << endl;
    for (const auto& entry : phonebook) {
        cout << "  " << entry.first << ": " << entry.second << endl;
    }

    // 查找某人的所有号码
    string name = "Alice";
    cout << "\n" << name << " 的电话号码:" << endl;
    auto range = phonebook.equal_range(name);
    for (auto it = range.first; it != range.second; ++it) {
        cout << "  " << it->second << endl;
    }

    // count 获取键的数量
    cout << "Alice 有 " << phonebook.count("Alice") << " 个号码" << endl;

    // 实际应用：区间查询
    multimap<int, string> schedule = {
        {9, "Meeting A"},
        {10, "Task 1"},
        {10, "Task 2"},
        {11, "Lunch"},
        {14, "Review"}
    };

    cout << "\n10 点的任务:" << endl;
    auto tasks = schedule.equal_range(10);
    for (auto it = tasks.first; it != tasks.second; ++it) {
        cout << "  " << it->second << endl;
    }
}

// ==================== 主函数 ====================

int main() {
    demoSet();
    demoMap();
    demoMultiset();
    demoMultimap();

    cout << "\n=== 容器选择总结 ===" << endl;
    cout << R"(
┌─────────────┬──────────┬──────────┬──────────┐
│   容器      │  唯一键  │ 允许重复 │   排序   │
├─────────────┼──────────┼──────────┼──────────┤
│ set         │    ✓     │    ✗     │   是     │
│ map         │    ✓     │    ✗     │   是     │
│ multiset    │    ✗     │    ✓     │   是     │
│ multimap    │    ✗     │    ✓     │   是     │
└─────────────┴──────────┴──────────┴──────────┘

使用建议：
- 需要唯一键 + 自动排序：用 set/map
- 允许重复键 + 自动排序：用 multiset/multimap
- 需要键值对：用 map/multimap
- 只需要值：用 set/multiset
- 不需要排序：用 unordered_map/unordered_set（哈希表）
)" << endl;

    return 0;
}
