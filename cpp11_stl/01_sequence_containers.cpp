/**
 * STL 序列容器详解
 *
 * 本文件涵盖：
 * 1. std::vector - 动态数组
 * 2. std::deque - 双端队列
 * 3. std::list - 双向链表
 * 4. std::forward_list - 单向链表
 *
 * 编译：g++ -std=c++11 01_sequence_containers.cpp -o sequence_containers
 */

#include <iostream>
#include <vector>
#include <deque>
#include <list>
#include <forward_list>
#include <algorithm>
#include <string>

using namespace std;

// ==================== 1. std::vector 动态数组 ====================

/*
 * vector 特点：
 * - 连续内存存储，支持随机访问 O(1)
 * - 尾部插入/删除效率高 O(1)，中间/头部插入效率低 O(n)
 * - 自动扩容（通常 2 倍）
 * - 常用方法：
 *   push_back(), pop_back(), emplace_back()
 *   size(), capacity(), reserve(), resize()
 *   operator[], at(), front(), back()
 *   begin(), end(), rbegin(), rend()
 *   insert(), erase(), clear()
 */

void demoVector() {
    cout << "=== std::vector 示例 ===" << endl;

    // 1. 创建和初始化
    vector<int> v1;                       // 空 vector
    vector<int> v2(5);                    // 5 个元素，默认值 0
    vector<int> v3(5, 100);               // 5 个元素，值都是 100
    vector<int> v4 = {1, 2, 3, 4, 5};     // 初始化列表
    vector<int> v5(v4.begin(), v4.end()); // 迭代器范围

    // 2. 添加元素
    v1.push_back(10);     // 尾部添加
    v1.push_back(20);
    v1.emplace_back(30);  // 原地构造，更高效

    cout << "v1: ";
    for (int x : v1) cout << x << " ";
    cout << endl;

    // 3. 访问元素
    cout << "v1[0]: " << v1[0] << endl;           // 不检查边界
    cout << "v1.at(1): " << v1.at(1) << endl;     // 检查边界，越界抛异常
    cout << "v1.front(): " << v1.front() << endl; // 第一个元素
    cout << "v1.back(): " << v1.back() << endl;   // 最后一个元素

    // 4. 容量管理
    cout << "size: " << v1.size() << endl;        // 元素个数
    cout << "capacity: " << v1.capacity() << endl; // 容量

    v1.reserve(20);  // 预留空间，避免多次扩容
    cout << "after reserve(20), capacity: " << v1.capacity() << endl;

    v1.resize(10);   // 调整大小，新增元素用默认值
    cout << "after resize(10), size: " << v1.size() << endl;

    // 5. 插入和删除
    vector<int> v = {1, 2, 3, 4, 5};
    v.insert(v.begin() + 2, 99);  // 在位置 2 插入 99
    // v: {1, 2, 99, 3, 4, 5}

    v.erase(v.begin() + 1);  // 删除位置 1 的元素
    // v: {1, 99, 3, 4, 5}

    v.pop_back();  // 删除最后一个元素

    cout << "after modifications: ";
    for (int x : v) cout << x << " ";
    cout << endl;

    // 6. 清空和判断
    cout << "empty: " << v.empty() << endl;
    v.clear();
    cout << "after clear, empty: " << v.empty() << endl;

    // 7. 二维 vector（常用）
    vector<vector<int>> matrix = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}
    };

    cout << "\n二维 vector:" << endl;
    for (const auto& row : matrix) {
        for (int x : row) {
            cout << x << " ";
        }
        cout << endl;
    }
}

// ==================== 2. std::deque 双端队列 ====================

/*
 * deque 特点：
 * - 双端队列，支持头部和尾部高效插入 O(1)
 * - 支持随机访问 O(1)
 * - 内存分段连续（非完全连续）
 * - 比 vector 多了 push_front(), pop_front()
 * - 中间插入仍然是 O(n)
 * - 常用方法：
 *   push_back(), push_front(), pop_back(), pop_front()
 *   operator[], at(), front(), back()
 *   与 vector 类似的接口
 */

void demoDeque() {
    cout << "\n=== std::deque 示例 ===" << endl;

    deque<int> d = {2, 3, 4};

    // 双端操作
    d.push_front(1);   // 头部插入: {1, 2, 3, 4}
    d.push_back(5);    // 尾部插入: {1, 2, 3, 4, 5}

    cout << "deque: ";
    for (int x : d) cout << x << " ";
    cout << endl;

    // 双端删除
    d.pop_front();     // 删除头部: {2, 3, 4, 5}
    d.pop_back();      // 删除尾部: {2, 3, 4}

    cout << "after pop: ";
    for (int x : d) cout << x << " ";
    cout << endl;

    // 随机访问
    cout << "d[1]: " << d[1] << endl;

    // 实际应用：滑动窗口
    deque<int> window;
    int data[] = {1, 3, -1, -3, 5, 3, 6, 7};
    int k = 3;  // 窗口大小

    cout << "\n滑动窗口最大值: ";
    for (int i = 0; i < 8; ++i) {
        // 移除窗口外的元素
        while (!window.empty() && window.front() <= i - k) {
            window.pop_front();
        }
        // 移除比当前元素小的元素
        while (!window.empty() && data[window.back()] < data[i]) {
            window.pop_back();
        }
        window.push_back(i);
        if (i >= k - 1) {
            cout << data[window.front()] << " ";
        }
    }
    cout << endl;
}

