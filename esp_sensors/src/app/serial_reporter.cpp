#include <Arduino.h>

extern float temperature;
extern float humidity;
extern int co2ppm;
extern float hcho;

void SerialReporter_begin()
{
    Serial.println("Reporter Ready");
}

void SerialReporter_print()
{
    Serial.println("\n==============================");

    Serial.printf("温度 : %.2f °C\n", temperature);
    Serial.printf("湿度 : %.2f %%\n", humidity);

    if (co2ppm >= 0)
        Serial.printf("CO2  : %d ppm\n", co2ppm);
    else
        Serial.println("CO2  : NO DATA");

    if (hcho >= 0)
        Serial.printf("甲醛 : %.3f mg/m³\n", hcho);
    else
        Serial.println("甲醛 : NO DATA");

    Serial.println("==============================");
}