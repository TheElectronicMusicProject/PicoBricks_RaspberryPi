/**
 * @file    main.ino
 * @author  Filippo Graziani
 * @date    04/02/2024
 * @brief   In this project, we will create a timer alarm that adjusts for
 *          daylight using the light sensor in Picobricks. In this project we
 *          will make a simple alarm application. The alarm system we will
 *          design is designed to sound automatically in the morning. For this,
 *          we will use LDR sensor in the project.
 *          https://docs.picobricks.com/en/latest/projects/alarm%20clock.html
 * @attention   The library "ACROBOTIC SSD1306" and "Adafruit NeoPixel" are
 *              needed.
 */

#include <stdint.h>
#include <stdlib.h>
#include <Wire.h>
#include <ACROBOTIC_SSD1306.h>
#include "Adafruit_NeoPixel.h"

#ifdef __AVR__
#   include <avr/power.h>
#endif

#define PICOBRICKS_BTN_PIN          10
#define PICOBRICKS_NEOPIXEL_PIN     6
#define PICOBRICKS_LDR_PIN          27
#define PICOBRICKS_BUZZER_PIN       20
#define NUMLEDS                     1
#define DELAY_PIXEL                 500
#define LIGHT_THRESHOLD             100

static void irq_btn();

static volatile bool gb_timer_stopped(true);

static class Adafruit_NeoPixel g_leds(NUMLEDS, PICOBRICKS_NEOPIXEL_PIN,
                                      NEO_GRB + NEO_KHZ800);

void
setup ()
{
    Serial.begin(115200);

    while (!Serial)
    {
        /* Do nothing */
    }

#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
    clock_prescale_set(clock_div_1);
#endif
    pinMode(PICOBRICKS_LDR_PIN, INPUT);
    pinMode(PICOBRICKS_BUZZER_PIN, OUTPUT);
    pinMode(PICOBRICKS_BTN_PIN, INPUT);
    attachInterrupt(digitalPinToInterrupt(PICOBRICKS_BTN_PIN), irq_btn,
                    FALLING);

    g_leds.begin();
    g_leds.setPixelColor(0, g_leds.Color(0, 0, 0));
    g_leds.show();

    Wire.begin();
    oled.init();
    oled.clearDisplay();
    oled.setTextXY(0, 0);

    Serial.println("STARTING!");
}   /* setup() */

void
loop ()
{
    static bool b_prev_stat = gb_timer_stopped;
    static bool b_debounce(false);
    static uint32_t debounce_time(0);
    static bool b_key_pressed(false);
    int32_t light(analogRead(PICOBRICKS_LDR_PIN));
    static bool b_wakeup(true);
    static int32_t screen_now(0);
    static int32_t screen_prev(screen_now);

    Serial.println("Light is " + String(light));

    if (gb_timer_stopped != b_prev_stat)
    {
        b_wakeup = false;
        Serial.println("Button is " + String(gb_timer_stopped));
        detachInterrupt(digitalPinToInterrupt(PICOBRICKS_BTN_PIN));
        b_prev_stat = gb_timer_stopped;
        b_debounce = true;
        debounce_time = millis();
        b_key_pressed = !b_key_pressed;
    }

    if (light <= LIGHT_THRESHOLD)
    {
        if (true == b_wakeup)
        {
            screen_now = 0;
            oled.setTextXY(0, 0);
            oled.putString("Goodmorning!");
            tone(PICOBRICKS_BUZZER_PIN, 494);
        }
        else
        {
            screen_now = 1;
            noTone(PICOBRICKS_BUZZER_PIN);
            oled.setTextXY(0, 0);
            oled.putString("Have a nice day!");
        }

        g_leds.setPixelColor(0, g_leds.Color(0, 0, 0));
    }
    else
    {
        screen_now = 2;
        noTone(PICOBRICKS_BUZZER_PIN);
        b_wakeup = true;
        oled.setTextXY(0, 0);
        oled.putString("Goodnight <3");
        
        g_leds.setPixelColor(0, g_leds.Color(255, 255, 255));
    }

    if (screen_now != screen_prev)
    {
        screen_prev = screen_now;
        oled.clearDisplay();
        g_leds.show();
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