// ==================== 3. std::list 双向链表 ====================

/*
 * list 特点：
 * - 双向链表，非连续内存
 * - 任意位置插入/删除 O(1)（已知位置）
 * - 不支持随机访问，无 operator[]
 * - 插入/删除不使迭代器失效（除了被删除的）
 * - 常用方法：
 *   push_back(), push_front(), pop_back(), pop_front()
 *   insert(), erase(), splice()
 *   merge(), sort(), unique(), reverse()
 *   front(), back(), begin(), end()
 */

void demoList() {
    cout << "\n=== std::list 示例 ===" << endl;

    // 1. 基本操作
    list<int> lst = {3, 1, 4, 1, 5};

    lst.push_front(0);   // 头部插入
    lst.push_back(6);    // 尾部插入

    cout << "list: ";
    for (int x : lst) cout << x << " ";
    cout << endl;

    // 2. 插入和删除
    auto it = lst.begin();
    advance(it, 2);      // 移动迭代器到位置 2
    lst.insert(it, 99);  // 在位置 2 插入

    lst.remove(1);       // 删除所有值为 1 的元素

    cout << "after insert and remove: ";
    for (int x : lst) cout << x << " ";
    cout << endl;

    // 3. 链表特有操作
    list<int> lst1 = {1, 3, 5};
    list<int> lst2 = {2, 4, 6};

    lst1.merge(lst2);    // 合并两个有序链表
    cout << "after merge: ";
    for (int x : lst1) cout << x << " ";
    cout << endl;

    // 4. 排序和去重
    list<int> unsorted = {5, 2, 8, 1, 9, 3};
    unsorted.sort();     // 链表专用的排序（比 std::sort 更高效）
    cout << "sorted: ";
    for (int x : unsorted) cout << x << " ";
    cout << endl;

    unsorted.unique();   // 删除连续的重复元素
    unsorted.reverse();  // 反转链表

    // 5. splice - 转移元素
    list<int> l1 = {1, 2, 3};
    list<int> l2 = {4, 5, 6};

    auto pos = l1.begin();
    l1.splice(pos, l2);  // 将 l2 全部插入到 l1 的 pos 位置

    cout << "after splice l1: ";
    for (int x : l1) cout << x << " ";
    cout << endl;
    cout << "l2 is now empty: " << l2.empty() << endl;
}

// ==================== 4. std::forward_list 单向链表 ====================

/*
 * forward_list 特点：
 * - 单向链表，比 list 更省内存
 * - 只能向前遍历
 * - 没有 size() 方法（需要遍历计数）
 * - 插入操作在位置之后而非之前
 * - 常用方法：
 *   push_front(), pop_front()
 *   insert_after(), emplace_after(), erase_after()
 *   front(), before_begin(), cbefore_begin()
 */

void demoForwardList() {
    cout << "\n=== std::forward_list 示例 ===" << endl;

    forward_list<int> fl = {1, 2, 3, 4, 5};

    // 头部操作
    fl.push_front(0);

    cout << "forward_list: ";
    for (int x : fl) cout << x << " ";
    cout << endl;

    // 插入在位置之后
    auto it = fl.begin();
    advance(it, 2);              // 指向第 3 个元素
    fl.insert_after(it, 99);     // 在其后插入

    cout << "after insert_after: ";
    for (int x : fl) cout << x << " ";
    cout << endl;

    // 删除位置之后的元素
    fl.erase_after(it);

    cout << "after erase_after: ";
    for (int x : fl) cout << x << " ";
    cout << endl;

    // 在开头之前插入（特殊的 before_begin）
    fl.insert_after(fl.before_begin(), -1);

    cout << "after insert at beginning: ";
    for (int x : fl) cout << x << " ";
    cout << endl;
}

// ==================== 5. 容器选择指南 ====================

void demoContainerSelection() {
    cout << "\n=== 容器选择对比 ===" << endl;

    cout << R"(
┌──────────────┬──────────┬──────────┬──────────┬──────────┐
│    操作      │  vector  │  deque   │   list   │forward_  │
│              │          │          │          │  list    │
├──────────────┼──────────┼──────────┼──────────┼──────────┤
│ 随机访问     │   O(1)   │   O(1)   │   O(n)   │   O(n)   │
│ 头部插入     │   O(n)   │   O(1)   │   O(1)   │   O(1)   │
│ 尾部插入     │   O(1)   │   O(1)   │   O(1)   │   O(1)   │
│ 中间插入     │   O(n)   │   O(n)   │   O(1)   │   O(1)   │
│ 内存连续     │    是    │   分段   │    否    │    否    │
│ 迭代器失效   │  可能    │  可能    │   很少   │   很少   │
│ 额外开销     │   低     │   中     │   高     │  很低    │
└──────────────┴──────────┴──────────┴──────────┴──────────┘

选择建议：
- 默认使用 vector（性能好，缓存友好）
- 需要频繁头部操作用 deque
- 频繁中间插入/删除用 list
- 只需单向遍历且省内存用 forward_list
)" << endl;
}

// ==================== 主函数 ====================

int main() {
    demoVector();
    demoDeque();
    demoList();
    demoForwardList();
    demoContainerSelection();

    return 0;
}
