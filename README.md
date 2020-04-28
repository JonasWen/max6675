# MAX6675 软件包

## 简介

MAX6675 软件包提供了使用温度传感器 `max6675` 基本功能，并且本软件包新的版本已经对接到了 Sensor 框架，通过 Sensor 框架，开发者可以快速的将此传感器驱动起来。

| 功能 | 量程 | 精度 |
| ---- | ---- | ---- |
| 温度 | `0℃ - 1024℃` |`±0.25℃`|

## 支持情况

| 包含设备     | 温度 |
| ------------ | ---- |
| **通信接口** |      |
| SPI          | √    |
| **工作模式** |      |
| 轮询         | √    |
| 中断         |      |
| FIFO         |      |

## 使用说明

### 依赖

- RT-Thread 4.0.0+
- Sensor 组件
- SPI 驱动：max6675 设备使用 SPI 进行数据通讯，需要系统 SPI 驱动支持

### 获取软件包

使用 max7675 软件包需要在 RT-Thread 的包管理中选中它，具体路径如下：

```
RT-Thread online packages  --->
  peripheral libraries and drivers  --->
    sensors drivers  --->
          max6675: a package of digital temperature sensor.
               Version (latest)  --->
```

**Version**：软件包版本选择，默认选择最新版本。

### 使用软件包

max6675 软件包初始化函数如下所示：

```c
int rt_hw_max6675_init(const char *name, struct rt_sensor_config *cfg)；
```

该函数需要由用户调用，函数主要完成的功能有，

- 设备配置和初始化（根据传入的配置信息配置接口设备）；
- 注册相应的传感器设备，完成 max6675 设备的注册；

#### 初始化示例

```c
#include "sensor_maxim_max6675.h"

#define MAX667_SPI_BUS  "spi10"

int rt_hw_max6675_port(void)
{
    struct rt_sensor_config cfg;

    cfg.intf.dev_name  = MAX667_SPI_BUS;

    rt_hw_ap3216c_init("max6675", &cfg);

    return RT_EOK;
}
INIT_ENV_EXPORT(rt_hw_max6675_port);
```

## 注意事项

暂无。

## 联系人信息

维护人:

- 维护：[Jonas](https://github.com/Jonas)
- 主页：https://github.com/RT-Thread-packages/max6675