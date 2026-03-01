/**
 * 综合应用：文本分析工具
 *
 * 本文件展示如何使用 STL 容器实现文本分析：
 * 1. 词频统计
 * 2. 搜索和替换
 * 3. 文本相似度计算
 * 4. 简单的拼写检查建议
 *
 * 编译：g++ -std=c++11 08_practical_text_analysis.cpp -o text_analysis
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <set>
#include <algorithm>
#include <queue>
#include <cmath>
#include <numeric>
#include <iomanip>

using namespace std;

// ==================== 1. 词频统计器 ====================

class WordFrequencyAnalyzer {
private:
    unordered_map<string, int> wordCount;
    set<string> stopWords;

    void initStopWords() {
        stopWords = {
            "the", "a", "an", "and", "or", "but", "is", "are", "was", "were",
            "in", "on", "at", "to", "for", "of", "with", "by", "from", "as",
            "it", "this", "that", "these", "those", "be", "been", "being",
            "have", "has", "had", "do", "does", "did", "will", "would",
            "can", "could", "should", "may", "might", "must", "i", "you",
            "he", "she", "we", "they", "what", "which", "who", "when", "where",
            "why", "how", "all", "each", "every", "both", "few", "more", "most"
        };
    }

    string normalizeWord(string word) const {
        // 转小写
        transform(word.begin(), word.end(), word.begin(), ::tolower);

        // 移除开头和结尾的非字母字符
        word.erase(remove_if(word.begin(), word.end(),
                             [](char c) { return !isalpha(c); }),
                   word.end());

        return word;
    }

public:
    WordFrequencyAnalyzer() {
        initStopWords();
    }

    void analyzeText(const string& text) {
        stringstream ss(text);
        string word;

        while (ss >> word) {
            string normalized = normalizeWord(word);
            if (!normalized.empty() && stopWords.find(normalized) == stopWords.end()) {
                wordCount[normalized]++;
            }
        }
    }

    void analyzeFile(const string& filename) {
        ifstream file(filename);
        if (!file.is_open()) {
            cerr << "无法打开文件: " << filename << endl;
            return;
        }

        string word;
        while (file >> word) {
            string normalized = normalizeWord(word);
            if (!normalized.empty() && stopWords.find(normalized) == stopWords.end()) {
                wordCount[normalized]++;
            }
        }
    }

    void printTopWords(size_t n = 20) const {
        // 转换为 vector 以便排序
        vector<pair<string, int>> sortedWords(wordCount.begin(), wordCount.end());

        sort(sortedWords.begin(), sortedWords.end(),
             [](const pair<string, int>& a, const pair<string, int>& b) {
                 return a.second > b.second;
             });

        cout << "\n=== Top " << n << " 高频词 ===" << endl;
        cout << left << setw(20) << "单词"
             << right << setw(10) << "出现次数" << endl;
        cout << string(30, '-') << endl;

        for (size_t i = 0; i < min(n, sortedWords.size()); ++i) {
            cout << left << setw(20) << sortedWords[i].first
                 << right << setw(10) << sortedWords[i].second << endl;
        }
    }

    int getWordCount(const string& word) const {
        string normalized = normalizeWord(word);
        auto it = wordCount.find(normalized);
        return it != wordCount.end() ? it->second : 0;
    }

    size_t totalUniqueWords() const {
        return wordCount.size();
    }

    size_t totalWords() const {
        return accumulate(wordCount.begin(), wordCount.end(), 0,
                         [](size_t sum, const pair<string, int>& p) {
                             return sum + p.second;
                         });
    }
};

// ==================== 2. 文本搜索引擎 ====================

class TextSearchEngine {
private:
    vector<string> lines;
    unordered_map<string, set<size_t>> invertedIndex;  // 单词 -> 行号集合

    void buildIndex() {
        for (size_t i = 0; i < lines.size(); ++i) {
            stringstream ss(lines[i]);
            string word;
            while (ss >> word) {
                // 标准化单词
                transform(word.begin(), word.end(), word.begin(), ::tolower);
                word.erase(remove_if(word.begin(), word.end(),
                                    [](char c) { return !isalpha(c); }),
                          word.end());
                if (!word.empty()) {
                    invertedIndex[word].insert(i);
                }
            }
        }
    }

public:
    void loadText(const string& text) {
        stringstream ss(text);
        string line;
        while (getline(ss, line)) {
            lines.push_back(line);
        }
        buildIndex();
    }

    void loadFile(const string& filename) {
        ifstream file(filename);
        string line;
        while (getline(file, line)) {
            lines.push_back(line);
        }
        buildIndex();
    }

    // 搜索单个单词
    vector<size_t> searchWord(const string& word) const {
        string normalized = word;
        transform(normalized.begin(), normalized.end(), normalized.begin(), ::tolower);

        auto it = invertedIndex.find(normalized);
        if (it != invertedIndex.end()) {
            return vector<size_t>(it->second.begin(), it->second.end());
        }
        return {};
    }

    // 搜索多个单词（AND 查询）
    vector<size_t> searchAll(const vector<string>& words) const {
        if (words.empty()) return {};

        auto results = searchWord(words[0]);

        for (size_t i = 1; i < words.size(); ++i) {
            auto current = searchWord(words[i]);
            vector<size_t> intersection;

            set_intersection(results.begin(), results.end(),
                           current.begin(), current.end(),
                           back_inserter(intersection));

            results = intersection;
        }

        return results;
    }

    // 显示搜索结果
    void displayResults(const vector<size_t>& lineNumbers, size_t context = 1) const {
        if (lineNumbers.empty()) {
            cout << "未找到匹配" << endl;
            return;
        }

        cout << "\n找到 " << lineNumbers.size() << " 个匹配:" << endl;

        for (size_t lineNum : lineNumbers) {
            cout << "\n行 " << (lineNum + 1) << ": ";

            // 显示上下文
            size_t start = max(size_t(0), lineNum - context);
            size_t end = min(lines.size() - 1, lineNum + context);

            for (size_t i = start; i <= end; ++i) {
                string prefix = (i == lineNum) ? ">>>" : "   ";
                cout << prefix << lines[i] << endl;
            }
        }
    }
};

// ==================== 3. 文本相似度计算 ====================

class TextSimilarity {
public:
    // 计算余弦相似度
    static double cosineSimilarity(const unordered_map<string, int>& doc1,
                                   const unordered_map<string, int>& doc2) {
        double dotProduct = 0;
        double norm1 = 0;
        double norm2 = 0;

        // 计算点积
        for (const auto& p : doc1) {
            auto it = doc2.find(p.first);
            if (it != doc2.end()) {
                dotProduct += p.second * it->second;
            }
            norm1 += p.second * p.second;
        }

        for (const auto& p : doc2) {
            norm2 += p.second * p.second;
        }

        if (norm1 == 0 || norm2 == 0) return 0;

        return dotProduct / (sqrt(norm1) * sqrt(norm2));
    }

    // 计算编辑距离（Levenshtein 距离）
    static int editDistance(const string& s1, const string& s2) {
        int m = s1.length();
        int n = s2.length();

        vector<vector<int>> dp(m + 1, vector<int>(n + 1));

        for (int i = 0; i <= m; ++i) dp[i][0] = i;
        for (int j = 0; j <= n; ++j) dp[0][j] = j;

        for (int i = 1; i <= m; ++i) {
            for (int j = 1; j <= n; ++j) {
                if (s1[i - 1] == s2[j - 1]) {
                    dp[i][j] = dp[i - 1][j - 1];
                } else {
                    dp[i][j] = 1 + min({dp[i - 1][j],      // 删除
                                        dp[i][j - 1],      // 插入
                                        dp[i - 1][j - 1]}); // 替换
                }
            }
        }

        return dp[m][n];
    }

    // 计算相似度百分比（基于编辑距离）
    static double similarityRatio(const string& s1, const string& s2) {
        int dist = editDistance(s1, s2);
        int maxLen = max(s1.length(), s2.length());
        if (maxLen == 0) return 1.0;
        return 1.0 - static_cast<double>(dist) / maxLen;
    }
};

// ==================== 4. 拼写检查建议 ====================

class SpellChecker {
private:
    set<string> dictionary;

    // 生成编辑距离为 1 的所有可能单词
    vector<string> edits1(const string& word) const {
        vector<string> result;

        // 删除
        for (size_t i = 0; i < word.size(); ++i) {
            result.push_back(word.substr(0, i) + word.substr(i + 1));
        }

        // 替换
        for (size_t i = 0; i < word.size(); ++i) {
            for (char c = 'a'; c <= 'z'; ++c) {
                result.push_back(word.substr(0, i) + c + word.substr(i + 1));
            }
        }

        // 插入
        for (size_t i = 0; i <= word.size(); ++i) {
            for (char c = 'a'; c <= 'z'; ++c) {
                result.push_back(word.substr(0, i) + c + word.substr(i));
            }
        }

        return result;
    }

public:
    void loadDictionary(const set<string>& dict) {
        dictionary = dict;
    }

    // 简单的拼写建议
    vector<string> getSuggestions(const string& word, size_t maxSuggestions = 5) const {
        vector<string> suggestions;
        priority_queue<pair<int, string>, vector<pair<int, string>>,
                      greater<pair<int, string>>> pq;

        // 检查编辑距离为 1 的单词
        auto candidates = edits1(word);
        for (const auto& candidate : candidates) {
            if (dictionary.find(candidate) != dictionary.end()) {
                int dist = TextSimilarity::editDistance(word, candidate);
                pq.push({dist, candidate});
            }
        }

        while (!pq.empty() && suggestions.size() < maxSuggestions) {
            suggestions.push_back(pq.top().second);
            pq.pop();
        }

        return suggestions;
    }

    bool isCorrect(const string& word) const {
        return dictionary.find(word) != dictionary.end();
    }
};

// ==================== 主函数 ====================

int main() {
    cout << "=== 文本分析工具集 ===" << endl;

    // 示例文本
    string sampleText = R"(
The quick brown fox jumps over the lazy dog.
The dog was not amused by the fox's jumping.
Quick thinking helps in programming.
Programming requires patience and practice.
The fox ran away quickly.
)";

    // ==================== 1. 词频统计 ====================
    cout << "\n### 词频统计 ###" << endl;

    WordFrequencyAnalyzer analyzer;
    analyzer.analyzeText(sampleText);
    analyzer.printTopWords(10);

    cout << "\n统计信息:" << endl;
    cout << "唯一词数: " << analyzer.totalUniqueWords() << endl;
    cout << "总词数: " << analyzer.totalWords() << endl;
    cout << "'fox' 出现次数: " << analyzer.getWordCount("fox") << endl;

    // ==================== 2. 文本搜索 ====================
    cout << "\n### 文本搜索 ###" << endl;

    TextSearchEngine searchEngine;
    searchEngine.loadText(sampleText);

    cout << "\n搜索单词 'fox':" << endl;
    auto results = searchEngine.searchWord("fox");
    searchEngine.displayResults(results);

    cout << "\n搜索 'fox' AND 'dog':" << endl;
    auto results2 = searchEngine.searchAll({"fox", "dog"});
    searchEngine.displayResults(results2);

    // ==================== 3. 文本相似度 ====================
    cout << "\n### 文本相似度 ###" << endl;

    string s1 = "hello world";
    string s2 = "hello word";
    string s3 = "hello there world";

    cout << "\n编辑距离:" << endl;
    cout << "'" << s1 << "' vs '" << s2 << "': "
         << TextSimilarity::editDistance(s1, s2) << endl;
    cout << "相似度: "
         << fixed << setprecision(2)
         << TextSimilarity::similarityRatio(s1, s2) * 100 << "%" << endl;

    cout << "\n'" << s1 << "' vs '" << s3 << "': "
         << TextSimilarity::editDistance(s1, s3) << endl;
    cout << "相似度: "
         << fixed << setprecision(2)
         << TextSimilarity::similarityRatio(s1, s3) * 100 << "%" << endl;

    // 词向量余弦相似度
    unordered_map<string, int> doc1 = {{"hello", 1}, {"world", 1}};
    unordered_map<string, int> doc2 = {{"hello", 2}, {"world", 1}, {"there", 1}};

    cout << "\n词向量余弦相似度: "
         << fixed << setprecision(2)
         << TextSimilarity::cosineSimilarity(doc1, doc2) << endl;

    // ==================== 4. 拼写检查 ====================
    cout << "\n### 拼写检查 ###" << endl;

    set<string> dictionary = {
        "hello", "world", "quick", "brown", "fox", "dog",
        "jumps", "over", "lazy", "programming", "think", "help"
    };

    SpellChecker checker;
    checker.loadDictionary(dictionary);

    vector<string> testWords = {"helo", "wrold", "progrming", "qick"};
    for (const auto& word : testWords) {
        cout << "\n单词: " << word << endl;
        if (!checker.isCorrect(word)) {
            auto suggestions = checker.getSuggestions(word);
            cout << "  建议: ";
            for (const auto& s : suggestions) {
                cout << s << " ";
            }
            cout << endl;
        } else {
            cout << "  拼写正确" << endl;
        }
    }

    cout << "\n=== 分析完成 ===" << endl;
    return 0;
}
