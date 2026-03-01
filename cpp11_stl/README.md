# C++ STL 容器与算法学习指南

本目录包含一系列完整的 C++11 STL 容器和算法学习示例，涵盖基础知识到实际应用。

## 📁 文件结构

### 基础容器篇

| 文件 | 描述 | 主要内容 |
|------|------|----------|
| [`01_sequence_containers.cpp`](01_sequence_containers.cpp) | 序列容器详解 | `vector`, `deque`, `list`, `forward_list` 的使用方法和性能对比 |
| [`02_associative_containers.cpp`](02_associative_containers.cpp) | 关联容器详解 | `set`, `map`, `multiset`, `multimap` 的有序存储和查找 |
| [`03_unordered_containers.cpp`](03_unordered_containers.cpp) | 无序容器详解 | `unordered_set`, `unordered_map` 哈希表实现和自定义哈希 |
| [`04_container_adaptors.cpp`](04_container_adaptors.cpp) | 容器适配器详解 | `stack`, `queue`, `priority_queue` 及其实际应用场景 |

### 算法与工具篇

| 文件 | 描述 | 主要内容 |
|------|------|----------|
| [`05_stl_algorithms.cpp`](05_stl_algorithms.cpp) | STL 算法库 | 非修改/修改序列算法、排序算法、数值算法 |
| [`06_string_operations.cpp`](06_string_operations.cpp) | 字符串操作 | `std::string` 常用操作、查找替换、格式化、解析 |

### 综合应用篇

| 文件 | 描述 | 解决问题 |
|------|------|----------|
| [`07_practical_data_processing.cpp`](07_practical_data_processing.cpp) | 数据处理工具 | CSV 读取、数据统计、分组聚合、筛选排序 |
| [`08_practical_text_analysis.cpp`](08_practical_text_analysis.cpp) | 文本分析工具 | 词频统计、文本搜索、相似度计算、拼写建议 |
| [`09_practical_cache_implementation.cpp`](09_practical_cache_implementation.cpp) | 缓存系统实现 | LRU/LFU 缓存、TTL 缓存、多级缓存 |
| [`10_practical_todo_app.cpp`](10_practical_todo_app.cpp) | 待办事项应用 | 命令行 TODO 管理器，综合运用多种容器 |

## 🚀 快速开始

### 编译运行

```bash
# 编译单个文件
g++ -std=c++11 01_sequence_containers.cpp -o seq_containers
./seq_containers

# 编译并运行综合应用
g++ -std=c++11 10_practical_todo_app.cpp -o todo_app
./todo_app --demo    # 演示模式
./todo_app           # 交互模式
```

### 待办应用命令

```
> add 完成项目报告          # 添加任务
> update 1 done             # 标记完成
> tag 1 work                # 添加标签
> list                      # 列出所有任务
> search work               # 按标签搜索
> stats                     # 显示统计
> show 1                    # 显示详情
```

## 📊 容器选择指南

### 序列容器对比

| 容器 | 随机访问 | 头部插入 | 尾部插入 | 中间插入 | 适用场景 |
|------|---------|---------|---------|---------|----------|
| `vector` | O(1) | O(n) | O(1) | O(n) | 默认选择，缓存友好 |
| `deque` | O(1) | O(1) | O(1) | O(n) | 需要频繁头部操作 |
| `list` | O(n) | O(1) | O(1) | O(1) | 频繁中间插入/删除 |
| `forward_list` | O(n) | O(1) | O(1) | O(1) | 单向遍历，省内存 |

### 关联容器对比

| 容器 | 排序 | 唯一键 | 允许重复 | 平均复杂度 | 适用场景 |
|------|------|--------|----------|-----------|----------|
| `set` | ✓ | ✓ | ✗ | O(log n) | 有序唯一集合 |
| `map` | ✓ | ✓ | ✗ | O(log n) | 有序键值对 |
| `unordered_set` | ✗ | ✓ | ✗ | O(1) | 无序唯一集合，快速查找 |
| `unordered_map` | ✗ | ✓ | ✗ | O(1) | 无序键值对，快速查找 |

## 🎯 学习路径

1. **第一步**：从 `01_sequence_containers.cpp` 开始，掌握最常用的容器
2. **第二步**：学习 `02_associative_containers.cpp` 和 `03_unordered_containers.cpp`
3. **第三步**：了解 `04_container_adaptors.cpp` 的特殊用途
4. **第四步**：掌握 `05_stl_algorithms.cpp` 中的常用算法
5. **第五步**：通过 `07-10` 的综合应用巩固知识

## 💡 关键概念

### 迭代器

```cpp
vector<int> v = {1, 2, 3, 4, 5};

// 正向迭代器
for (auto it = v.begin(); it != v.end(); ++it) {
    cout << *it << " ";
}

// 反向迭代器
for (auto it = v.rbegin(); it != v.rend(); ++it) {
    cout << *it << " ";
}

// 范围 for 循环 (C++11)
for (const auto& x : v) {
    cout << x << " ";
}
```

### Lambda 表达式

```cpp
// find_if 使用 lambda
auto it = find_if(v.begin(), v.end(), [](int x) {
    return x > 3;
});

// sort 使用 lambda
sort(v.begin(), v.end(), [](int a, int b) {
    return a > b;  // 降序
});
```

### 算法与函数对象

```cpp
// accumulate 带自定义操作
int sum = accumulate(v.begin(), v.end(), 0);           // 求和
int product = accumulate(v.begin(), v.end(), 1, multiplies<int>());  // 求积

// transform 使用 lambda
vector<int> result;
transform(v.begin(), v.end(), back_inserter(result), [](int x) {
    return x * x;
});
```

## 🔧 实用技巧

### 去重

```cpp
// 方法1：使用 set
unordered_set<int> s(v.begin(), v.end());
vector<int> unique(s.begin(), s.end());

// 方法2：先排序再用 unique
sort(v.begin(), v.end());
v.erase(unique(v.begin(), v.end()), v.end());
```

### 删除元素

```cpp
// 删除所有偶数
v.erase(remove_if(v.begin(), v.end(), [](int x) {
    return x % 2 == 0;
}), v.end());
```

### 自定义比较

```cpp
// map 降序
map<int, string, greater<int>> m;

// set 自定义结构
struct Person {
    string name;
    int age;
    bool operator<(const Person& other) const {
        return age < other.age;  // 按年龄排序
    }
};
set<Person> people;
```

## 📚 参考资料

- [cppreference](https://en.cppreference.com/w/) - C++ 权威参考
- [cplusplus.com](https://www.cplusplus.com/) - C++ 资源
- C++ Primer - 经典教材

## 📝 注意事项

1. **包含头文件**：使用容器时记得包含对应的头文件
2. **命名空间**：示例中使用 `using namespace std;`，实际项目建议用 `std::`
3. **C++ 标准**：所有代码使用 C++11 标准，部分特性需要更高版本
4. **编译警告**：建议加上 `-Wall -Wextra` 编译选项

---

**Happy Coding! 🎉**
