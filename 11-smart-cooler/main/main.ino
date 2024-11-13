/**
 * @file    main.ino
 * @author  Filippo Graziani
 * @date    14/11/2024
 * @brief   In our project, we will firstly display the temperature values
 *          measured by the DHT11 temperature and humidity sensor on Picobricks.
 *          Then, we will define a temperature limit and write the necessary
 *          codes for the DC motor connected to Picobricks to start rotating
 *          when the temperature value from the DHT11 module reaches this limit,
 *          and for the DC motor to stop when the temperature value falls below
 *          the limit we have determined.
 *          https://docs.picobricks.com/en/latest/projects/smart%20cooler.html
 * @attention   The library "DHT sensor library" is needed.
 * @attention   WARNING! The motor driver of the Picobricks is directly
 *              connected to the power supply of the microcontroller. Best
 *              choice is to use an external power supply for the motor VCC.
 *              Specs of the motor driver can be found here:
 *              https://mm.digikey.com/Volume0/opasdata/d220001/medias/docus/5042/L9110S.pdf
 */

#include <Arduino.h>
#include <stdint.h>
#include <stdlib.h>
#include <Wire.h>
#include <DHT.h>

#define PICOBRICKS_PIN_DHT11    11
#define PICOBRICKS_PIN_MOTOR    21
#define TEMP_LIMIT              20

static DHT g_dht_val(PICOBRICKS_PIN_DHT11, DHT11);

void
setup ()
{
    Serial.begin(115200);

    while (!Serial)
    {
        /* Do nothing */
    }

    g_dht_val.begin();

    pinMode(PICOBRICKS_PIN_DHT11, OUTPUT);
    pinMode(PICOBRICKS_PIN_MOTOR, OUTPUT);

    Serial.println("STARTING!");
}   /* setup() */

void
loop ()
{
    float temp_read(g_dht_val.readTemperature());
    Serial.println("Temperature" + String(temp_read));

    if (temp_read > TEMP_LIMIT)
    {
        digitalWrite(PICOBRICKS_PIN_MOTOR, HIGH);
    }
    else
    {
        digitalWrite(PICOBRICKS_PIN_MOTOR, LOW);
    }

    delay(100);
}   /* loop() */


/*** End of file ***/
