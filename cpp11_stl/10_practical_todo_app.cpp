/**
 * 综合应用：命令行待办事项管理器
 *
 * 本文件综合运用多种 STL 容器实现一个完整的待办事项应用：
 * - priority_queue: 管理任务优先级
 * - map: 存储任务详情
 * - vector: 维护任务列表
 * - unordered_map: 快速查找
 * - set: 管理标签
 * - string: 文本处理
 * - algorithm: 排序和筛选
 *
 * 编译：g++ -std=c++11 10_practical_todo_app.cpp -o todo_app
 */

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <set>
#include <queue>
#include <algorithm>
#include <iomanip>
#include <ctime>
#include <sstream>
#include <fstream>

using namespace std;

// ==================== 数据结构 ====================

enum class Priority {
    LOW = 1,
    MEDIUM = 2,
    HIGH = 3,
    URGENT = 4
};

enum class Status {
    TODO,
    IN_PROGRESS,
    DONE
};

struct Task {
    int id;
    string title;
    string description;
    Priority priority;
    Status status;
    time_t dueDate;
    set<string> tags;

    string getPriorityString() const {
        switch (priority) {
            case Priority::LOW: return "Low";
            case Priority::MEDIUM: return "Medium";
            case Priority::HIGH: return "High";
            case Priority::URGENT: return "Urgent";
        }
        return "Unknown";
    }

    string getStatusString() const {
        switch (status) {
            case Status::TODO: return "TODO";
            case Status::IN_PROGRESS: return "In Progress";
            case Status::DONE: return "Done";
        }
        return "Unknown";
    }

    string getDueDateString() const {
        if (dueDate == 0) return "No due date";

        time_t now = time(nullptr);
        double diff = difftime(dueDate, now);
        int days = static_cast<int>(diff / 86400);

        if (days < 0) return "Overdue";
        if (days == 0) return "Today";
        if (days == 1) return "Tomorrow";
        if (days < 7) return to_string(days) + " days";

        char buf[20];
        strftime(buf, sizeof(buf), "%Y-%m-%d", localtime(&dueDate));
        return string(buf);
    }
};

// ==================== Todo 应用类 ====================

class TodoApp {
private:
    map<int, Task> tasks;  // id -> task
    unordered_map<string, set<int>> tagIndex;  // tag -> task ids
    set<int> freeIds;  // 可重用的 id
    int nextId = 1;

    // 优先级比较器（高优先级先）
    struct TaskCompare {
        bool operator()(const Task* a, const Task* b) const {
            if (a->priority != b->priority) {
                return a->priority < b->priority;
            }
            return a->dueDate > b->dueDate;  // 早到期优先
        }
    };

public:
    // 添加任务
    int addTask(const string& title, const string& description = "",
                Priority priority = Priority::MEDIUM,
                time_t dueDate = 0) {
        int id = getNextId();

        Task task;
        task.id = id;
        task.title = title;
        task.description = description;
        task.priority = priority;
        task.status = Status::TODO;
        task.dueDate = dueDate;

        tasks[id] = task;

        cout << "✓ 已添加任务 (ID: " << id << "): " << title << endl;
        return id;
    }

    // 更新任务状态
    bool updateStatus(int id, Status newStatus) {
        auto it = tasks.find(id);
        if (it == tasks.end()) {
            cout << "✗ 未找到任务 ID: " << id << endl;
            return false;
        }

        Status oldStatus = it->second.status;
        it->second.status = newStatus;

        cout << "✓ 任务 " << id << " 状态: "
             << it->second.getStatusString() << " -> "
             << (newStatus == Status::DONE ? "✓ 完成" :
                 newStatus == Status::IN_PROGRESS ? "→ 进行中" : "□ 待办")
             << endl;
        return true;
    }

