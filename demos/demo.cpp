#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <filesystem>

// 将CSV文件的一行解析为向量
std::vector<double> parseLine(const std::string& line) {
    std::vector<double> values;
    std::istringstream ss(line);
    std::string value;

    // 使用std::ws作为分隔符，读取空格分隔的数值
    while (ss >> value) {
        try {
            double num = std::stod(value);
            values.push_back(num);
        }
        catch (std::invalid_argument& e) {
            // 如果转换失败，可能是空字符串，存储0
            values.push_back(0);
        }
        ss >> std::ws;
    }
    values.resize(8);
    return values;
}

// 读取CSV文件并存储为二维向量
std::vector<std::vector<double>> readCSV(const std::filesystem::path& filepath) {
    std::vector<std::vector<double>> data;
    std::ifstream file(filepath);
    std::string line;

    if (file.is_open()) {
        while (std::getline(file, line)) {
            std::vector<double> row = parseLine(line);
            data.push_back(row);
        }
        file.close();
    }
    else {
        std::cout << "无法打开文件" << std::endl;
    }

    return data;
}

// 打印二维向量
void printData(const std::vector<std::vector<double>>& data) {
    for (const auto& row : data) {
        for (const auto& val : row) {
            std::cout << val << " ";
        }
        std::cout << std::endl;
    }
}

int main() {
    std::filesystem::path filepath = "../config/config.csv"; // CSV文件名
    std::vector<std::vector<double>> data = readCSV(filepath);
    printData(data);
    return 0;
}