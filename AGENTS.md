# AGENTS.md - STM32 Watch 项目 AI 编码助手指南

## 项目概述

基于 STM32F103C8T6 + FreeRTOS + u8g2 的智能手表固件项目。
使用 CMake + Ninja 构建，目标架构为 ARM Cortex-M3。

---

## 目录结构速查

| 路径 | 内容 |
|------|------|
| Core/Src/main.c | 主程序入口，main() 函数，外设初始化与内核启动 |
| Core/Src/freertos.c | FreeRTOS 任务创建与配置 (MX_FREERTOS_Init) |
| Core/Inc/main.h | 引脚宏定义 (OLED_Pin, DHT11_data_Pin 等) |
| Task/ | 用户 FreeRTOS 任务源文件 |
| Lib/ | 板级外设驱动（Buzzer, DHT11, IR_rec, RGB） |
| u8g2/ | u8g2 图形库（不可修改） |
| Drivers/ | STM32 HAL 与 CMSIS 库（不可修改） |
| Middlewares/ | FreeRTOS 内核源码（不可修改） |

---

## 代码规范

### 通用规则

- 使用 /* USER CODE BEGIN/END */ 标记保护代码区域，必须在标记区域内编写用户代码
- 函数命名：大驼峰式（ShowClockTask, MX_GPIO_Init）
- 宏定义：全大写加下划线（LED_ON, OLED_Pin）
- 注释：使用 /* */ 风格块注释，保持与 STM32 HAL 一致的风格
- 缩进：2 空格

### 文件修改规则

- Core/Inc/main.h：仅修改 /* USER CODE BEGIN Private defines */ 区域
- Core/Src/main.c：仅修改 /* USER CODE BEGIN */ 区域
- Core/Src/freertos.c：添加新任务时，在 MX_FREERTOS_Init 的 USER CODE 区域创建
- Task/ 与 Lib/：项目自有代码，可按需修改

### 禁止修改

- Drivers/ 下的所有 HAL/CMSIS 驱动文件
- Middlewares/ 下的 FreeRTOS 内核源码
- u8g2/ 下的图形库文件

---

## 构建命令

`powershell
# 完整构建
cd D:\STM\Watch\build
ninja

# 或使用 CMake
cmake --build D:\STM\Watch\build
`

---

## FreeRTOS 任务开发注意事项

1. 任务栈大小：在 freertos.c 中的 osThreadNew 调用中指定
2. 任务优先级：合理安排优先级，避免低优先级任务饿死
3. 共享资源：访问全局数据（如 MyData.c）时，使用队列或信号量保护
4. 阻塞调用：任务循环中使用 osDelay() 而非 HAL_Delay()，避免阻塞调度器
5. DHT11 时序：DHT11 驱动 (Lib/DHT11/) 涉及微秒级延时，注意中断对此的影响

---

## 外设驱动添加规范

1. 新外设驱动放在 Lib/<DeviceName>/ 目录下
2. 每个驱动包含 .c 和 .h 文件
3. 初始化函数在 Core/Src/main.c 的 /* USER CODE BEGIN 2 */ 中调用
4. 如需要 FreeRTOS 任务，在 Core/Src/freertos.c 的 MX_FREERTOS_Init 中创建
5. 引脚定义在 Core/Inc/main.h 的 /* USER CODE BEGIN Private defines */ 中声明

---

## 文件管理安全规则

- **禁止批量删除文件或目录**
- 删除文件时，一次只能删除一个明确路径的文件
- 不要使用 del /s, rd /s, rmdir /s, Remove-Item -Recurse, rm -rf
- 需要批量删除时，应停止操作并向用户请求
