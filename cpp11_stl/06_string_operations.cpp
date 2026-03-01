/**
 * STL string 和字符串操作详解
 *
 * 本文件涵盖：
 * 1. std::string 基本操作
 * 2. 字符串查找和替换
 * 3. 字符串转换和格式化
 * 4. 实际应用案例
 *
 * 编译：g++ -std=c++11 06_string_operations.cpp -o string_operations
 */

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <cctype>

using namespace std;

// ==================== 1. std::string 基本操作 ====================

void demoStringBasics() {
    cout << "=== std::string 基本操作 ===" << endl;

    // 1. 创建和初始化
    string s1;                           // 空字符串
    string s2 = "hello";                 // 初始化
    string s3(5, 'a');                   // "aaaaa"
    string s4(s2, 1, 3);                 // "ell" (从位置1开始，取3个字符)
    string s5 = "hello " + string("world");  // 拼接

    cout << "s2: " << s2 << endl;
    cout << "s3: " << s3 << endl;
    cout << "s4: " << s4 << endl;
    cout << "s5: " << s5 << endl;

    // 2. 容量操作
    cout << "\n容量操作:" << endl;
    cout << "size: " << s2.size() << endl;
    cout << "length: " << s2.length() << endl;
    cout << "capacity: " << s2.capacity() << endl;
    cout << "empty: " << s2.empty() << endl;

    s2.reserve(100);  // 预留空间
    cout << "after reserve(100), capacity: " << s2.capacity() << endl;

    s2.resize(10, 'x');  // 调整大小，填充 'x'
    cout << "after resize(10, 'x'): " << s2 << endl;

    // 3. 元素访问
    cout << "\n元素访问:" << endl;
    string s = "hello";
    cout << "s[0]: " << s[0] << endl;
    cout << "s.at(1): " << s.at(1) << endl;
    cout << "s.front(): " << s.front() << endl;
    cout << "s.back(): " << s.back() << endl;

    // 4. 迭代器
    cout << "\n使用迭代器遍历: ";
    for (auto it = s.begin(); it != s.end(); ++it) {
        cout << *it << " ";
    }
    cout << endl;

    // 5. 反向遍历
    cout << "反向遍历: ";
    for (auto it = s.rbegin(); it != s.rend(); ++it) {
        cout << *it << " ";
    }
    cout << endl;

    // 6. C 风格字符串转换
    cout << "\nC 风格转换:" << endl;
    const char* cstr = s.c_str();
    cout << "c_str(): " << cstr << endl;
}

// ==================== 2. 字符串查找和替换 ====================

void demoStringFindReplace() {
    cout << "\n=== 字符串查找和替换 ===" << endl;

    string s = "hello world, hello c++";

    // find - 查找子串
    size_t pos = s.find("hello");
    if (pos != string::npos) {
        cout << "found 'hello' at: " << pos << endl;
    }

    // rfind - 反向查找
    pos = s.rfind("hello");
    cout << "rfind 'hello': " << pos << endl;

    // find_first_of - 查找任意字符
    pos = s.find_first_of("aeiou");
    cout << "find_first_of('aeiou'): " << pos << endl;

    // find_last_of
    pos = s.find_last_of("aeiou");
    cout << "find_last_of('aeiou'): " << pos << endl;

    // find_first_not_of
    pos = s.find_first_not_of("helo ");
    cout << "find_first_not_of('helo '): " << pos << endl;

    // replace
    string s2 = "I like apples, apples are great";
    cout << "\n原始: " << s2 << endl;

    s2.replace(s2.find("apples"), 6, "oranges");
    cout << "replace 一次: " << s2 << endl;

    // 替换所有
    string s3 = "I like apples, apples are great";
    string search = "apples";
    string replace = "oranges";
    size_t pos2 = 0;
    while ((pos2 = s3.find(search, pos2)) != string::npos) {
        s3.replace(pos2, search.length(), replace);
        pos2 += replace.length();
    }
    cout << "replace all: " << s3 << endl;

    // substr - 子串
    string s4 = "hello world";
    cout << "\nsubstr(0, 5): " << s4.substr(0, 5) << endl;
    cout << "substr(6): " << s4.substr(6) << endl;

    // compare - 比较
    string a = "apple";
    string b = "banana";
    cout << "\ncompare('apple', 'banana'): " << a.compare(b) << endl;
    cout << "'apple' < 'banana': " << (a < b) << endl;
}

