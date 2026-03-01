/**
 * 综合应用：缓存系统实现
 *
 * 本文件展示如何使用 STL 容器实现常见缓存策略：
 * 1. LRU (Least Recently Used) 缓存
 * 2. LFU (Least Frequently Used) 缓存
 * 3. 带过期的缓存
 * 4. 多级缓存
 *
 * 编译：g++ -std=c++11 09_practical_cache_implementation.cpp -o cache_impl
 */

#include <iostream>
#include <list>
#include <unordered_map>
#include <map>
#include <chrono>
#include <thread>
#include <string>
#include <algorithm>
#include <functional>
#include <iomanip>

using namespace std;
using namespace std::chrono;

// ==================== 1. LRU (Least Recently Used) 缓存 ====================

/*
 * LRU 原理：
 * - 最近使用的项放在前面
 * - 容量满时，淘汰最久未使用的项
 * - 使用 list 维护访问顺序 + unordered_map 快速查找
 */

template<typename Key, typename Value>
class LRUCache {
private:
    size_t capacity;
    list<pair<Key, Value>> itemList;  // 维护访问顺序
    unordered_map<Key, typename list<pair<Key, Value>>::iterator> itemMap;  // Key -> 迭代器

public:
    LRUCache(size_t cap) : capacity(cap) {}

    // 获取值，不存在返回默认值
    Value get(const Key& key, const Value& defaultValue = Value()) {
        auto it = itemMap.find(key);
        if (it == itemMap.end()) {
            return defaultValue;  // 未找到
        }

        // 找到了，移到链表头部（表示最近使用）
        itemList.splice(itemList.begin(), itemList, it->second);
        return it->second->second;
    }

    // 设置值
    void put(const Key& key, const Value& value) {
        auto it = itemMap.find(key);
        if (it != itemMap.end()) {
            // 键已存在，更新值并移到头部
            it->second->second = value;
            itemList.splice(itemList.begin(), itemList, it->second);
            return;
        }

        // 检查容量
        if (itemList.size() >= capacity) {
            // 淘汰最久未使用的（链表尾部）
            auto last = itemList.back();
            itemMap.erase(last.first);
            itemList.pop_back();
        }

        // 添加新项到头部
        itemList.emplace_front(key, value);
        itemMap[key] = itemList.begin();
    }

    // 检查键是否存在
    bool contains(const Key& key) const {
        return itemMap.find(key) != itemMap.end();
    }

    // 删除键
    void remove(const Key& key) {
        auto it = itemMap.find(key);
        if (it != itemMap.end()) {
            itemList.erase(it->second);
            itemMap.erase(it);
        }
    }

    // 清空缓存
    void clear() {
        itemList.clear();
        itemMap.clear();
    }

    // 获取当前大小
    size_t size() const {
        return itemList.size();
    }

    // 打印缓存内容
    void print() const {
        cout << "LRU Cache (容量: " << capacity << ", 大小: " << size() << ")" << endl;
        cout << "  [头部 -> 尾部]:" << endl;
        for (const auto& item : itemList) {
            cout << "    " << item.first << " => " << item.second << endl;
        }
    }
};

// ==================== 2. LFU (Least Frequently Used) 缓存 ====================

/*
 * LFU 原理：
 * - 跟踪每个项的访问频率
 * - 容量满时，淘汰访问频率最低的项
 * - 频率相同时，淘汰最早的
 */

template<typename Key, typename Value>
class LFUCache {
private:
    struct CacheItem {
        Value value;
        size_t frequency;
        time_point<steady_clock> lastAccess;

        bool operator<(const CacheItem& other) const {
            if (frequency != other.frequency) {
                return frequency < other.frequency;  // 频率低的优先
            }
            return lastAccess < other.lastAccess;  // 频率相同时，时间早的优先
        }
    };

    size_t capacity;
    unordered_map<Key, CacheItem> cache;

public:
    LFUCache(size_t cap) : capacity(cap) {}

