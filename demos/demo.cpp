#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <filesystem>
#include <chrono>
#include <thread>
#include "serialPort/SerialPort.h"
#include "unitreeMotor/unitreeMotor.h"

std::vector<float> parseLine(const std::string& line) {
    std::vector<float> values;
    std::istringstream ss(line);
    std::string value;

    while (ss >> value) {
        try {
            float num = std::stof(value);
            values.push_back(num);
        }
        catch (std::invalid_argument& e) {
            values.push_back(0.0f);
        }
        ss >> std::ws;
    }
    values.resize(8);
    return values;
}

std::vector<std::vector<float>> readCSV(const std::filesystem::path& filepath) {
    std::vector<std::vector<float>> data;
    std::ifstream file(filepath);
    std::string line;

    if (file.is_open()) {
        while (std::getline(file, line)) {
            std::vector<float> row = parseLine(line);
            data.push_back(row);
        }
        file.close();
    }
    else {
        std::cout << "无法打开文件" << std::endl;
    }
    return data;
}

float sendCmd(const std::vector<float>& cmds, const std::string& serial_port) {
    SerialPort  serial(serial_port);
    MotorCmd    cmd;
    MotorData   data;

    cmd.motorType = MotorType::GO_M8010_6;
    data.motorType = MotorType::GO_M8010_6;
    cmd.id = static_cast<unsigned short>(cmds[0]);
    cmd.mode = static_cast<unsigned short>(cmds[1]);
    cmd.q = cmds[2];
    cmd.dq = cmds[3];
    cmd.kp = cmds[4];
    cmd.kd = cmds[5];
    cmd.kd = cmds[6];
    serial.sendRecv(&cmd, &data);

    std::cout << cmds[0] << "\t";
    std::cout << cmds[1] << "\t";
    std::cout << cmds[2] << "\t";
    std::cout << cmds[3] << "\t";
    std::cout << cmds[4] << "\t";
    std::cout << cmds[5] << "\t";
    std::cout << cmds[6] << "\t";
    std::cout << std::endl;

    std::cout << "\x1b[2G\x1b[A"; // 从第二行开始覆盖输出
    std::cout << data.q << "\t";
    std::cout << data.temp << "\t";
    std::cout << data.dq << "\t";
    std::cout << data.merror << "\t";
    std::cout << std::endl;

    return cmds[7];
}

int main(int argc, char* argv[]) {
    std::string serial_port = "/dev/ttyUSB0";
    std::filesystem::path filepath = argv[1]; // CSV文件名
    std::vector<std::vector<float>> data = readCSV(filepath); // 读取

    switch (argc) {
        case 1: std::cout << "Please input the parameters." << std::endl; return -1;
        case 2: std::cout << "No Serial Port input, using the default /dev/ttyUSB0." << std::endl; break;
        default:
            serial_port = argv[2];
            std::cout << "Starting with the Serial Port: " << serial_port << std::endl;
    }

    // 第一行
    std::cout <<  "motor.q\tmotor.temp\tmotor.W\tmotor.merror\t" << std::endl;
    // 第二行
    std::cout << "Waiting..." << std::endl;
    // 第三行
    std::cout << std::endl;
    //第四行
    std::cout << "id\tmode\tq\tdq\tkp\tkd\ttau\tcontinue_time\t" << std::endl;

    for (const auto& cmds : data) {
        float continue_time = sendCmd(cmds, serial_port);
        std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(continue_time)));
    }

    return 0;
}