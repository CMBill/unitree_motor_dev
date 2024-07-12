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

//    while (ss >> value) {
    while (std::getline(ss, value, ',')) {
        try {
            float num = std::stof(value);
            values.push_back(num);
        }
        catch (std::invalid_argument& e) {
            values.push_back(0.0f);
        }
//        ss >> std::ws;
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
    cmd.tau = cmds[6];
    serial.sendRecv(&cmd, &data);

    for(int i = 0; i <= 6; i++) {
        std::cout << cmds[i] << " ";
    }

    std::cout << std::endl;

    std::cout << data.q << " "
              << data.temp << " "
              << data.dq << " "
              << data.merror << " " << std::endl << std::endl;

    return cmds[7];
}

int main(int argc, char* argv[]) {
    std::string serial_port = "/dev/ttyUSB0";
    std::filesystem::path filepath; // CSV文件名
    std::vector<std::vector<float>> data; // 读取

    switch (argc) {
        case 1: std::cout << "Please input the parameters." << std::endl; return -1;
        case 2:
            std::cout << "No Serial Port input, using the default /dev/ttyUSB0." << std::endl;
            filepath = argv[1];
            break;
        default:
            serial_port = argv[2];
            filepath = argv[1];
            std::cout << "Starting with the Serial Port: " << serial_port << std::endl;
    }

    data = readCSV(filepath);
    for (const auto& cmds : data) {
        float continue_time = sendCmd(cmds, serial_port);
        // std::cout << serial_port << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(continue_time)));
    }

    return 0;
}