    Value get(const Key& key, const Value& defaultValue = Value()) {
        auto it = cache.find(key);
        if (it == cache.end()) {
            return defaultValue;
        }

        // 更新频率和访问时间
        it->second.frequency++;
        it->second.lastAccess = steady_clock::now();
        return it->second.value;
    }

    void put(const Key& key, const Value& value) {
        auto it = cache.find(key);
        if (it != cache.end()) {
            it->second.value = value;
            it->second.frequency++;
            it->second.lastAccess = steady_clock::now();
            return;
        }

        if (cache.size() >= capacity) {
            // 找到频率最低的项
            auto minIt = min_element(cache.begin(), cache.end(),
                [](const pair<Key, CacheItem>& a, const pair<Key, CacheItem>& b) {
                    if (a.second.frequency != b.second.frequency) {
                        return a.second.frequency < b.second.frequency;
                    }
                    return a.second.lastAccess < b.second.lastAccess;
                });
            cache.erase(minIt);
        }

        cache[key] = {value, 1, steady_clock::now()};
    }

    bool contains(const Key& key) const {
        return cache.find(key) != cache.end();
    }

    size_t size() const {
        return cache.size();
    }

    void print() const {
        cout << "LFU Cache (容量: " << capacity << ", 大小: " << size() << ")" << endl;
        multimap<size_t, pair<Key, Value>> sorted;
        for (const auto& p : cache) {
            sorted.insert({p.second.frequency, {p.first, p.second.value}});
        }

        cout << "  [频率低 -> 高]:" << endl;
        for (const auto& p : sorted) {
            cout << "    " << p.second.first << " => " << p.second.second
                 << " (频率: " << p.first << ")" << endl;
        }
    }
};

// ==================== 3. 带过期的缓存 ====================

template<typename Key, typename Value>
class TTLCache {
private:
    struct CacheItem {
        Value value;
        time_point<steady_clock> expireTime;

        bool isExpired() const {
            return steady_clock::now() > expireTime;
        }
    };

    unordered_map<Key, CacheItem> cache;
    milliseconds defaultTTL;

    void cleanExpired() {
        for (auto it = cache.begin(); it != cache.end();) {
            if (it->second.isExpired()) {
                it = cache.erase(it);
            } else {
                ++it;
            }
        }
    }

public:
    TTLCache(milliseconds ttl = milliseconds(5000))
        : defaultTTL(ttl) {}

    void put(const Key& key, const Value& value, milliseconds ttl = milliseconds(0)) {
        if (ttl == milliseconds(0)) {
            ttl = defaultTTL;
        }
        cache[key] = {value, steady_clock::now() + ttl};
    }

    Value get(const Key& key, const Value& defaultValue = Value()) {
        cleanExpired();  // 清理过期项

        auto it = cache.find(key);
        if (it == cache.end() || it->second.isExpired()) {
            return defaultValue;
        }
        return it->second.value;
    }

    bool contains(const Key& key) {
        cleanExpired();
        auto it = cache.find(key);
        return it != cache.end() && !it->second.isExpired();
    }

    void remove(const Key& key) {
        cache.erase(key);
    }

    void clear() {
        cache.clear();
    }

    size_t size() const {
        return cache.size();
    }

    void print() const {
        cout << "TTL Cache (大小: " << size() << ")" << endl;
        auto now = steady_clock::now();
        for (const auto& p : cache) {
            auto remaining = duration_cast<milliseconds>(
                p.second.expireTime - now).count();
            cout << "    " << p.first << " => " << p.second.value
                 << " (剩余: " << remaining << "ms)" << endl;
        }
    }
};

// ==================== 4. 多级缓存 ====================

template<typename Key, typename Value>
class MultiLevelCache {
private:
    LRUCache<Key, Value> l1Cache;  // 一级缓存（小而快）
    LRUCache<Key, Value> l2Cache;  // 二级缓存（大而慢）
    function<Value(const Key&)> loader;  // 数据加载函数

