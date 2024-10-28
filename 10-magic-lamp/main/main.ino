/**
 * @file    main.ino
 * @author  Filippo Graziani
 * @date    28/10/2024
 * @brief   In this project, we will turn a lamp we use at home on and off with
 *          sound. In our project, which we will build using the Picobricks
 *          sound level sensor, we will perform the on-off operations by making
 *          a clap sound. As in the previous projects, in projects where sensors
 *          are used, before we start to write the codes, it will make your
 *          progress easier to see what values the sensor sends in the
 *          operations we want to do by just operating the sensor, and then
 *          writing the codes of the project based on these values.
 *          https://docs.picobricks.com/en/latest/projects/magic%20lamp.html
 */

#include <stdint.h>
#include <stdlib.h>

#define PICOBRICKS_MIC_PIN_D        4
#define PICOBRICKS_RELAY_PIN        12
#define SOUND_THRESHOLD             0

void
setup ()
{
    Serial.begin(115200);

    while (!Serial)
    {
        /* Do nothing */
    }

    pinMode(PICOBRICKS_MIC_PIN_D, INPUT);
    pinMode(PICOBRICKS_RELAY_PIN, OUTPUT);

    Serial.println("STARTING!");
}   /* setup() */

void
loop ()
{
    static bool b_pin_toggle = false;
    static int prev_time = millis();

    if ((HIGH == digitalRead(PICOBRICKS_MIC_PIN_D)) &&
        ((millis() - prev_time) > 500))
    {
        b_pin_toggle ^= 1;
        Serial.println(b_pin_toggle);
        digitalWrite(PICOBRICKS_RELAY_PIN, b_pin_toggle);
        prev_time = millis();
    }
}   /* loop() */


/*** End of file ***/
