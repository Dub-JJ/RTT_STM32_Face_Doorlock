# RTT_STM32_Face_Doorlock
基于 RT-Thread 的 STM32F103ZET6 门禁：K210 人脸识别 + MLX90614 测温 + SU-03T 语音 + OLED 显示 + SG90 舵机开关门
## 功能特性
- K210 端：人脸检测/识别、按指令录入/删除、识别结果上报
- STM32 端：按键录入/删除控制、接收识别结果、测温判定、舵机开门/关门、蜂鸣器报警、OLED 显示、语音播报
- 语音播报：录入成功 / 识别成功 / 删除成功（温度异常语音预留协议）

## 硬件清单
- 主控：STM32F103ZET6
- AI 模块：K210（Maix 系列开发板）
- 语音模块：机芯智能 SU-03T（同系列 SU-11T / SU-13T）
- 执行器：SG90 舵机、喇叭
- 显示：0.96 寸 OLED（IIC 四针）
- 传感：红外温度传感器（GY906 / MLX90614）
- 工具：ST-Link 烧写器、USB 转 TTL（用于 SU-03T 配置/升级）

## 串口分工
- UART1：日志打印（RT-Thread Console）
- UART2：STM32 <-> K210 通讯
- UART3：STM32 -> SU-03T 语音模块

## 引脚连接（按当前工程配置）
- 按键（下拉触发，中断下降沿）：
  - KEY0：PE2（人脸录入）
  - KEY1：PE3（人脸删除）
- 蜂鸣器：PB8
- 舵机 PWM：TIM3_CH4 -> PB1（pwm3 channel 4）
- OLED I2C（软件 I2C）：PB6(SCL) / PB7(SDA)
- 温度传感器（软件 I2C，与 OLED 共线）：PB6(SCL) / PB7(SDA)
- UART2（K210）：PA2(TX) / PA3(RX)
- UART3（语音）：PB10(TX) / PB11(RX)

## 运行流程（简述）
1. 上电初始化 OLED、温度传感器、按键中断、UART2/UART3、PWM
2. 正常循环：
   - 读取人体温度并刷新 OLED
   - 温度 > 37℃：蜂鸣器报警
   - 若收到 K210 “识别成功”上报，且温度 < 37℃：舵机开门；延时约 7 秒后自动关门复位
3. 按键：
   - KEY0(PE2)：发送“录入”指令给 K210
   - KEY1(PE3)：发送“删除”指令给 K210
4. 语音：
   - K210 上报录入/识别/删除成功后，STM32 转发对应语音播报指令给 SU-03T

## 通信协议

### K210 -> MCU（UART2，上报）
- 人脸录入成功：`AF 05 00 FA`
- 识别成功：`AF 02 <编号> FA`
- 删除成功：`AF 01 00 FA`

### MCU -> K210（UART2，下发）
- 删除：`A1 01 02 1A`
- 录入：`A1 02 00 1A`

### MCU -> 语音模块（UART3，下发）
- 人脸录入成功：`AA 55 01 55 AA`
- 识别成功：`AA 55 02 55 AA`
- 删除成功：`AA 55 03 55 AA`
- 温度异常（预留）：`AA 55 04 55 AA`

## SU-03T 固件与升级说明（务必阅读）
1. 本固件只适用于机芯智能：SU-03T / SU-11T / SU-13T
2. 本仓库不建议直接提交厂商固件文件（可能涉及授权/版权），请从厂商 SDK/资料包获取
3. `jx_su_03t_release.bin`（烧录器版本固件）：只能用烧录器烧录
4. `jx_su_03t_release_update.bin`（串口升级固件）：只能通过串口进行升级烧入
5. 串口消息固定格式（厂商说明）：
   - `0xAA 0x55 ... 0x55 0xAA`
   - `msgno` 为消息编号（1 开始）；本工程当前使用无参数帧：`AA 55 msgno 55 AA`

## K210 端部署（MaixPy / kflash_gui）
1. 安装 MaixPy IDE 和 kflash_gui
2. 使用 kflash_gui 烧录“获取机器码”的固件，进入 IDE 读取并保存机器码
3. 下载人脸模型（人脸检测 / 五点关键点 / 196 维特征模型）：
   - https://maixhub.com/model/zoo
4. 使用 kflash_gui 依次烧录：
   - K210 固件
   - 三个人脸模型文件（按下载包顺序）
5. 回到 IDE 运行人脸示例/你的 K210 程序，确认能输出协议帧（以 `AF` 开头，以 `FA` 结束）

## STM32 端编译与下载
- 使用 RT-Thread Studio 导入工程后编译
- 使用 ST-Link 下载到 STM32F103ZET6 运行
- 运行时可通过 UART1 查看日志

## 目录结构（简述）
- `applications/`：业务逻辑（按键、串口协议、OLED、测温、PWM/舵机）
- `drivers/`：STM32 外设驱动适配
- `rt-thread/`：RT-Thread 内核与组件
- `packages/`：第三方包（如 u8g2）

## 许可证与声明
- RT-Thread、第三方组件遵循各自开源协议
- K210 模型、SU-03T 固件等二进制资源请按来源协议使用与分发
