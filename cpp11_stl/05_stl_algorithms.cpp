/**
 * STL 算法库详解
 *
 * 本文件涵盖：
 * 1. 非修改序列算法（count, find, search 等）
 * 2. 修改序列算法（copy, transform, replace 等）
 * 3. 排序相关算法（sort, merge, binary_search 等）
 * 4. 数值算法（accumulate, inner_product 等）
 *
 * 编译：g++ -std=c++11 05_stl_algorithms.cpp -o stl_algorithms
 */

#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <string>
#include <iterator>
#include <random>

using namespace std;

// ==================== 1. 非修改序列算法 ====================

void demoNonModifyingAlgorithms() {
    cout << "=== 非修改序列算法 ===" << endl;

    vector<int> v = {1, 2, 3, 4, 5, 3, 2, 1};

    // count / count_if
    cout << "count(3): " << count(v.begin(), v.end(), 3) << endl;
    cout << "count_if(>3): "
         << count_if(v.begin(), v.end(), [](int x) { return x > 3; })
         << endl;

    // find / find_if
    auto it = find(v.begin(), v.end(), 4);
    if (it != v.end()) {
        cout << "found 4 at position: " << distance(v.begin(), it) << endl;
    }

    auto it2 = find_if(v.begin(), v.end(), [](int x) { return x > 4; });
    if (it2 != v.end()) {
        cout << "first > 4: " << *it2 << endl;
    }

    // all_of / any_of / none_of
    cout << "all_of > 0: "
         << all_of(v.begin(), v.end(), [](int x) { return x > 0; })
         << endl;
    cout << "any_of == 3: "
         << any_of(v.begin(), v.end(), [](int x) { return x == 3; })
         << endl;
    cout << "none_of < 0: "
         << none_of(v.begin(), v.end(), [](int x) { return x < 0; })
         << endl;

    // for_each
    cout << "for_each 输出: ";
    for_each(v.begin(), v.end(), [](int x) { cout << x << " "; });
    cout << endl;

    // search - 查找子序列
    vector<int> pattern = {3, 4, 5};
    auto searchIt = search(v.begin(), v.end(), pattern.begin(), pattern.end());
    if (searchIt != v.end()) {
        cout << "found pattern at position: "
             << distance(v.begin(), searchIt) << endl;
    }

    // adjacent_find - 查找相邻重复元素
    vector<int> v2 = {1, 2, 2, 3, 4};
    auto adjIt = adjacent_find(v2.begin(), v2.end());
    if (adjIt != v2.end()) {
        cout << "adjacent duplicate: " << *adjIt << endl;
    }

    // equal - 比较两个序列
    vector<int> v3 = {1, 2, 3, 4, 5};
    cout << "v == v3 (前5个): "
         << equal(v.begin(), v.begin() + 5, v3.begin())
         << endl;

    // mismatch - 找到第一个不匹配的位置
    auto pairIt = mismatch(v.begin(), v.begin() + 5, v3.begin());
    if (pairIt.first != v.begin() + 5) {
        cout << "mismatch at position "
             << distance(v.begin(), pairIt.first) << ": "
             << *pairIt.first << " vs " << *pairIt.second << endl;
    }
}

// ==================== 2. 修改序列算法 ====================

