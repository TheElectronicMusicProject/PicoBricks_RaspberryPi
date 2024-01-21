/**
 * @file    main.ino
 * @author  Filippo Graziani
 * @date    21/01/2024
 * @brief   In the project, we will understand the pressing status by checking
 *          whether the button conducts current or not. If it is pressed, it
 *          will light the LED, if it is not pressed, we will turn off the LED.
 *          https://docs.picobricks.com/en/latest/projects/action-reaction.html
 * @attention   The button on pin 10 has a 0 logic value by default.
 */

#include <Arduino.h>
#define PICOBRICKS_LED_PIN      7
#define PICOBRICKS_BTN_PIN      10

static void irq_btn();

static volatile bool gb_btn_pressed = false;

void
setup ()
{
    pinMode(PICOBRICKS_LED_PIN, OUTPUT);
    attachInterrupt(digitalPinToInterrupt(PICOBRICKS_BTN_PIN), irq_btn, CHANGE);
}   /* setup() */

void
loop ()
{
    static bool b_led_status = false;

    b_led_status = gb_btn_pressed == true ? true : false;
    digitalWrite(PICOBRICKS_LED_PIN, b_led_status);
}   /* loop() */

static void
irq_btn ()
{
    gb_btn_pressed = !gb_btn_pressed;
}   /* irq_btn() */


/*** End of file ***/
