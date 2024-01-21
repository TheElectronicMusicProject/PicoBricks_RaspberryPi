/**
 * @file    main.ino
 * @author  Filippo Graziani
 * @date    21/01/2024
 * @brief   The first step in physical computing is the Blink application:
 *          controlling the on/off status of a LED. By simply connecting an LED
 *          to the microprocessor board, the code keeps the LED blinking
 *          continuously.
 *          https://docs.picobricks.com/en/latest/projects/blink.html
 */

#include <Arduino.h>
#define PICOBRICKS_LED_PIN      7

void
setup ()
{
    pinMode(PICOBRICKS_LED_PIN, OUTPUT);
}   /* setup() */

void
loop ()
{
    static bool b_led_status = true;

    digitalWrite(PICOBRICKS_LED_PIN, b_led_status);
    b_led_status = !b_led_status;
    delay(500);
}   /* loop() */


/*** End of file ***/