    // 添加标签
    bool addTag(int taskId, const string& tag) {
        auto it = tasks.find(taskId);
        if (it == tasks.end()) {
            cout << "✗ 未找到任务 ID: " << taskId << endl;
            return false;
        }

        it->second.tags.insert(tag);
        tagIndex[tag].insert(taskId);

        cout << "✓ 已为任务 " << taskId << " 添加标签: " << tag << endl;
        return true;
    }

    // 删除任务
    bool deleteTask(int id) {
        auto it = tasks.find(id);
        if (it == tasks.end()) {
            cout << "✗ 未找到任务 ID: " << id << endl;
            return false;
        }

        // 从标签索引中删除
        for (const auto& tag : it->second.tags) {
            tagIndex[tag].erase(id);
            if (tagIndex[tag].empty()) {
                tagIndex.erase(tag);
            }
        }

        cout << "✗ 已删除任务 (ID: " << id << "): " << it->second.title << endl;
        tasks.erase(it);
        freeIds.insert(id);
        return true;
    }

    // 按优先级列出任务
    void listByPriority() const {
        cout << "\n=== 任务列表（按优先级） ===" << endl;

        priority_queue<const Task*, vector<const Task*>, TaskCompare> pq;
        for (const auto& p : tasks) {
            pq.push(&p.second);
        }

        printTasks(pq);
    }

    // 按状态列出任务
    void listByStatus(Status status) const {
        cout << "\n=== " << getStatusTitle(status) << " ===" << endl;

        vector<const Task*> filtered;
        for (const auto& p : tasks) {
            if (p.second.status == status) {
                filtered.push_back(&p.second);
            }
        }

        sort(filtered.begin(), filtered.end(),
             [](const Task* a, const Task* b) {
                 if (a->priority != b->priority) {
                     return a->priority > b->priority;
                 }
                 return a->dueDate < b->dueDate;
             });

        for (const auto* task : filtered) {
            printTask(*task);
        }
    }

    // 按标签搜索
    void searchByTag(const string& tag) const {
        cout << "\n=== 标签 '" << tag << "' 的任务 ===" << endl;

        auto it = tagIndex.find(tag);
        if (it == tagIndex.end() || it->second.empty()) {
            cout << "未找到带有此标签的任务" << endl;
            return;
        }

        for (int id : it->second) {
            auto taskIt = tasks.find(id);
            if (taskIt != tasks.end()) {
                printTask(taskIt->second);
            }
        }
    }

    // 显示所有标签
    void listAllTags() const {
        cout << "\n=== 所有标签 ===" << endl;

        if (tagIndex.empty()) {
            cout << "没有标签" << endl;
            return;
        }

        // 按任务数量排序
        vector<pair<string, size_t>> sortedTags;
        for (const auto& p : tagIndex) {
            sortedTags.push_back({p.first, p.second.size()});
        }

        sort(sortedTags.begin(), sortedTags.end(),
             [](const pair<string, size_t>& a, const pair<string, size_t>& b) {
                 return a.second > b.second;
             });

        for (const auto& p : sortedTags) {
            cout << "  " << p.first << " (" << p.second << " 个任务)" << endl;
        }
    }

    // 显示统计
    void showStats() const {
        cout << "\n=== 任务统计 ===" << endl;

        size_t total = tasks.size();
        size_t todo = 0, inProgress = 0, done = 0;
        size_t overdue = 0;

        time_t now = time(nullptr);

        for (const auto& p : tasks) {
            switch (p.second.status) {
                case Status::TODO: todo++; break;
                case Status::IN_PROGRESS: inProgress++; break;
                case Status::DONE: done++; break;
            }
            if (p.second.dueDate > 0 && p.second.dueDate < now &&
                p.second.status != Status::DONE) {
                overdue++;
            }
        }

        cout << "总任务数: " << total << endl;
        cout << "  待办: " << todo << endl;
        cout << "  进行中: " << inProgress << endl;
        cout << "  已完成: " << done << endl;
        cout << "已过期: " << overdue << endl;

        if (total > 0) {
            double completion = 100.0 * done / total;
            cout << "\n完成度: " << fixed << setprecision(1) << completion << "%" << endl;
        }

        cout << "标签数: " << tagIndex.size() << endl;
    }

