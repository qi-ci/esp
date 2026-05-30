#include <Arduino.h>
#include <Wire.h>
#include <HTU21D.h>

HTU21D htu;

// ================= UART =================
HardwareSerial s8Serial(1);
HardwareSerial ze08Serial(2);

// ================= 数据 =================
float temperature = NAN;
float humidity = NAN;

int co2ppm = -1;
float hcho = -1;

// ================= CRC =================
uint16_t modbusCRC(uint8_t *buf, uint8_t len)
{
    uint16_t crc = 0xFFFF;

    for (uint8_t pos = 0; pos < len; pos++)
    {
        crc ^= (uint16_t)buf[pos];

        for (int i = 8; i != 0; i--)
        {
            if (crc & 0x0001)
            {
                crc >>= 1;
                crc ^= 0xA001;
            }
            else
            {
                crc >>= 1;
            }
        }
    }

    return crc;
}

// ================= HTU21D =================
void readHTU21D()
{
    temperature = htu.readTemperature();
    humidity = htu.readHumidity();
}

// ================= ZE08 =================
void readZE08()
{
    while (ze08Serial.available() >= 9)
    {
        uint8_t buf[9];
        ze08Serial.readBytes(buf, 9);

        if (buf[0] == 0xFF)
        {
            uint16_t value = ((uint16_t)buf[4] << 8) | buf[5];
            hcho = value / 1000.0;
        }
    }
}

// ================= S8 CO2 =================
void readS8()
{
    uint8_t cmd[8] =
    {
        0xFE, 0x04, 0x00, 0x03,
        0x00, 0x01, 0x00, 0x00
    };

    uint16_t crc = modbusCRC(cmd, 6);
    cmd[6] = crc & 0xFF;
    cmd[7] = crc >> 8;

    while (s8Serial.available())
        s8Serial.read();

    s8Serial.write(cmd, 8);

    delay(120);

    if (s8Serial.available() >= 7)
    {
        uint8_t buf[7];
        s8Serial.readBytes(buf, 7);

        co2ppm = ((uint16_t)buf[3] << 8) | buf[4];
    }
    else
    {
        co2ppm = -1;
    }
}

// ================= SETUP =================
void setup()
{
    Serial.begin(115200);
    delay(2000);

    Serial.println("\n=== AIR QUALITY MONITOR START ===");

    Wire.begin(21, 22);

    if (htu.begin())
        Serial.println("HTU21D OK");
    else
        Serial.println("HTU21D FAIL");

    // S8: RX=4 TX=5
    s8Serial.begin(9600, SERIAL_8N1, 4, 5);

    // ZE08: RX=16 TX=17
    ze08Serial.begin(9600, SERIAL_8N1, 16, 17);
}

// ================= LOOP =================
void loop()
{
    readHTU21D();
    readZE08();
    readS8();

    Serial.println("\n==============================");

    // 温湿度
    if (!isnan(temperature))
        Serial.printf("温度 : %.2f °C\n", temperature);
    else
        Serial.println("温度 : NO DATA");

    if (!isnan(humidity))
        Serial.printf("湿度 : %.2f %%\n", humidity);
    else
        Serial.println("湿度 : NO DATA");

    // CO2
    if (co2ppm >= 0)
        Serial.printf("CO2  : %d ppm\n", co2ppm);
    else
        Serial.println("CO2  : NO DATA");

    // 甲醛
    if (hcho >= 0)
        Serial.printf("甲醛 : %.3f mg/m³\n", hcho);
    else
        Serial.println("甲醛 : NO DATA");


    Serial.println("==============================");

    delay(5000);
}