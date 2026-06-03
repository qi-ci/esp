环境监测仪app：
1. 无需登录，仅个人家庭办公室使用
2. 一共三个传感器，温湿度传感器htu21d/CO2传感器s8/HCHO传感器ze08，
3. 三个页面，
4. 一个页面dashboard_page，共四个框，
    一个框显示时间、国家、城市，
    三个框为传感器数据框，框标题为三个传感器名字，每个框内有传感器对应返回的数据、上次更新时间，传感器更新按钮，
    固定温度、湿度、CO2、HCHO单位分别为°C、%RH、ppm、mg/m3
5. 一个页面history，可下拉菜单选择温度、湿度、CO2、HCHO，然后选择按照年月日时查询历史记录，列出所涉及数据，并画出所选区域趋势图，
4. 一个页面settings，共五个框：
    一个框读取esp32 MQTT Broker地址并显示，
    一个框可设置esp32连接app的mqtt的 wifi和密码，
    一个框设置传感器是否启用自动刷新，如果启用，设置每个传感器的刷新间隔时间，单位s，
    一个框设置时区、国家、城市，全部下拉菜单选择，
    一个框设置app使用什么语言，下拉菜单选择语言




一个页面dashboard_page，共五个框，
    一个框显示时间、国家、城市
    设备状态
    ```
    Device Online

    WiFi: Connected
    MQTT: Connected
    Uptime: 12h 35m
    FW: v1.0.3
    ```
    三个框为传感器数据框，框标题为三个传感器名字，每个框内有传感器对应返回的数据、上次更新时间，传感器更新按钮，
    固定温度、湿度、CO2、HCHO单位分别为°C、%RH、ppm、mg/m3
一个页面history，
    指标：
    □温度
    □湿度
    □CO2
    □HCHO

    时间范围：
    ○24小时
    ○7天
    ○30天
    ○自定义
一个页面settings，共三个框：
    ① 设备设置
    设备ID

    MQTT Broker

    WiFi配置

    重启设备

    恢复出厂设置
    ② 数据设置
    自动刷新

    刷新间隔

    历史保留天数
    ③ 地区设置
    国家

    城市

    时区

    语言
一个页面Device Setup Page:
首次启动APP

↓
发现未绑定设备

↓
进入设备配网页

↓
BLE连接ESP32

↓
配置：
SSID
Password
MQTT Broker

↓
ESP联网

↓
MQTT上线

↓
Dashboard