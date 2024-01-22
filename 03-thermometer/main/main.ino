/**
 * @file    main.ino
 * @author  Filippo Graziani
 * @date    22/01/2024
 * @brief   In this project, you will prepare a thermometer with Picobricks that
 *          will display the ambient temperature on the OLED screen.
 *          Picobricks has a DHT11 module. This module can sense the temperature
 *          and humidity in the environment and send data to the
 *          microcontroller. In this project, we will write the necessary codes
 *          to print the temperature values measured by the DHT11 temperature
 *          and humidity sensor on the OLED screen.
 *          https://docs.picobricks.com/en/latest/projects/thermometer.html
 * @attention   The library "ACROBOTIC SSD1306" and "DHT sensor library" are
 *              needed.
 */

#include <Arduino.h>
#include <stdint.h>
#include <Wire.h>
#include <DHT.h>
#include <ACROBOTIC_SSD1306.h>

#define PICOBRICKS_PIN_DHT11    11

static DHT g_dht_val(PICOBRICKS_PIN_DHT11, DHT11);

void
setup ()
{
    g_dht_val.begin();

    // To set I2C it will uses the default GP4 (SDA) and GP5 (SCL).
    //
    Wire.begin();
    oled.init();
    oled.clearDisplay();

    oled.setTextXY(0, 0);
    oled.putString("Temperature and");
    oled.setTextXY(1, 0);
    oled.putString("humidity");
}   /* setup() */

void
loop ()
{
    float temp = g_dht_val.readTemperature();
    float humid = g_dht_val.readHumidity();

    String str_temp_hum = String(temp) + String("\t") + String(humid);
    oled.setTextXY(4, 3);
    oled.putString(str_temp_hum);

    delay(500);
}   /* loop() */


/*** End of file ***/
