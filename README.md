# README.md

主要源码为 `demos/demo.cpp`，默认编译为`demo`文件。

程序运行时传入参数：

```shell
sudo ./demo config_file serial_port
```

- `config_file`：电机控制指令存储的文件，在此中存储控制电机的7个命令。文件以逗号分隔，每一行为：

  ```csv
  id, mode, q, dq, kp, kd, tau, continue_time
  ```

    其中前7个为对应的7个命令，最后一个为持续时间（单位毫秒）。
- `serial_port`: 电机连接的串口，默认为 `/dev/ttyUSB0`。

示例config_file位于 `config` 文件夹下。

## Build
在项目根目录
```shell
mkdir build
cd build
cmake ..
make
```

## Run
使用示例的配置文件（`config/config.csv`），串口为 `/dev/ttyUSB0`。
```shell
sudo ./demo ../config/config.csv /dev/ttyUSB0
```