    size_t l1Hits = 0;
    size_t l2Hits = 0;
    size_t misses = 0;

public:
    MultiLevelCache(size_t l1Size, size_t l2Size,
                   function<Value(const Key&)> loadFunc)
        : l1Cache(l1Size), l2Cache(l2Size), loader(loadFunc) {}

    Value get(const Key& key) {
        // 先查 L1
        if (l1Cache.contains(key)) {
            l1Hits++;
            return l1Cache.get(key);
        }

        // 再查 L2
        if (l2Cache.contains(key)) {
            l2Hits++;
            Value value = l2Cache.get(key);
            l1Cache.put(key, value);  // 提升到 L1
            return value;
        }

        // 都没有，加载
        misses++;
        Value value = loader(key);
        l2Cache.put(key, value);
        l1Cache.put(key, value);
        return value;
    }

    void put(const Key& key, const Value& value) {
        l1Cache.put(key, value);
        l2Cache.put(key, value);
    }

    void printStats() const {
        size_t total = l1Hits + l2Hits + misses;
        if (total == 0) {
            cout << "没有访问记录" << endl;
            return;
        }

        cout << "\n=== 缓存统计 ===" << endl;
        cout << "总访问: " << total << endl;
        cout << "L1 命中: " << l1Hits << " ("
             << fixed << setprecision(1) << (100.0 * l1Hits / total) << "%)" << endl;
        cout << "L2 命中: " << l2Hits << " ("
             << (100.0 * l2Hits / total) << "%)" << endl;
        cout << "未命中: " << misses << " ("
             << (100.0 * misses / total) << "%)" << endl;
        cout << "命中率: "
             << (100.0 * (l1Hits + l2Hits) / total) << "%" << endl;
    }
};

// ==================== 主函数和测试 ====================

// 模拟数据库查询
string databaseQuery(int key) {
    static unordered_map<int, string> data = {
        {1, "Apple"}, {2, "Banana"}, {3, "Cherry"},
        {4, "Date"}, {5, "Elderberry"}, {6, "Fig"},
        {7, "Grape"}, {8, "Honeydew"}
    };

    auto it = data.find(key);
    if (it != data.end()) {
        cout << "  [数据库查询] " << endl;
        return it->second;
    }
    return "Unknown";
}

void demoLRU() {
    cout << "\n### LRU 缓存测试 ###" << endl;

    LRUCache<int, string> cache(3);

    cache.put(1, "One");
    cache.put(2, "Two");
    cache.put(3, "Three");
    cache.print();

    cout << "\n访问 key=1:" << endl;
    cache.get(1);
    cache.print();

    cout << "\n添加 key=4 (会淘汰 key=2):" << endl;
    cache.put(4, "Four");
    cache.print();

    cout << "\nkey=2 是否存在: " << (cache.contains(2) ? "是" : "否") << endl;
}

void demoLFU() {
    cout << "\n### LFU 缓存测试 ###" << endl;

    LFUCache<int, string> cache(3);

    cache.put(1, "One");
    cache.put(2, "Two");
    cache.put(3, "Three");

    cout << "\n多次访问 key=1 和 key=2:" << endl;
    for (int i = 0; i < 5; ++i) {
        cache.get(1);
    }
    for (int i = 0; i < 3; ++i) {
        cache.get(2);
    }
    cache.get(3);  // 只访问一次

    cache.print();

    cout << "\n添加 key=4 (会淘汰 key=3):" << endl;
    cache.put(4, "Four");
    cache.print();
}

