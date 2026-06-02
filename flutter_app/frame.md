# 🧠 二、Flutter App端（完整IoT控制台设计）

---

## 🎯 App目标

```text id="app_goal"
家庭空气监测控制中心
```

特点：

* 无登录
* 多设备支持
* 实时数据 + 历史数据
* 地理+时间上下文
* 手动刷新控制

---

# 🧱 1️⃣ Flutter App 分层架构设计

```text id="flutter_tree"
lib/
├── core/                         # 🧠 核心基础层
│   ├── app_config.dart         # 时区/主题/device设置
│   ├── mqtt_config.dart        # broker配置
│   ├── device_context.dart     # 当前设备信息
│
├── network/                      # 🌐 通信层
│   ├── mqtt_service.dart       # MQTT连接/订阅
│   ├── mqtt_topics.dart        # ⭐必须与ESP32一致
│   ├── message_router.dart     # 消息分发
│
├── models/                      # 📦 数据模型层
│   ├── sensor_data.dart        # 温湿度/CO2/HCHO
│   ├── device_model.dart       # device_id
│   ├── command_model.dart      # cmd结构
│
├── data/                        # 💾 数据层
│   ├── local_db.dart           # SQLite/Hive
│   ├── history_repository.dart # 历史数据管理
│   ├── cache_manager.dart      # 实时缓存
│
├── logic/                       # 🧠 业务逻辑层
│   ├── sensor_controller.dart  # 数据控制
│   ├── command_controller.dart  # 命令发送
│   ├── history_controller.dart # 历史查询
│
├── ui/                          # 📱 UI层
│   ├── dashboard/              # 实时数据页
│   ├── control/                # 手动控制页
│   ├── history/                # 历史记录页
│   ├── settings/               # 时区/地点设置
│
└── main.dart
```

---

# 🧠 2️⃣ Flutter关键模块设计说明

---

## 🌐 ① MQTT Service（核心通信）

作用：

* 连接ESP32
* 订阅 tele/cmd/resp
* 转发给逻辑层

---

## 📦 ② SensorData Model

统一数据结构：

* temperature
* humidity
* co2
* hcho
* timestamp

---

## 💾 ③ History Repository（关键）

实现：

```text id="history"
按日存储
按月索引
时间范围查询
```

存储方式：

* SQLite（推荐）
* 或 Hive（轻量）

---

## 📍 ④ Device Context（新增关键）

管理：

* 当前设备ID
* 经纬度
* 城市国家
* 时区

---

## ⏰ ⑤ 时间系统（关键）

支持：

* UTC
* 本地时区切换
* UI展示时间格式化

---

## 🧠 ⑥ Command Controller

控制ESP32：

* refresh temp
* refresh co2
* reboot device

---

# 🧠 3️⃣ Flutter UI结构（对应你需求）

---

## 📊 Dashboard（首页）

```text id="ui1"
温度卡片
湿度卡片
CO2卡片
甲醛卡片
更新时间
```

---

## 🎛 Control Page（控制页）

```text id="ui2"
[刷新温度]
[刷新CO2]
[刷新甲醛]
[刷新全部]
```

---

## 📍 Settings（环境信息）

```text id="ui3"
国家
城市
经纬度
时区选择
```

---

## 📅 History（历史数据）

```text id="ui4"
按日查看
按月查看
折线图
导出数据
```

---

# 🧠 4️⃣ Flutter数据流

---

## 🔁 实时流

```text id="flowA"
ESP32 MQTT tele
 ↓
mqtt_service
 ↓
sensor_controller
 ↓
UI刷新
```

---

## 🔁 命令流

```text id="flowB"
UI按钮
 ↓
command_controller
 ↓
MQTT cmd
 ↓
ESP32
 ↓
resp
 ↓
UI更新
```

---

## 🔁 历史流

```text id="flowC"
MQTT数据
 ↓
local_db
 ↓
history_controller
 ↓
history UI
```

---

# 🧠 三、整体系统最终形态

---

## 🌐 全系统架构

```text id="final_arch"
        Flutter App
 ┌────────┼────────┐
实时UI   控制    历史
   ↓        ↓       ↓
        MQTT Broker
             ↓
           ESP32
     ┌──────┼──────┐
   Sensors  Core  Network
```

---

# 🧠 一句话总结

```text id="summary"
ESP32负责“采集 + 执行”，
Flutter负责“控制 + 可视化 + 历史分析”，
MQTT负责“连接整个IoT系统的神经网络”。