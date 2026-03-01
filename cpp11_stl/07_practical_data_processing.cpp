/**
 * 综合应用：数据处理工具集
 *
 * 本文件展示如何组合使用 STL 容器解决实际问题：
 * 1. 读取和解析 CSV 数据
 * 2. 数据统计和分析
 * 3. 数据分组和聚合
 * 4. 数据排序和筛选
 *
 * 编译：g++ -std=c++11 07_practical_data_processing.cpp -o data_processing
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <functional>
#include <vector>
#include <string>
#include <map>
#include <unordered_map>
#include <set>
#include <algorithm>
#include <iomanip>
#include <numeric>

using namespace std;

// ==================== 数据结构定义 ====================

struct SalesRecord {
    string date;
    string product;
    string category;
    int quantity;
    double price;
    string region;

    double total() const { return quantity * price; }

    // 从 CSV 行解析
    static SalesRecord parse(const string& line) {
        stringstream ss(line);
        string field;
        vector<string> fields;

        while (getline(ss, field, ',')) {
            // 去除引号（如果有）
            if (!field.empty() && field.front() == '"') {
                field = field.substr(1, field.size() - 2);
            }
            fields.push_back(field);
        }

        if (fields.size() >= 7) {
            return {
                fields[0],
                fields[1],
                fields[2],
                stoi(fields[3]),
                stod(fields[4]),
                fields[6]
            };
        }
        throw runtime_error("Invalid CSV format");
    }
};

// ==================== 1. CSV 文件读取和解析 ====================

class CSVReader {
public:
    static vector<SalesRecord> readFile(const string& filename) {
        ifstream file(filename);
        vector<SalesRecord> records;

        if (!file.is_open()) {
            cerr << "无法打开文件: " << filename << endl;
            return records;
        }

        string line;
        // 跳过表头
        getline(file, line);

        // 读取数据
        while (getline(file, line)) {
            if (!line.empty()) {
                try {
                    records.push_back(SalesRecord::parse(line));
                } catch (const exception& e) {
                    cerr << "解析错误: " << e.what() << " - " << line << endl;
                }
            }
        }

        return records;
    }

    // 生成示例数据
    static void generateSampleData(const string& filename) {
        ofstream file(filename);
        file << "Date,Product,Category,Quantity,UnitPrice,Total,Region\n";

        vector<string> products = {"Laptop", "Mouse", "Keyboard", "Monitor", "Headphones"};
        vector<string> categories = {"Electronics", "Accessories", "Accessories", "Electronics", "Accessories"};
        vector<double> prices = {999.99, 29.99, 49.99, 299.99, 79.99};
        vector<string> regions = {"North", "South", "East", "West"};

        for (int day = 1; day <= 30; ++day) {
            for (size_t i = 0; i < products.size(); ++i) {
                int qty = 1 + (rand() % 10);
                string date = string("2024-01-") + (day < 10 ? "0" : "") + to_string(day);
                string region = regions[rand() % regions.size()];

                file << date << ","
                     << "\"" << products[i] << "\","
                     << "\"" << categories[i] << "\","
                     << qty << ","
                     << prices[i] << ","
                     << qty * prices[i] << ","
                     << region << "\n";
            }
        }

        file.close();
        cout << "示例数据已生成: " << filename << endl;
    }
};

// ==================== 2. 数据分析类 ====================

class SalesAnalyzer {
private:
    vector<SalesRecord> records;

public:
    SalesAnalyzer(const vector<SalesRecord>& r) : records(r) {}

    // 基本统计
    void printBasicStats() const {
        if (records.empty()) return;

        double totalSales = 0;
        int totalQuantity = 0;

        for (const auto& r : records) {
            totalSales += r.total();
            totalQuantity += r.quantity;
        }

        cout << "\n=== 基本统计 ===" << endl;
        cout << "总记录数: " << records.size() << endl;
        cout << "总销售额: $" << fixed << setprecision(2) << totalSales << endl;
        cout << "总销量: " << totalQuantity << endl;
        cout << "平均订单额: $" << totalSales / records.size() << endl;
    }

    // 按类别统计销售额
    void analyzeByCategory() const {
        map<string, double> categorySales;
        map<string, int> categoryCount;

        for (const auto& r : records) {
            categorySales[r.category] += r.total();
            categoryCount[r.category]++;
        }

        cout << "\n=== 按类别统计 ===" << endl;
        cout << left << setw(20) << "类别"
             << right << setw(12) << "订单数"
             << setw(15) << "销售额" << endl;
        cout << string(47, '-') << endl;

        for (const auto& p : categorySales) {
            cout << left << setw(20) << p.first
                 << right << setw(12) << categoryCount.at(p.first)
                 << setw(14) << fixed << setprecision(2) << p.second << endl;
        }
    }

    // 按产品统计（Top N）
    void analyzeByProduct(size_t topN = 5) const {
        map<string, pair<double, int>> productData;  // <产品, <销售额, 数量>>

        for (const auto& r : records) {
            productData[r.product].first += r.total();
            productData[r.product].second += r.quantity;
        }

        // 转换为 vector 以便排序
        vector<pair<string, pair<double, int>>> products(
            productData.begin(), productData.end());

        sort(products.begin(), products.end(),
             [](const pair<string, pair<double, int>>& a, const pair<string, pair<double, int>>& b) {
                 return a.second.first > b.second.first;
             });

        cout << "\n=== Top " << topN << " 产品（按销售额） ===" << endl;
        cout << left << setw(20) << "产品"
             << right << setw(12) << "销量"
             << setw(15) << "销售额" << endl;
        cout << string(47, '-') << endl;

        for (size_t i = 0; i < min(topN, products.size()); ++i) {
            cout << left << setw(20) << products[i].first
                 << right << setw(12) << products[i].second.second
                 << setw(14) << fixed << setprecision(2)
                 << products[i].second.first << endl;
        }
    }

    // 按地区统计
    void analyzeByRegion() const {
        unordered_map<string, double> regionSales;

        for (const auto& r : records) {
            regionSales[r.region] += r.total();
        }

        cout << "\n=== 按地区统计 ===" << endl;
        for (const auto& p : regionSales) {
            cout << p.first << ": $" << fixed << setprecision(2) << p.second << endl;
        }
    }

    // 时间趋势分析
    void analyzeTimeTrend() const {
        map<string, double> dailySales;

        for (const auto& r : records) {
            dailySales[r.date] += r.total();
        }

        cout << "\n=== 每日销售额趋势 ===" << endl;
        cout << left << setw(15) << "日期"
             << right << setw(15) << "销售额" << endl;
        cout << string(30, '-') << endl;

        // 只显示前10天和后10天
        size_t count = 0;
        for (const auto& p : dailySales) {
            if (count < 10 || count >= dailySales.size() - 10) {
                cout << left << setw(15) << p.first
                     << right << setw(14) << fixed << setprecision(2)
                     << p.second << endl;
            }
            count++;
        }
    }

    // 筛选数据
    vector<SalesRecord> filter(function<bool(const SalesRecord&)> pred) const {
        vector<SalesRecord> result;
        copy_if(records.begin(), records.end(), back_inserter(result), pred);
        return result;
    }

    // 查找特定产品
    void findProductSales(const string& productName) const {
        auto filtered = filter([&](const SalesRecord& r) {
            return r.product == productName;
        });

        if (filtered.empty()) {
            cout << "\n未找到产品: " << productName << endl;
            return;
        }

        double total = accumulate(filtered.begin(), filtered.end(), 0.0,
                                  [](double sum, const SalesRecord& r) {
                                      return sum + r.total();
                                  });

        cout << "\n=== " << productName << " 销售详情 ===" << endl;
        cout << "订单数: " << filtered.size() << endl;
        cout << "总销售额: $" << fixed << setprecision(2) << total << endl;

        // 找出购买最多的地区
        unordered_map<string, int> regionCount;
        for (const auto& r : filtered) {
            regionCount[r.region]++;
        }

        string topRegion = max_element(regionCount.begin(), regionCount.end(),
                                       [](const pair<string, int>& a, const pair<string, int>& b) {
                                           return a.second < b.second;
                                       })->first;

        cout << "主要销售地区: " << topRegion
             << " (" << regionCount[topRegion] << " 笔订单)" << endl;
    }
};

// ==================== 主函数 ====================

int main() {
    cout << "=== 销售数据分析系统 ===" << endl;

    // 生成示例数据
    const string filename = "sales_data.csv";
    CSVReader::generateSampleData(filename);

    // 读取数据
    auto records = CSVReader::readFile(filename);
    if (records.empty()) {
        cerr << "没有数据可分析" << endl;
        return 1;
    }

    // 创建分析器
    SalesAnalyzer analyzer(records);

    // 执行分析
    analyzer.printBasicStats();
    analyzer.analyzeByCategory();
    analyzer.analyzeByProduct(5);
    analyzer.analyzeByRegion();
    analyzer.analyzeTimeTrend();

    // 查找特定产品
    analyzer.findProductSales("Laptop");

    // 高级筛选示例
    cout << "\n=== 高级筛选示例 ===" << endl;

    // 筛选高价值订单（单笔超过$500）
    auto highValueOrders = analyzer.filter([](const SalesRecord& r) {
        return r.total() > 500;
    });
    cout << "高价值订单数(>$500): " << highValueOrders.size() << endl;

    // 筛选特定地区
    auto northOrders = analyzer.filter([](const SalesRecord& r) {
        return r.region == "North";
    });
    double northTotal = 0;
    for (const auto& r : northOrders) {
        northTotal += r.total();
    }
    cout << "North 地区总销售额: $" << fixed << setprecision(2) << northTotal << endl;

    // 按时间段筛选
    auto firstWeekOrders = analyzer.filter([](const SalesRecord& r) {
        return r.date >= "2024-01-01" && r.date <= "2024-01-07";
    });
    cout << "第一周订单数: " << firstWeekOrders.size() << endl;

    cout << "\n=== 分析完成 ===" << endl;

    return 0;
}
