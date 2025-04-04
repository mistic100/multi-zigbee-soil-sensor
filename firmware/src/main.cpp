#include <Arduino.h>
#include <Zigbee.h>
#include "MyZigbeeHumiditySensor.hpp"

#define MIN_READ 2800
#define MAX_READ 1100

#define ADC_1 0
#define ADC_2 1
#define ADC_3 2
#define ADC_4 3

#define TIME_TO_SLEEP_S 55

auto sensor1 = MyZigbeeHumiditySensor(10);
auto sensor2 = MyZigbeeHumiditySensor(11);
auto sensor3 = MyZigbeeHumiditySensor(12);
auto sensor4 = MyZigbeeHumiditySensor(13);

void onboardLed(uint8_t red_val, uint8_t green_val, uint8_t blue_val)
{
    rgbLedWriteOrdered(RGB_BUILTIN, LED_COLOR_ORDER_RGB, red_val, green_val, blue_val);
}

void readSensor(MyZigbeeHumiditySensor &sensor, const uint8_t adcPin)
{
    analogRead(adcPin);
    auto val = analogRead(adcPin);
    float humidity = roundf((val - MIN_READ) * 100.0f / (MAX_READ - MIN_READ));
    if (humidity < 0) {
        humidity = 0;
    }
    if (humidity > 100) {
        humidity = 100;
    }
    Serial.printf("Report humidity %d : %f%%\n", adcPin, humidity);
    sensor.sendHumidity(humidity);
}

/*
static void temp_sensor_value_update(void *arg) {
    for (;;) {
        onboardLed(0, 0, 255);

        readSensor(sensor1, ADC_1);
        readSensor(sensor2, ADC_2);
        readSensor(sensor3, ADC_3);
        readSensor(sensor4, ADC_4);

        onboardLed(0, 0, 0);

        delay(TIME_TO_SLEEP_S * 1000);
    }
}
*/

void setup()
{
    Serial.begin(115200);

    onboardLed(255, 0, 0);

    esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP_S * 1000000ULL);

    sensor1.init();
    sensor2.init();
    sensor3.init();
    sensor4.init();

    Zigbee.addEndpoint(&sensor1);
    Zigbee.addEndpoint(&sensor2);
    Zigbee.addEndpoint(&sensor3);
    Zigbee.addEndpoint(&sensor4);

    if (!Zigbee.begin())
    {
        Serial.println("Zigbee failed to start!");
        delay(1000);
        esp_restart();
    }
    else
    {
        Serial.println("Zigbee started successfully!");
    }

    Serial.print("Connecting to network ");
    while (!Zigbee.connected())
    {
        Serial.print(".");
        delay(100);
    }
    Serial.println();
    Serial.println("Ready");

    onboardLed(0, 0, 255);

    // Delay to allow establishing proper connection with coordinator
    delay(1000);

    readSensor(sensor1, ADC_1);
    readSensor(sensor2, ADC_2);
    readSensor(sensor3, ADC_3);
    readSensor(sensor4, ADC_4);

    onboardLed(0, 255, 0);

    // Delay to allow the data to be sent before going to sleep
    delay(1000);

    onboardLed(0, 0, 0);

    Serial.println("Going to sleep now");
    esp_deep_sleep_start();

    // xTaskCreate(temp_sensor_value_update, "temp_sensor_update", 2048, NULL, 10, NULL);
}

void loop()
{
}
