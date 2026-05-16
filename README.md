# STM32 智能手表项目 (STM32 Smart Watch)

基于 **STM32F103C8T6** 微控制器的多功能智能手表，集成 **FreeRTOS** 实时操作系统与 **u8g2** 图形库，支持时钟显示、温湿度监测、遥控操作等功能。

---

## 功能特性

- 🕐 **数字时钟** — 实时显示时间（时:分:秒），支持日期与星期显示
- 🌡️ **温湿度监测** — 通过 DHT11 传感器采集环境温湿度
- 🎛️ **菜单界面** — 基于 u8g2 + mui 的多级菜单系统，支持红外遥控操作
- 🎵 **蜂鸣器** — 支持闹钟/提醒发声
- 💡 **RGB LED** — 多彩指示灯，用于状态指示
- ⏱️ **计时器** — 倒计时功能
- 📅 **日历显示** — 月历视图
- 🔄 **多任务并发** — FreeRTOS 实时调度，任务间协同工作

---

## 硬件架构

### 主控芯片

| 型号 | 核心 | 主频 | Flash | SRAM |
|------|------|------|-------|------|
| STM32F103C8T6 | ARM Cortex-M3 | 72 MHz | 64 KB | 20 KB |

### 外设引脚映射

| 外设 | 通信协议 | 引脚 | 说明 |
|------|---------|------|------|
| OLED (SSD1306) | I2C1 | PB6 (SCL), PB7 (SDA) | 128x64 像素 |
| OLED 背光 | GPIO | PC13 | 低电平点亮 (LED_ON) |
| DHT11 | 单总线 | PA1 | 温湿度传感器 |
| RGB-R | GPIO | PA2 | 红色通道 |
| RGB-B | GPIO | PA15 | 蓝色通道 |
| RGB-G | GPIO | PB3 | 绿色通道 |
| 红外接收 | GPIO | - | NEC 协议解码 |
| 蜂鸣器 | GPIO | - | PWM/电平驱动 |
| IWDG | 内部 | - | 独立看门狗（当前注释） |

> 具体引脚定义详见 Core/Inc/main.h 中的 Private defines 部分。

---
## 软件架构

### 技术栈

| 组件 | 说明 |
|------|------|
| **FreeRTOS** (CMSIS-RTOS v2) | 实时操作系统内核 |
| **STM32 HAL** | STM32 硬件抽象层 |
| **u8g2** | 单色图形库，驱动 SSD1306 OLED |
| **mui** (uUI) | 基于 u8g2 的微用户界面框架 |
| **CMake + Ninja** | 构建系统 |

### FreeRTOS 任务划分

| 任务 | 源文件 | 功能描述 |
|------|--------|---------|
| ShowClockTask | Task/ShowClockTask.c | 实时时钟显示（时:分:秒） |
| ShowCalendar | Task/ShowCalendar.c | 日历/日期界面 |
| ShowDHT11Task | Task/ShowDHT11Task.c | DHT11 温湿度数据采集与显示 |
| ShowRGBTask | Task/ShowRGBTask.c | RGB LED 灯光控制 |
| ShowTimerTask | Task/ShowTimerTask.c | 倒计时计时器 |
| ShowmeuTask | Task/ShowmeuTask.c | 主菜单 UI 交互 |

### 目录结构

`
D:\STM\Watch/
├── Core/                      # 核心代码（STM32CubeMX 生成）
│   ├── Inc/                   #   头文件
│   │   ├── main.h             #   主头文件（引脚定义等）
│   │   ├── gpio.h / i2c.h / tim.h / iwdg.h
│   │   ├── stm32f1xx_hal_conf.h
│   │   └── stm32f1xx_it.h
│   └── Src/                   #   源文件
│       ├── main.c             #   主程序入口
│       ├── freertos.c         #   FreeRTOS 配置与任务创建
│       ├── gpio.c / i2c.c / tim.c / iwdg.c
│       └── stm32f1xx_it.c / syscalls.c / sysmem.c
├── Drivers/                   # 外设驱动库
│   ├── CMSIS/                 #   ARM CMSIS 内核库
│   └── STM32F1xx_HAL_Driver/ #   STM32 HAL 驱动
├── Middlewares/               # 中间件
│   └── Third_Party/FreeRTOS/ #   FreeRTOS 源码
├── Task/                      # 用户任务（应用层）
│   ├── ShowClockTask.c/h      #   时钟任务
│   ├── ShowCalendar.c/h       #   日历任务
│   ├── ShowDHT11Task.c/h      #   温湿度任务
│   ├── ShowRGBTask.c/h        #   RGB 任务
│   ├── ShowTimerTask.c/h      #   计时器任务
│   └── ShowmeuTask.c/h        #   菜单任务
├── Lib/                       # 板级外设驱动
│   ├── Buzzer/                #   蜂鸣器驱动
│   ├── DHT11/                 #   DHT11 传感器驱动
│   ├── IR_rec/                #   红外接收解码
│   └── RGB/                   #   RGB LED 驱动
├── u8g2/                      # u8g2 图形库源码
├── cmake/                     # CMake 辅助脚本
├── build/                     # 构建输出目录（已 gitignore）
├── CMakeLists.txt             # 顶层 CMake 构建文件
├── CMakePresets.json          # CMake 预设配置
├── STM32F103C8TX_FLASH.ld     # 链接脚本
└── Watch.ioc                  # STM32CubeMX 项目文件
`

---
## 开发环境搭建

### 方式一：STM32CubeIDE（推荐）

1. 安装 [STM32CubeIDE](https://www.st.com/en/development-tools/stm32cubeide.html)
2. 打开 Watch.ioc -> 生成代码 -> 构建

### 方式二：VSCode + CMake + ARM GCC

#### 前置依赖

- [ARM GCC 工具链](https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm) (arm-none-eabi-gcc)
- [CMake](https://cmake.org/) >= 3.20
- [Ninja](https://ninja-build.org/) 构建系统
- [STM32CubeMX](https://www.st.com/en/development-tools/stm32cubemx.html)（用于生成 HAL 初始化代码）

#### 构建步骤

`ash
# 1. 进入项目目录
cd D:\STM\Watch

# 2. 配置 CMake（使用 Ninja）
cmake -S . -B build -G Ninja

# 3. 构建
cmake --build build

# 4. 生成的可执行文件
build/Watch.elf
`

> 已预置 build/ 目录和 build.ninja 文件，可直接在 build/ 目录下运行 ninja 构建。

### 烧录

- **ST-Link**: openocd -f interface/stlink.cfg -f target/stm32f1x.cfg -c "program build/Watch.elf verify reset exit"
- **STM32CubeProgrammer**: 通过 GUI 选择 Watch.elf 烧录

---

## 许可证

本项目包含以下第三方组件，请遵守各自的许可证条款：

| 组件 | 许可证 | 文件位置 |
|------|--------|---------|
| STM32 HAL 驱动 | BSD-3-Clause | Drivers/STM32F1xx_HAL_Driver/ |
| CMSIS | Apache-2.0 / BSD | Drivers/CMSIS/ |
| FreeRTOS | MIT | Middlewares/Third_Party/FreeRTOS/ |
| u8g2 | BSD-2-Clause | u8g2/ |
| 用户代码 | - | Core/, Task/, Lib/ |