void demoModifyingAlgorithms() {
    cout << "\n=== 修改序列算法 ===" << endl;

    vector<int> v = {1, 2, 3, 4, 5};

    // copy
    vector<int> v1(5);
    copy(v.begin(), v.end(), v1.begin());
    cout << "copy: ";
    for (int x : v1) cout << x << " ";
    cout << endl;

    // copy_n
    vector<int> v2(3);
    copy_n(v.begin(), 3, v2.begin());
    cout << "copy_n(3): ";
    for (int x : v2) cout << x << " ";
    cout << endl;

    // copy_if
    vector<int> v3;
    copy_if(v.begin(), v.end(), back_inserter(v3), [](int x) {
        return x % 2 == 0;
    });
    cout << "copy_if (偶数): ";
    for (int x : v3) cout << x << " ";
    cout << endl;

    // transform
    vector<int> v4;
    transform(v.begin(), v.end(), back_inserter(v4), [](int x) {
        return x * x;
    });
    cout << "transform (平方): ";
    for (int x : v4) cout << x << " ";
    cout << endl;

    // 两个序列的 transform
    vector<int> a = {1, 2, 3};
    vector<int> b = {4, 5, 6};
    vector<int> c;
    transform(a.begin(), a.end(), b.begin(), back_inserter(c),
              [](int x, int y) { return x + y; });
    cout << "transform (相加): ";
    for (int x : c) cout << x << " ";
    cout << endl;

    // fill / fill_n
    vector<int> v5(5);
    fill(v5.begin(), v5.end(), 42);
    cout << "fill(42): ";
    for (int x : v5) cout << x << " ";
    cout << endl;

    // generate / generate_n
    vector<int> v6(5);
    int counter = 0;
    generate(v6.begin(), v6.end(), [&counter]() { return ++counter; });
    cout << "generate: ";
    for (int x : v6) cout << x << " ";
    cout << endl;

    // replace / replace_if
    vector<int> v7 = {1, 2, 3, 2, 1};
    replace(v7.begin(), v7.end(), 2, 99);
    cout << "replace(2->99): ";
    for (int x : v7) cout << x << " ";
    cout << endl;

    // remove / remove_if (不真正删除，而是移到末尾)
    vector<int> v8 = {1, 2, 3, 4, 5, 6};
    auto newEnd = remove_if(v8.begin(), v8.end(), [](int x) {
        return x % 2 == 0;
    });
    v8.erase(newEnd, v8.end());  // 真正删除
    cout << "remove_if (删除偶数): ";
    for (int x : v8) cout << x << " ";
    cout << endl;

    // unique (移除连续重复元素)
    vector<int> v9 = {1, 1, 2, 2, 2, 3, 3};
    auto newEnd2 = unique(v9.begin(), v9.end());
    v9.erase(newEnd2, v9.end());
    cout << "unique: ";
    for (int x : v9) cout << x << " ";
    cout << endl;

    // reverse
    vector<int> v10 = {1, 2, 3, 4, 5};
    reverse(v10.begin(), v10.end());
    cout << "reverse: ";
    for (int x : v10) cout << x << " ";
    cout << endl;

    // rotate
    vector<int> v11 = {1, 2, 3, 4, 5};
    rotate(v11.begin(), v11.begin() + 2, v11.end());
    cout << "rotate(2): ";
    for (int x : v11) cout << x << " ";
    cout << endl;

    // shuffle (C++11)
    vector<int> v12 = {1, 2, 3, 4, 5};
    random_device rd;
    mt19937 g(rd());
    shuffle(v12.begin(), v12.end(), g);
    cout << "shuffle: ";
    for (int x : v12) cout << x << " ";
    cout << endl;

    // swap / iter_swap
    vector<int> v13 = {1, 2, 3};
    vector<int> v14 = {4, 5, 6};
    swap(v13, v14);
    cout << "swap v13: ";
    for (int x : v13) cout << x << " ";
    cout << endl;
}

// ==================== 3. 排序相关算法 ====================

void demoSortingAlgorithms() {
    cout << "\n=== 排序相关算法 ===" << endl;

    vector<int> v = {5, 2, 8, 1, 9, 3};

    // sort
    sort(v.begin(), v.end());
    cout << "sort (升序): ";
    for (int x : v) cout << x << " ";
    cout << endl;

    // 降序排序
    sort(v.begin(), v.end(), greater<int>());
    cout << "sort (降序): ";
    for (int x : v) cout << x << " ";
    cout << endl;

    // 自定义比较
    struct Person {
        string name;
        int age;
    };

    vector<Person> people = {
        {"Alice", 25},
        {"Bob", 20},
        {"Charlie", 30}
    };

    sort(people.begin(), people.end(),
         [](const Person& a, const Person& b) {
             return a.age < b.age;
         });

    cout << "按年龄排序:" << endl;
    for (const auto& p : people) {
        cout << "  " << p.name << ": " << p.age << endl;
    }

    // stable_sort (保持相等元素的相对顺序)
    v = {3, 1, 4, 1, 5, 9, 2, 6};
    stable_sort(v.begin(), v.end());
    cout << "stable_sort: ";
    for (int x : v) cout << x << " ";
    cout << endl;

    // partial_sort (只排序前 N 个)
    v = {5, 2, 8, 1, 9, 3};
    partial_sort(v.begin(), v.begin() + 3, v.end());
    cout << "partial_sort(3): ";
    for (int x : v) cout << x << " ";
    cout << endl;

    // nth_element (找到第 N 小的元素)
    v = {5, 2, 8, 1, 9, 3};
    nth_element(v.begin(), v.begin() + 2, v.end());
    cout << "nth_element(2): ";
    for (int x : v) cout << x << " ";
    cout << " (第3小: " << v[2] << ")" << endl;

    // binary_search (需要先排序)
    v = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    bool found = binary_search(v.begin(), v.end(), 5);
    cout << "binary_search(5): " << found << endl;

    // lower_bound / upper_bound
    auto lower = lower_bound(v.begin(), v.end(), 5);
    auto upper = upper_bound(v.begin(), v.end(), 5);
    cout << "lower_bound(5): " << *lower << endl;
    cout << "upper_bound(5): " << *upper << endl;
    cout << "等于5的元素个数: " << distance(lower, upper) << endl;

    // equal_range
    auto range = equal_range(v.begin(), v.end(), 5);
    cout << "equal_range(5): [" << distance(v.begin(), range.first)
         << ", " << distance(v.begin(), range.second) << ")" << endl;

    // merge (合并两个有序序列)
    vector<int> a = {1, 3, 5};
    vector<int> b = {2, 4, 6};
    vector<int> result;
    merge(a.begin(), a.end(), b.begin(), b.end(), back_inserter(result));
    cout << "merge: ";
    for (int x : result) cout << x << " ";
    cout << endl;

    // set operations
    vector<int> set1 = {1, 2, 3, 4, 5};
    vector<int> set2 = {3, 4, 5, 6, 7};
    vector<int> out;

    set_difference(set1.begin(), set1.end(), set2.begin(), set2.end(),
                   back_inserter(out));
    cout << "set_difference (set1 - set2): ";
    for (int x : out) cout << x << " ";
    cout << endl;

    out.clear();
    set_intersection(set1.begin(), set1.end(), set2.begin(), set2.end(),
                     back_inserter(out));
    cout << "set_intersection: ";
    for (int x : out) cout << x << " ";
    cout << endl;

    out.clear();
    set_union(set1.begin(), set1.end(), set2.begin(), set2.end(),
              back_inserter(out));
    cout << "set_union: ";
    for (int x : out) cout << x << " ";
    cout << endl;

    // min_element / max_element
    v = {3, 1, 4, 1, 5, 9};
    auto minIt = min_element(v.begin(), v.end());
    auto maxIt = max_element(v.begin(), v.end());
    cout << "min: " << *minIt << ", max: " << *maxIt << endl;

    // minmax_element
    auto minmax = minmax_element(v.begin(), v.end());
    cout << "minmax: min=" << *minmax.first << ", max=" << *minmax.second << endl;

    // clamp (C++17)
    // int x = 5;
    // int clamped = clamp(x, 0, 10);
}

