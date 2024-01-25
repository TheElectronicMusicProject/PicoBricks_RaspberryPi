/**
 * @file    main.ino
 * @author  Filippo Graziani
 * @date    25/01/2024
 * @brief   In this project, you will make your own time measuring device using
 *          Picobricks, OLED display, button and potentiometer modules. A Timer…
 *          https://docs.picobricks.com/en/latest/projects/my%20timer.html
 * @attention   The library "ACROBOTIC SSD1306" is needed.
 */

#include <stdint.h>
#include <stdlib.h>
#include <Wire.h>
#include "ACROBOTIC_SSD1306.h"

#define PICOBRICKS_BTN_PIN      10
#define PICOBRICKS_POT_PIN      26

static void irq_btn();

static volatile bool gb_timer_stopped(true);

void
setup ()
{
    Serial.begin(115200);

    while (!Serial)
    {
        /* Do nothing */
    }

    pinMode(PICOBRICKS_POT_PIN, INPUT);
    pinMode(PICOBRICKS_BTN_PIN, INPUT);
    attachInterrupt(digitalPinToInterrupt(PICOBRICKS_BTN_PIN), irq_btn,
                    FALLING);

    Wire.begin();
    oled.init();
    oled.clearDisplay();

    Serial.println("STARTING!");
}   /* setup() */

void
loop ()
{
    static bool b_prev_stat = gb_timer_stopped;
    static bool b_debounce(false);
    static uint32_t debounce_time(0);
    static bool b_key_pressed(false);
    static uint32_t prev_val(analogRead(PICOBRICKS_POT_PIN));
    static uint32_t minutes_timer(0);
    static uint32_t prev_time(0);

    if (gb_timer_stopped != b_prev_stat)
    {
        Serial.println("Button is " + String(gb_timer_stopped));
        detachInterrupt(digitalPinToInterrupt(PICOBRICKS_BTN_PIN));
        b_prev_stat = gb_timer_stopped;
        b_debounce = true;
        debounce_time = millis();
        b_key_pressed = !b_key_pressed;
        oled.clearDisplay();
    }

    if (true == gb_timer_stopped)
    {
        uint32_t val = analogRead(PICOBRICKS_POT_PIN);
        minutes_timer = val * 60 / 1023;

        if (val != prev_val)
        {
            oled.setTextXY(0, 0);

            if (minutes_timer < 9)
            {
                oled.putString("0" + String(minutes_timer));
            }
            else
            {
                oled.putString(String(minutes_timer));
            }

            oled.setTextXY(0, 5);
            oled.putString("min.");
        }

        oled.setTextXY(7, 4);
        oled.putString("Set timer");
        prev_time = millis();
    }
    else
    {
        int32_t min = (minutes_timer - 1) -
                      (((millis() - prev_time) % 360000) / 60000);
        int32_t sec = 59 - (((millis() - prev_time) % 60000) / 1000);
        int32_t mil = 999 - ((millis() - prev_time) % 1000);

        if (0 != minutes_timer)
        {
            if (min <= 9)
            {
                oled.setTextXY(0, 0);
                if (min < 0)
                {
                    oled.putString("00");
                }
                else
                {
                    oled.putString("0" + String(min));
                }
                
                oled.setTextXY(0, 5);
                oled.putString("min.");
            }
            else
            {
                oled.setTextXY(0, 0);
                oled.putString(String(min));
                oled.setTextXY(0, 5);
                oled.putString("min.");
            }

            if (sec <= 9)
            {
                oled.setTextXY(1, 0);
                oled.putString("0" + String(sec));
                oled.setTextXY(1, 5);
                oled.putString("sec.");
            }
            else
            {
                oled.setTextXY(1, 0);
                oled.putString(String(sec));
                oled.setTextXY(1, 5);
                oled.putString("sec.");
            }

            if (mil <= 9)
            {
                oled.setTextXY(2, 0);
                oled.putString("00" + String(mil));
                oled.setTextXY(2, 5);
                oled.putString("millis.");
            }
            else if (mil <= 99)
            {
                oled.setTextXY(2, 0);
                oled.putString("0" + String(mil));
                oled.setTextXY(2, 5);
                oled.putString("millis.");
            }
            else
            {
                oled.setTextXY(2, 0);
                oled.putString(String(mil));
                oled.setTextXY(2, 5);
                oled.putString("millis.");
            }
        }

        if ((0 == minutes_timer) || (0 > min))
        {
            minutes_timer = 0;
            oled.setTextXY(0, 0);
            oled.putString("- Finished -");
            oled.setTextXY(1, 0);
            oled.putString("            ");
            oled.setTextXY(2, 0);
            oled.putString("            ");
        }
    }

    if ((true == b_debounce) && ((millis() - debounce_time) > 500))
    {
        b_debounce = false;
        attachInterrupt(digitalPinToInterrupt(PICOBRICKS_BTN_PIN), irq_btn,
                        FALLING);
    }
}   /* loop() */

static void
irq_btn ()
{
    gb_timer_stopped = !gb_timer_stopped;
}   /* irq_btn() */


/*** End of file ***/
