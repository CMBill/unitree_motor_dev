# README.md

程序运行时传入参数：

```shell
sudo ./motor_control_demo config_file serial_port
```

- `config_file`：电机控制指令存储的文件，在此中存储控制电机的7个命令。文件以空格分隔，每一行为：

  ```shell
  id mode q dq kp kd tau continue_time
  ```

    其中前7个为对应的7个命令，最后一个为持续时间（单位毫秒）。

示例config_file位于 `config` 文件夹下。

## Build
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