    // 显示任务详情
    void showTask(int id) const {
        auto it = tasks.find(id);
        if (it == tasks.end()) {
            cout << "✗ 未找到任务 ID: " << id << endl;
            return;
        }

        cout << "\n=== 任务详情 ===" << endl;
        printTask(it->second, true);
    }

private:
    int getNextId() {
        if (!freeIds.empty()) {
            int id = *freeIds.begin();
            freeIds.erase(freeIds.begin());
            return id;
        }
        return nextId++;
    }

    string getStatusTitle(Status status) const {
        switch (status) {
            case Status::TODO: return "待办任务";
            case Status::IN_PROGRESS: return "进行中的任务";
            case Status::DONE: return "已完成的任务";
        }
        return "";
    }

    void printTask(const Task& task, bool detailed = false) const {
        cout << "[" << task.id << "] ";

        // 优先级标记
        string priorityMark;
        switch (task.priority) {
            case Priority::URGENT: priorityMark = "🔴"; break;
            case Priority::HIGH: priorityMark = "🟠"; break;
            case Priority::MEDIUM: priorityMark = "🟡"; break;
            case Priority::LOW: priorityMark = "🟢"; break;
        }

        // 状态标记
        string statusMark;
        switch (task.status) {
            case Status::DONE: statusMark = "✓"; break;
            case Status::IN_PROGRESS: statusMark = "→"; break;
            case Status::TODO: statusMark = "□"; break;
        }

        cout << priorityMark << " " << statusMark << " ";
        cout << task.title;

        if (!detailed) {
            cout << " [" << task.getDueDateString() << "]";
        }
        cout << endl;

        if (detailed) {
            if (!task.description.empty()) {
                cout << "    描述: " << task.description << endl;
            }
            cout << "    优先级: " << task.getPriorityString() << endl;
            cout << "    状态: " << task.getStatusString() << endl;
            cout << "    截止: " << task.getDueDateString() << endl;

            if (!task.tags.empty()) {
                cout << "    标签: ";
                for (const auto& tag : task.tags) {
                    cout << "#" << tag << " ";
                }
                cout << endl;
            }
        }
    }

    template<typename Queue>
    void printTasks(Queue& pq) const {
        while (!pq.empty()) {
            printTask(*pq.top());
            pq.pop();
        }
    }
};

// ==================== 辅助函数 ====================

Priority stringToPriority(const string& s) {
    if (s == "low" || s == "1") return Priority::LOW;
    if (s == "medium" || s == "2") return Priority::MEDIUM;
    if (s == "high" || s == "3") return Priority::HIGH;
    if (s == "urgent" || s == "4") return Priority::URGENT;
    return Priority::MEDIUM;
}

time_t parseDueDate(const string& dateStr) {
    if (dateStr == "0" || dateStr.empty()) return 0;

    // 简单解析 YYYY-MM-DD
    if (dateStr.size() == 10 && dateStr[4] == '-' && dateStr[7] == '-') {
        tm tm = {};
        istringstream ss(dateStr);
        char dash;
        ss >> tm.tm_year >> dash >> tm.tm_mon >> dash >> tm.tm_mday;
        tm.tm_year -= 1900;
        tm.tm_mon -= 1;
        return mktime(&tm);
    }

    // 支持天数偏移: +7 表示7天后
    if (dateStr[0] == '+') {
        int days = stoi(dateStr.substr(1));
        return time(nullptr) + days * 86400;
    }

    return 0;
}

// ==================== 交互式命令行界面 ====================