void demoTTL() {
    cout << "\n### TTL 缓存测试 ###" << endl;

    TTLCache<string, int> cache(milliseconds(1000));  // 1秒过期

    cache.put("a", 1);
    cache.put("b", 2);
    cache.print();

    cout << "\n500ms 后获取:" << endl;
    this_thread::sleep_for(milliseconds(500));
    cout << "cache.get('a'): " << cache.get("a", -1) << endl;

    cout << "\n再等 600ms (已超过 1 秒):" << endl;
    this_thread::sleep_for(milliseconds(600));
    cout << "cache.get('a'): " << cache.get("a", -1) << " (应该返回 -1)" << endl;
    cout << "cache.get('b'): " << cache.get("b", -1) << " (应该返回 -1)" << endl;
}

void demoMultiLevel() {
    cout << "\n### 多级缓存测试 ###" << endl;

    MultiLevelCache<int, string> mlCache(2, 4, databaseQuery);

    cout << "\n第一次访问 (全部未命中):" << endl;
    cout << "key=1: " << mlCache.get(1) << endl;
    cout << "key=2: " << mlCache.get(2) << endl;
    cout << "key=3: " << mlCache.get(3) << endl;

    cout << "\n再次访问 (应该命中 L1 或 L2):" << endl;
    cout << "key=1: " << mlCache.get(1) << endl;
    cout << "key=2: " << mlCache.get(2) << endl;
    cout << "key=1: " << mlCache.get(1) << endl;
    cout << "key=4: " << mlCache.get(4) << endl;

    mlCache.printStats();
}

// ==================== 实际应用：斐波那契缓存 ====================

int fibonacci(int n) {
    if (n <= 1) return n;
    return fibonacci(n - 1) + fibonacci(n - 2);
}

int fibonacciWithCache(int n, LRUCache<int, int>& cache) {
    if (n <= 1) return n;

    // 检查缓存
    int cached = cache.get(n, -1);
    if (cached != -1) {
        return cached;
    }

    // 计算并缓存
    int result = fibonacciWithCache(n - 1, cache) + fibonacciWithCache(n - 2, cache);
    cache.put(n, result);
    return result;
}

void demoFibonacciCache() {
    cout << "\n### 斐波那契缓存优化 ###" << endl;

    LRUCache<int, int> cache(100);

    cout << "计算 fib(40) 使用缓存:" << endl;
    auto start = steady_clock::now();
    int result = fibonacciWithCache(40, cache);
    auto end = steady_clock::now();

    cout << "fib(40) = " << result << endl;
    cout << "耗时: "
         << duration_cast<microseconds>(end - start).count()
         << " 微秒" << endl;

    cout << "\n不使用缓存计算 fib(40):" << endl;
    start = steady_clock::now();
    result = fibonacci(40);
    end = steady_clock::now();

    cout << "fib(40) = " << result << endl;
    cout << "耗时: "
         << duration_cast<milliseconds>(end - start).count()
         << " 毫秒" << endl;
}

// ==================== 主函数 ====================

int main() {
    cout << "=== STL 缓存实现示例 ===" << endl;

    demoLRU();
    demoLFU();
    demoTTL();
    demoMultiLevel();
    demoFibonacciCache();

    cout << "\n=== 缓存策略总结 ===" << endl;
    cout << R"(
1. LRU (Least Recently Used):
   - 淘汰最久未使用的项
   - 适合访问模式局部性强的场景
   - 实现：list + unordered_map

2. LFU (Least Frequently Used):
   - 淘汰访问频率最低的项
   - 适合有明显热点数据的场景
   - 实现：unordered_map + 频率计数

3. TTL (Time To Live):
   - 基于时间的淘汰策略
   - 适合需要定期刷新的数据
   - 实现：记录过期时间，定期清理

4. 多级缓存:
   - L1: 小而快（内存）
   - L2: 大而慢（磁盘/网络）
   - 适合读多写少的场景

选择建议：
- 一般场景：LRU
- 热点数据明显：LFU
- 需要定时刷新：TTL
- 大规模数据：多级缓存
)" << endl;

    return 0;
}
