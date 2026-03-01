/**
 * STL 容器适配器详解
 *
 * 本文件涵盖：
 * 1. std::stack - 栈（LIFO）
 * 2. std::queue - 队列（FIFO）
 * 3. std::priority_queue - 优先队列
 *
 * 编译：g++ -std=c++11 04_container_adaptors.cpp -o container_adaptors
 */

#include <iostream>
#include <stack>
#include <queue>
#include <vector>
#include <string>
#include <list>

using namespace std;

// ==================== 1. std::stack 栈 ====================

/*
 * stack 特点：
 * - 后进先出（LIFO）
 * - 只能访问栈顶元素
 * - 默认基于 deque 实现
 * - 常用方法：
 *   push() / emplace() - 入栈
 *   pop() - 出栈（不返回值）
 *   top() - 访问栈顶
 *   empty() / size()
 */

void demoStack() {
    cout << "=== std::stack 示例 ===" << endl;

    // 1. 基本操作
    stack<int> s;

    s.push(1);
    s.push(2);
    s.push(3);

    cout << "栈大小: " << s.size() << endl;
    cout << "栈顶: " << s.top() << endl;

    s.pop();  // 出栈，不返回值
    cout << "pop 后栈顶: " << s.top() << endl;

    // 2. 使用不同底层容器
    stack<int, vector<int>> vectorStack;
    stack<int, list<int>> listStack;

    // 3. 实际应用：括号匹配
    cout << "\n=== 括号匹配检测 ===" << endl;

    auto isBalanced = [](const string& expr) -> bool {
        stack<char> s;
        for (char c : expr) {
            if (c == '(' || c == '[' || c == '{') {
                s.push(c);
            } else if (c == ')' || c == ']' || c == '}') {
                if (s.empty()) return false;
                char top = s.top();
                s.pop();
                if ((c == ')' && top != '(') ||
                    (c == ']' && top != '[') ||
                    (c == '}' && top != '{')) {
                    return false;
                }
            }
        }
        return s.empty();
    };

    vector<string> tests = {
        "(a + b) * (c - d)",
        "((()))",
        "([{}])",
        "([)]",
        "((()"
    };

    for (const auto& test : tests) {
        cout << "'" << test << "' is "
             << (isBalanced(test) ? "balanced" : "not balanced")
             << endl;
    }

    // 4. 实际应用：函数调用栈模拟
    cout << "\n=== 函数调用栈模拟 ===" << endl;

    struct CallFrame {
        string function;
        int line;

        CallFrame(const string& f, int l) : function(f), line(l) {}
    };

    stack<CallFrame> callStack;

    callStack.emplace("main", 1);
    callStack.emplace("foo", 10);
    callStack.emplace("bar", 20);

    cout << "调用栈（从顶到底）：" << endl;
    while (!callStack.empty()) {
        const auto& frame = callStack.top();
        cout << "  " << frame.function << ":" << frame.line << endl;
        callStack.pop();
    }

    // 5. 实际应用：表达式求值（后缀表达式）
    cout << "\n=== 后缀表达式求值 ===" << endl;

    auto evalPostfix = [](const string& expr) -> int {
        stack<int> s;
        string num;
        for (char c : expr) {
            if (isdigit(c)) {
                num += c;
            } else if (c == ' ') {
                if (!num.empty()) {
                    s.push(stoi(num));
                    num.clear();
                }
            } else {  // 运算符
                int b = s.top(); s.pop();
                int a = s.top(); s.pop();
                switch (c) {
                    case '+': s.push(a + b); break;
                    case '-': s.push(a - b); break;
                    case '*': s.push(a * b); break;
                    case '/': s.push(a / b); break;
                }
            }
        }
        if (!num.empty()) {
            s.push(stoi(num));
        }
        return s.top();
    };

    cout << "2 3 + 4 * = " << evalPostfix("2 3 + 4 *") << endl;  // 20
    cout << "5 1 2 + 4 * + 3 - = "
         << evalPostfix("5 1 2 + 4 * + 3 -") << endl;  // 14
}

// ==================== 2. std::queue 队列 ====================