void runInteractive() {
    TodoApp app;
    cout << "=== 命令行待办事项管理器 ===" << endl;
    cout << "输入 'help' 查看命令列表" << endl;

    string line;
    while (true) {
        cout << "\n> ";
        getline(cin, line);

        if (line.empty()) continue;

        istringstream ss(line);
        string cmd;
        ss >> cmd;

        if (cmd == "help" || cmd == "h") {
            cout << R"(
命令列表:
  add <title>                    - 添加任务
  update <id> <status>           - 更新状态 (todo/in_progress/done)
  tag <id> <tag>                 - 添加标签
  delete <id>                    - 删除任务
  list                           - 列出所有任务（按优先级）
  status <s>                     - 按状态列出 (todo/in_progress/done)
  search <tag>                   - 按标签搜索
  tags                           - 列出所有标签
  show <id>                      - 显示任务详情
  stats                          - 显示统计
  quit                           - 退出
)" << endl;
        }
        else if (cmd == "add") {
            string title;
            getline(ss, title);
            if (!title.empty() && title[0] == ' ') title = title.substr(1);
            if (!title.empty()) {
                app.addTask(title);
            }
        }
        else if (cmd == "update") {
            int id;
            string statusStr;
            ss >> id >> statusStr;

            Status status = Status::TODO;
            if (statusStr == "in_progress" || statusStr == "progress") {
                status = Status::IN_PROGRESS;
            } else if (statusStr == "done") {
                status = Status::DONE;
            }

            app.updateStatus(id, status);
        }
        else if (cmd == "tag") {
            int id;
            string tag;
            ss >> id >> tag;
            app.addTag(id, tag);
        }
        else if (cmd == "delete") {
            int id;
            ss >> id;
            app.deleteTask(id);
        }
        else if (cmd == "list") {
            app.listByPriority();
        }
        else if (cmd == "status") {
            string statusStr;
            ss >> statusStr;

            Status status = Status::TODO;
            if (statusStr == "in_progress" || statusStr == "progress") {
                status = Status::IN_PROGRESS;
            } else if (statusStr == "done") {
                status = Status::DONE;
            }

            app.listByStatus(status);
        }
        else if (cmd == "search") {
            string tag;
            ss >> tag;
            app.searchByTag(tag);
        }
        else if (cmd == "tags") {
            app.listAllTags();
        }
        else if (cmd == "show") {
            int id;
            ss >> id;
            app.showTask(id);
        }
        else if (cmd == "stats") {
            app.showStats();
        }
        else if (cmd == "quit" || cmd == "exit" || cmd == "q") {
            cout << "再见！" << endl;
            break;
        }
        else {
            cout << "未知命令: " << cmd << " (输入 'help' 查看帮助)" << endl;
        }
    }
}

// ==================== 演示模式 ====================

void runDemo() {
    TodoApp app;

    cout << "=== 演示模式 ===" << endl;

    // 添加任务
    cout << "\n### 添加任务 ###" << endl;
    int t1 = app.addTask("完成项目报告", "编写Q4季度报告", Priority::HIGH);
    int t2 = app.addTask("代码审查", "审查PR #123", Priority::MEDIUM);
    int t3 = app.addTask("团队会议", "周一例会", Priority::LOW);
    int t4 = app.addTask("修复bug #456", "紧急问题", Priority::URGENT);

    // 添加标签
    cout << "\n### 添加标签 ###" << endl;
    app.addTag(t1, "work");
    app.addTag(t1, "important");
    app.addTag(t2, "code");
    app.addTag(t3, "meeting");
    app.addTag(t4, "bug");
    app.addTag(t4, "urgent");

    // 更新状态
    cout << "\n### 更新状态 ###" << endl;
    app.updateStatus(t2, Status::IN_PROGRESS);
    app.updateStatus(t3, Status::DONE);

    // 查看统计
    app.showStats();

    // 列出任务
    app.listByPriority();

    // 搜索标签
    app.searchByTag("urgent");

    // 显示详情
    app.showTask(t1);

    // 列出标签
    app.listAllTags();
}

// ==================== 主函数 ====================

int main(int argc, char* argv[]) {
    if (argc > 1 && string(argv[1]) == "--demo") {
        runDemo();
    } else {
        runInteractive();
    }

    return 0;
}