// ==================== 4. 数值算法 ====================

void demoNumericAlgorithms() {
    cout << "\n=== 数值算法 ===" << endl;

    vector<int> v = {1, 2, 3, 4, 5};

    // accumulate (求和)
    int sum = accumulate(v.begin(), v.end(), 0);
    cout << "sum: " << sum << endl;

    // 带自定义操作的 accumulate
    int product = accumulate(v.begin(), v.end(), 1, multiplies<int>());
    cout << "product: " << product << endl;

    // 字符串连接
    vector<string> words = {"Hello", " ", "World", "!"};
    string sentence = accumulate(words.begin(), words.end(), string(""));
    cout << "concatenated: " << sentence << endl;

    // inner_product (内积)
    vector<int> a = {1, 2, 3};
    vector<int> b = {4, 5, 6};
    int ip = inner_product(a.begin(), a.end(), b.begin(), 0);
    cout << "inner_product: " << ip << endl;  // 1*4 + 2*5 + 3*6 = 32

    // 自定义 inner_product
    int customIP = inner_product(a.begin(), a.end(), b.begin(), 0,
                                 plus<int>(),  // 累加
                                 multiplies<int>());  // 乘法
    cout << "custom inner_product: " << customIP << endl;

    // adjacent_difference
    vector<int> result;
    adjacent_difference(v.begin(), v.end(), back_inserter(result));
    cout << "adjacent_difference: ";
    for (int x : result) cout << x << " ";
    cout << endl;

    // partial_sum
    vector<int> ps;
    partial_sum(v.begin(), v.end(), back_inserter(ps));
    cout << "partial_sum: ";
    for (int x : ps) cout << x << " ";
    cout << endl;

    // iota (填充递增序列)
    vector<int> v2(10);
    iota(v2.begin(), v2.end(), 1);
    cout << "iota(1): ";
    for (int x : v2) cout << x << " ";
    cout << endl;

    // gcd / lcm (C++17)
    // cout << "gcd(48, 18): " << gcd(48, 18) << endl;
    // cout << "lcm(4, 6): " << lcm(4, 6) << endl;
}

// ==================== 主函数 ====================

int main() {
    demoNonModifyingAlgorithms();
    demoModifyingAlgorithms();
    demoSortingAlgorithms();
    demoNumericAlgorithms();

    cout << "\n=== STL 算法总结 ===" << endl;
    cout << R"(
常用算法分类：

1. 非修改序列算法：
   - count/count_if - 计数
   - find/find_if - 查找
   - all_of/any_of/none_of - 条件判断
   - for_each - 遍历操作
   - search - 查找子序列
   - equal - 比较序列
   - mismatch - 找不匹配位置

2. 修改序列算法：
   - copy/copy_if - 复制
   - transform - 转换
   - fill/generate - 填充
   - replace/replace_if - 替换
   - remove/remove_if - 删除
   - unique - 去重
   - reverse - 反转
   - rotate - 旋转
   - shuffle - 随机打乱

3. 排序算法：
   - sort - 排序
   - stable_sort - 稳定排序
   - partial_sort - 部分排序
   - nth_element - 第 N 小元素
   - binary_search - 二分查找
   - lower_bound/upper_bound - 边界查找
   - merge - 合并有序序列
   - set_difference/intersection/union - 集合操作

4. 数值算法：
   - accumulate - 累积
   - inner_product - 内积
   - adjacent_difference - 相邻差
   - partial_sum - 部分和
   - iota - 填充递增序列

特点：
- 独立于容器，通过迭代器操作
- 高效、类型安全
- 支持自定义操作（lambda、函数对象）
)" << endl;

    return 0;
}