/*
 * queue 特点：
 * - 先进先出（FIFO）
 * - 只能访问队头和队尾
 * - 默认基于 deque 实现
 * - 常用方法：
 *   push() / emplace() - 入队
 *   pop() - 出队（不返回值）
 *   front() - 访问队头
 *   back() - 访问队尾
 *   empty() / size()
 */

void demoQueue() {
    cout << "\n=== std::queue 示例 ===" << endl;

    // 1. 基本操作
    queue<int> q;

    q.push(1);
    q.push(2);
    q.push(3);

    cout << "队头: " << q.front() << endl;
    cout << "队尾: " << q.back() << endl;

    q.pop();
    cout << "pop 后队头: " << q.front() << endl;

    // 2. 实际应用：任务调度
    cout << "\n=== 任务调度器 ===" << endl;

    struct Task {
        string name;
        int priority;

        Task(const string& n, int p) : name(n), priority(p) {}
    };

    queue<Task> taskQueue;

    taskQueue.emplace("Task 1", 1);
    taskQueue.emplace("Task 2", 2);
    taskQueue.emplace("Task 3", 1);

    while (!taskQueue.empty()) {
        const auto& task = taskQueue.front();
        cout << "执行: " << task.name
             << " (优先级: " << task.priority << ")" << endl;
        taskQueue.pop();
    }

    // 3. 实际应用：BFS 遍历
    cout << "\n=== BFS 树遍历 ===" << endl;

    struct TreeNode {
        int val;
        TreeNode* left;
        TreeNode* right;
        TreeNode(int v) : val(v), left(nullptr), right(nullptr) {}
    };

    // 构建树
    /*
          1
        /   \
       2     3
      / \   / \
     4   5 6   7
    */
    TreeNode* root = new TreeNode(1);
    root->left = new TreeNode(2);
    root->right = new TreeNode(3);
    root->left->left = new TreeNode(4);
    root->left->right = new TreeNode(5);
    root->right->left = new TreeNode(6);
    root->right->right = new TreeNode(7);

    queue<TreeNode*> bfsQueue;
    bfsQueue.push(root);

    cout << "BFS 遍历结果: ";
    while (!bfsQueue.empty()) {
        TreeNode* node = bfsQueue.front();
        bfsQueue.pop();
        cout << node->val << " ";

        if (node->left) bfsQueue.push(node->left);
        if (node->right) bfsQueue.push(node->right);
    }
    cout << endl;

    // 清理
    delete root->left->left;
    delete root->left->right;
    delete root->right->left;
    delete root->right->right;
    delete root->left;
    delete root->right;
    delete root;
}

// ==================== 3. std::priority_queue 优先队列 ====================

/*
 * priority_queue 特点：
 * - 优先级最高的元素先出队
 * - 默认是大顶堆（max heap）
 * - 常用方法：
 *   push() / emplace() - 插入
 *   pop() - 弹出优先级最高的
 *   top() - 访问最高优先级元素
 *   empty() / size()
 */