// ==================== 3. 字符串修改操作 ====================

void demoStringModification() {
    cout << "\n=== 字符串修改操作 ===" << endl;

    string s = "hello";

    // append - 追加
    s.append(" world");
    cout << "append: " << s << endl;

    s += "!";
    cout << "after += '!': " << s << endl;

    // push_back / pop_back
    s.push_back('?');
    cout << "push_back('?'): " << s << endl;
    s.pop_back();
    cout << "pop_back(): " << s << endl;

    // insert - 插入
    s.insert(5, " beautiful");
    cout << "insert: " << s << endl;

    // erase - 删除
    s.erase(5, 10);  // 从位置5删除10个字符
    cout << "erase: " << s << endl;

    // 清空
    s.clear();
    cout << "after clear, empty: " << s.empty() << endl;

    // 案例：删除所有空格
    string s2 = "a b c d e";
    s2.erase(remove(s2.begin(), s2.end(), ' '), s2.end());
    cout << "删除空格: " << s2 << endl;
}

// ==================== 4. 字符串转换 ====================

void demoStringConversion() {
    cout << "\n=== 字符串转换 ===" << endl;

    // 数字转字符串
    int i = 42;
    double d = 3.14;

    string s1 = to_string(i);
    string s2 = to_string(d);
    cout << "to_string(42): " << s1 << endl;
    cout << "to_string(3.14): " << s2 << endl;

    // 字符串转数字
    string numStr = "123";
    string floatStr = "3.14";

    int num = stoi(numStr);
    double fnum = stod(floatStr);
    cout << "stoi('123'): " << num << endl;
    cout << "stod('3.14'): " << fnum << endl;

    // 处理转换异常
    try {
        size_t pos;
        string s = "123abc";
        int n = stoi(s, &pos);
        cout << "stoi('123abc'): " << n << ", pos: " << pos << endl;
    } catch (const exception& e) {
        cout << "转换失败: " << e.what() << endl;
    }

    // 大小写转换
    string s = "Hello World";
    string upper = s, lower = s;

    transform(upper.begin(), upper.end(), upper.begin(), ::toupper);
    transform(lower.begin(), lower.end(), lower.begin(), ::tolower);

    cout << "\n大写: " << upper << endl;
    cout << "小写: " << lower << endl;
}

// ==================== 5. 字符串分割和连接 ====================

void demoStringSplitJoin() {
    cout << "\n=== 字符串分割和连接 ===" << endl;

    // 使用 stringstream 分割
    string s = "hello,world,cpp,string";
    stringstream ss(s);
    string token;
    vector<string> tokens;

    while (getline(ss, token, ',')) {
        tokens.push_back(token);
    }

    cout << "分割结果: ";
    for (const auto& t : tokens) {
        cout << t << " ";
    }
    cout << endl;

    // 连接字符串
    string joined;
    for (size_t i = 0; i < tokens.size(); ++i) {
        if (i > 0) joined += " | ";
        joined += tokens[i];
    }
    cout << "连接结果: " << joined << endl;
}

// ==================== 6. 实际应用案例 ====================

// 案例1：路径处理
void demoPathProcessing() {
    cout << "\n=== 路径处理 ===" << endl;

    string path = "/home/user/documents/file.txt";

    // 获取目录名
    size_t lastSlash = path.find_last_of("/\\");
    string dir = path.substr(0, lastSlash);
    string filename = path.substr(lastSlash + 1);

    cout << "目录: " << dir << endl;
    cout << "文件名: " << filename << endl;

    // 获取扩展名
    size_t dotPos = filename.find_last_of('.');
    string name = filename.substr(0, dotPos);
    string ext = filename.substr(dotPos + 1);

    cout << "主文件名: " << name << endl;
    cout << "扩展名: " << ext << endl;
}

// 案例2：CSV 解析
void demoCSVParsing() {
    cout << "\n=== CSV 解析 ===" << endl;

    string csvLine = "John,Doe,30,New York";

    vector<string> fields;
    stringstream ss(csvLine);
    string field;

    while (getline(ss, field, ',')) {
        // 去除首尾空格
        field.erase(0, field.find_first_not_of(" \t"));
        field.erase(field.find_last_not_of(" \t") + 1);
        fields.push_back(field);
    }

    cout << "CSV 字段:" << endl;
    for (size_t i = 0; i < fields.size(); ++i) {
        cout << "  " << i << ": " << fields[i] << endl;
    }
}

