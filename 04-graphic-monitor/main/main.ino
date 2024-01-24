/**
 * @file    main.ino
 * @author  Filippo Graziani
 * @date    22/01/2024
 * @brief   In this project, we will prepare a project in which we increase or
 *          decrease the brightness of the red LED with a potentiometer.
 *          When the picobricks starts, the potentiometer value will be read
 *          continuously and the brightness value of the LED will be adjusted.
 *          Applications in which the effect of the electrical signal is reduced
 *          by changing the frequency is called PWM. We will send the analog
 *          values we read from the potentiometer as PWM signals to the red LED
 *          and we will be able to adjust the illumination intensity.
 *          https://docs.picobricks.com/en/latest/projects/graphic%20monitor.html
 */

#include <Arduino.h>
#include <stdint.h>

#define PICOBRICKS_PIN_LED  7
#define PICOBRICKS_PIN_POT  26

void
setup ()
{
    pinMode(PICOBRICKS_PIN_LED, OUTPUT);
    pinMode(PICOBRICKS_PIN_POT, INPUT);

    Serial.begin(9600);
}   /* setup() */

void
loop ()
{
    String out_str = "POT = ";
    int32_t pot_read = analogRead(PICOBRICKS_PIN_POT);
    analogWrite(PICOBRICKS_PIN_LED, map(pot_read, 0, 1023, 0, 255));

    out_str += String(pot_read);
    Serial.println(out_str);
    delay(500);
}   /* loop() */


/*** End of file ***/