void demoPriorityQueue() {
    cout << "\n=== std::priority_queue 示例 ===" << endl;

    // 1. 大顶堆（默认）
    priority_queue<int> maxHeap;
    maxHeap.push(3);
    maxHeap.push(1);
    maxHeap.push(4);
    maxHeap.push(1);
    maxHeap.push(5);

    cout << "大顶堆（从大到小弹出）: ";
    while (!maxHeap.empty()) {
        cout << maxHeap.top() << " ";
        maxHeap.pop();
    }
    cout << endl;

    // 2. 小顶堆
    priority_queue<int, vector<int>, greater<int>> minHeap;
    minHeap.push(3);
    minHeap.push(1);
    minHeap.push(4);
    minHeap.push(1);
    minHeap.push(5);

    cout << "小顶堆（从小到大弹出）: ";
    while (!minHeap.empty()) {
        cout << minHeap.top() << " ";
        minHeap.pop();
    }
    cout << endl;

    // 3. 自定义比较（按绝对值）
    auto absCmp = [](int a, int b) { return abs(a) < abs(b); };
    priority_queue<int, vector<int>, decltype(absCmp)> absHeap(absCmp);

    absHeap.push(-5);
    absHeap.push(3);
    absHeap.push(-1);
    absHeap.push(4);
    absHeap.push(-2);

    cout << "按绝对值大顶堆: ";
    while (!absHeap.empty()) {
        cout << absHeap.top() << " ";
        absHeap.pop();
    }
    cout << endl;

    // 4. 实际应用：Top K 问题
    cout << "\n=== 找出第 K 大的元素 ===" << endl;

    vector<int> nums = {3, 2, 1, 5, 6, 4};
    int k = 2;

    // 方法1：使用大顶堆
    priority_queue<int> pq1(nums.begin(), nums.end());
    for (int i = 0; i < k - 1; ++i) pq1.pop();
    cout << "第 " << k << " 大的元素: " << pq1.top() << endl;

    // 方法2：使用小顶堆（更适合大数据流）
    priority_queue<int, vector<int>, greater<int>> pq2;
    for (int num : nums) {
        pq2.push(num);
        if (pq2.size() > k) pq2.pop();
    }
    cout << "第 " << k << " 大的元素: " << pq2.top() << endl;

    // 5. 实际应用：合并 K 个有序链表
    cout << "\n=== 合并 K 个有序数组 ===" << endl;

    vector<vector<int>> arrays = {
        {1, 4, 5},
        {1, 3, 4},
        {2, 6}
    };

    struct Element {
        int val;
        int arrayIndex;
        int elementIndex;

        bool operator<(const Element& other) const {
            return val > other.val;  // 小顶堆
        }
    };

    priority_queue<Element> mergePQ;

    // 初始化：将每个数组的第一个元素入队
    for (int i = 0; i < arrays.size(); ++i) {
        if (!arrays[i].empty()) {
            mergePQ.push({arrays[i][0], i, 0});
        }
    }

    vector<int> merged;
    while (!mergePQ.empty()) {
        Element elem = mergePQ.top();
        mergePQ.pop();
        merged.push_back(elem.val);

        // 将同一数组的下一个元素入队
        if (elem.elementIndex + 1 < arrays[elem.arrayIndex].size()) {
            mergePQ.push({
                arrays[elem.arrayIndex][elem.elementIndex + 1],
                elem.arrayIndex,
                elem.elementIndex + 1
            });
        }
    }

    cout << "合并结果: ";
    for (int x : merged) cout << x << " ";
    cout << endl;

    // 6. 实际应用：任务优先级调度
    cout << "\n=== 任务优先级调度 ===" << endl;

    struct PriorityTask {
        string name;
        int priority;  // 数字越小优先级越高

        bool operator<(const PriorityTask& other) const {
            return priority > other.priority;  // 小顶堆
        }
    };

    priority_queue<PriorityTask> taskPQ;

    taskPQ.push({"Low priority task", 3});
    taskPQ.push({"Critical task", 1});
    taskPQ.push({"Medium task", 2});
    taskPQ.push({"Another critical", 1});

    while (!taskPQ.empty()) {
        const auto& task = taskPQ.top();
        cout << "执行: " << task.name
             << " (优先级: " << task.priority << ")" << endl;
        taskPQ.pop();
    }
}

// ==================== 主函数 ====================

int main() {
    demoStack();
    demoQueue();
    demoPriorityQueue();

    cout << "\n=== 容器适配器总结 ===" << endl;
    cout << R"(
┌────────────────┬──────────┬──────────┬──────────────┐
│     容器       │  接口    │ 数据结构 │   典型应用   │
├────────────────┼──────────┼──────────┼──────────────┤
│ stack          │  LIFO    │   deque  │ 括号匹配、   │
│                │          │(默认)    │ 表达式求值   │
├────────────────┼──────────┼──────────┼──────────────┤
│ queue          │  FIFO    │   deque  │ 任务调度、   │
│                │          │(默认)    │ BFS 遍历     │
├────────────────┼──────────┼──────────┼──────────────┤
│ priority_queue │ 优先级   │  vector  │ Top K、      │
│                │          │(默认堆)  │ 任务调度     │
└────────────────┴──────────┴──────────┴──────────────┘

特点：
- 都是适配器，基于底层容器实现
- 不支持迭代器（设计上不提供遍历功能）
- 提供受限的接口（只能访问特定位置的元素）

选择建议：
- 需要后进先出：stack
- 需要先进先出：queue
- 需要按优先级处理：priority_queue
)" << endl;

    return 0;
}
