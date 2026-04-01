#include <Arduino.h>
#include <Zigbee.h>
#include "MyZigbeeHumiditySensor.hpp"

#define MIN_READ 2700
#define MAX_READ 1000

#define ADC_1 0
#define ADC_2 1
#define ADC_3 2
#define ADC_4 3

#define REPORT_INTERVAL_S (5 * 60)
#define DEBUG_INTERVAL_S 1

auto sensor1 = MyZigbeeHumiditySensor(1);
auto sensor2 = MyZigbeeHumiditySensor(2);
auto sensor3 = MyZigbeeHumiditySensor(3);
auto sensor4 = MyZigbeeHumiditySensor(4);

void onboardLed(uint8_t red_val, uint8_t green_val, uint8_t blue_val)
{
    rgbLedWriteOrdered(RGB_BUILTIN, LED_COLOR_ORDER_RGB, red_val, green_val, blue_val);
}

void readSensor(MyZigbeeHumiditySensor &sensor, const uint8_t adcPin)
{
    analogRead(adcPin);
    auto val = analogRead(adcPin);
    float humidity = roundf((val - MIN_READ) * 100.0f / (MAX_READ - MIN_READ));
    if (humidity < 0)
    {
        humidity = 0;
    }
    if (humidity > 100)
    {
        humidity = 100;
    }
    log_i("Report humidity %d : %f%%, raw value : %d", adcPin, humidity, val);
    sensor.sendHumidity(humidity);
}

void update_sensor_value(void *arg)
{
    int interval = (int)arg;

    for (;;)
    {
        onboardLed(0, 0, 100);

        readSensor(sensor1, ADC_1);
        readSensor(sensor2, ADC_2);
        readSensor(sensor3, ADC_3);
        readSensor(sensor4, ADC_4);

        onboardLed(0, 0, 0);

        vTaskDelay(pdMS_TO_TICKS(interval));
    }
}

void setup()
{
    Serial.begin(115200);

    onboardLed(255, 0, 0);

    sensor1.init();
    sensor2.init();
    sensor3.init();
    sensor4.init();

    Zigbee.addEndpoint(&sensor1);
    Zigbee.addEndpoint(&sensor2);
    Zigbee.addEndpoint(&sensor3);
    Zigbee.addEndpoint(&sensor4);

    // Connect to network
    if (!Zigbee.begin())
    {
        log_e("Zigbee failed to start!");
        delay(1000);
        esp_restart();
    }
    else
    {
        log_i("Zigbee started successfully!");
    }

    log_i("Connecting to network ");
    while (!Zigbee.connected())
    {
        Serial.print(".");
        delay(100);
    }
    Serial.println();
    log_i("Ready");

    onboardLed(0, 100, 0);

    // Delay to allow establishing proper connection with coordinator
    delay(1000);

    onboardLed(0, 0, 0);

    int interval = (Serial ? DEBUG_INTERVAL_S : REPORT_INTERVAL_S) * 1000;
    xTaskCreate(update_sensor_value, "update_sensor_value", 2048, (void *)interval, 10, NULL);
}

void loop()
{
}