// 案例3：URL 解析
void demoURLParsing() {
    cout << "\n=== URL 解析 ===" << endl;

    string url = "https://www.example.com:8080/path/to/page?query=value#fragment";

    struct URLParts {
        string protocol;
        string host;
        string port;
        string path;
        string query;
        string fragment;
    } parts;

    // 解析协议
    size_t pos = url.find("://");
    if (pos != string::npos) {
        parts.protocol = url.substr(0, pos);
        url = url.substr(pos + 3);
    }

    // 解析路径
    pos = url.find('/');
    string hostPort = url.substr(0, pos);
    parts.path = url.substr(pos);

    // 解析主机和端口
    pos = hostPort.find(':');
    if (pos != string::npos) {
        parts.host = hostPort.substr(0, pos);
        parts.port = hostPort.substr(pos + 1);
    } else {
        parts.host = hostPort;
    }

    // 解析查询和片段
    pos = parts.path.find('?');
    if (pos != string::npos) {
        parts.query = parts.path.substr(pos + 1);
        parts.path = parts.path.substr(0, pos);
    }

    pos = parts.query.find('#');
    if (pos != string::npos) {
        parts.fragment = parts.query.substr(pos + 1);
        parts.query = parts.query.substr(0, pos);
    }

    cout << "协议: " << parts.protocol << endl;
    cout << "主机: " << parts.host << endl;
    cout << "端口: " << parts.port << endl;
    cout << "路径: " << parts.path << endl;
    cout << "查询: " << parts.query << endl;
    cout << "片段: " << parts.fragment << endl;
}

// 案例4：字符串格式化
void demoStringFormatting() {
    cout << "\n=== 字符串格式化 ===" << endl;

    // 使用 stringstream
    stringstream ss;

    string name = "Alice";
    int age = 30;
    double score = 95.5;

    ss << "Name: " << name << ", Age: " << age \
    << ", Score: " << fixed << setprecision(1) << score;

    cout << "格式化结果: " << ss.str() << endl;

    // 对齐
    vector<string> items = {"apple", "banana", "cherry"};
    vector<double> prices = {1.99, 2.49, 3.99};

    cout << "\n商品列表:" << endl;
    for (size_t i = 0; i < items.size(); ++i) {
        cout << left << setw(15) << items[i]
             << right << setw(8) << fixed << setprecision(2) << prices[i]
             << endl;
    }
}

// 案例5：去除首尾空白
string trim(const string& s) {
    auto start = s.find_first_not_of(" \t\n\r");
    if (start == string::npos) return "";

    auto end = s.find_last_not_of(" \t\n\r");
    return s.substr(start, end - start + 1);
}

void demoTrim() {
    cout << "\n=== 去除首尾空白 ===" << endl;

    string s = "   \t  hello world  \n\t";
    cout << "原始: '" << s << "'" << endl;
    cout << "trim: '" << trim(s) << "'" << endl;
}

// ==================== 主函数 ====================

int main() {
    demoStringBasics();
    demoStringFindReplace();
    demoStringModification();
    demoStringConversion();
    demoStringSplitJoin();
    demoPathProcessing();
    demoCSVParsing();
    demoURLParsing();
    demoStringFormatting();
    demoTrim();

    cout << "\n=== string 常用方法总结 ===" << endl;
    cout << R"(
基本操作：
- size() / length() - 长度
- empty() - 是否为空
- capacity() - 容量
- reserve() - 预留空间
- resize() - 调整大小
- clear() - 清空

访问：
- operator[] / at() - 访问字符
- front() / back() - 首尾字符
- c_str() - C 风格字符串

修改：
- append() / operator+= - 追加
- push_back() / pop_back() - 单字符
- insert() - 插入
- erase() - 删除
- replace() - 替换

查找：
- find() / rfind() - 查找子串
- find_first_of() / find_last_of() - 查找字符
- compare() - 比较

转换：
- to_string() - 数字转字符串
- stoi() / stod() - 字符串转数字

实用函数：
- substr() - 子串
- getline() - 读取一行
)" << endl;

    return 0;
}
