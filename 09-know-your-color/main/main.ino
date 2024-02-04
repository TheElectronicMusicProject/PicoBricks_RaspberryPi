/**
 * @file    main.ino
 * @author  Filippo Graziani
 * @date    04/02/2024
 * @brief   In this project, you will learn about the randomness used in every
 *          programming language. We will prepare a enjoyable game with the RGB
 *          LED, OLED screen and button module of Picobricks. The game we will
 *          build in the project will be built on the user knowing the colors
 *          correctly or incorrectly.
 *          https://docs.picobricks.com/en/latest/projects/know%20your%20color.html
 * @attention   The library "ACROBOTIC SSD1306" and "Adafruit NeoPixel" are
 *              needed.
 */

#include <stdint.h>
#include <stdlib.h>
#include <Wire.h>
#include <ACROBOTIC_SSD1306.h>
#include "Adafruit_NeoPixel.h"

#define PICOBRICKS_BTN_PIN          10
#define PICOBRICKS_NEOPIXEL_PIN     6
#define NUMLEDS                     1
#define DELAY_PIXEL                 500
#define LIGHT_THRESHOLD             100

static void irq_btn();
static void random_color(int32_t& oled, int32_t& rgb);

static volatile bool gb_btn_clicked(false);

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

    pinMode(PICOBRICKS_BTN_PIN, INPUT);
    attachInterrupt(digitalPinToInterrupt(PICOBRICKS_BTN_PIN), irq_btn,
                    FALLING);

    g_leds.begin();
    g_leds.setPixelColor(0, g_leds.Color(0, 0, 0));
    g_leds.show();

    Wire.begin();
    oled.init();
    oled.clearDisplay();

    randomSeed(analogRead(27));

    Serial.println("STARTING!");
}   /* setup() */

void
loop ()
{
    static bool b_prev_stat = gb_btn_clicked;
    static bool b_debounce(false);
    static uint32_t debounce_time(0);
    static int32_t screen_now(0);
    static int32_t screen_prev(screen_now);
    static int32_t oled_color(0);
    static int32_t neopixel_color(0);
    static int32_t status(0);
    static int32_t counter(0);
    static int64_t prev_time(0);
    static int32_t points(0);

    if (gb_btn_clicked != b_prev_stat)
    {
        Serial.println("Button is " + String(gb_btn_clicked));
        detachInterrupt(digitalPinToInterrupt(PICOBRICKS_BTN_PIN));
        b_prev_stat = gb_btn_clicked;
        b_debounce = true;
        debounce_time = millis();
    }

    switch (status)
    {
        case 0:
            points = 0;
            counter = 0;

            oled.clearDisplay();
            oled.setTextXY(3, 1);
            oled.putString("The game begins");

            g_leds.setPixelColor(0, g_leds.Color(0, 0, 0));
            g_leds.show();

            delay(2000);
            oled.clearDisplay();

            status = 1;
        break;

        case 1:
            if (counter < 10)
            {
                random_color(oled_color, neopixel_color);
                g_leds.show();

                status = 2;
            }
            else
            {
                oled.clearDisplay();
                status = 5;
            }

            prev_time = millis();

            ++counter;
        break;

        case 2:
            if (true == gb_btn_clicked)
            {
                status = 3;
                gb_btn_clicked = false;
            }

            if ((millis() - prev_time) > 2000)
            {
                status = 4;
            }

            if (2 != status)
            {
                oled.clearDisplay();
                g_leds.setPixelColor(0, g_leds.Color(0, 0, 0));
                g_leds.show();
            }
        break;

        case 3:
            if (oled_color == neopixel_color)
            {
                points += 10;
            }
            else
            {
                points -= 10;
            }

            status = 1;
        break;

        case 4:
            if (oled_color != neopixel_color)
            {
                points += 10;
            }
            else
            {
                points -= 10;
            }

            status = 1;
        break;

        case 5:
            if (millis() - prev_time < 10000)
            {
                oled.setTextXY(2, 5);
                oled.putString("Score: ");
                oled.setTextXY(4, 7);
                oled.putString(String(points));
                oled.setTextXY(6, 5);
                oled.putString(" points");
            }
            else
            {
                status = 0;
            }
        break;

        default:
        break;
    }


    if (screen_now != screen_prev)
    {
        screen_prev = screen_now;
        oled.clearDisplay();
        g_leds.show();
    }

    if ((true == b_debounce) && ((millis() - debounce_time) > 200))
    {
        b_debounce = false;
        attachInterrupt(digitalPinToInterrupt(PICOBRICKS_BTN_PIN), irq_btn,
                        FALLING);
    }
}   /* loop() */

static void
random_color (int32_t& oled_color , int32_t& rgb_color)
{
    oled_color = random(1, 5);
    rgb_color  = random(1, 5);

    switch (oled_color)
    {
        case 1:
            oled.setTextXY(3, 7);
            oled.putString("red");
        break;

        case 2:
            oled.setTextXY(3, 6);
            oled.putString("green");
        break;

        case 3:
            oled.setTextXY(3, 6);
            oled.putString("blue");
        break;

        case 4:
            oled.setTextXY(3, 6);
            oled.putString("white");
        break;

        default:
        break;
    }

    switch (rgb_color)
    {
        case 1:
            g_leds.setPixelColor(0, g_leds.Color(255, 0, 0));
        break;

        case 2:
            g_leds.setPixelColor(0, g_leds.Color(0, 255, 0));
        break;

        case 3:
            g_leds.setPixelColor(0, g_leds.Color(0, 0, 255));
        break;

        case 4:
            g_leds.setPixelColor(0, g_leds.Color(255, 255, 255));
        break;

        default:
        break;
    }
}   /* random_color() */

static void
irq_btn ()
{
    gb_btn_clicked = true;
}   /* irq_btn() */


/*** End of file ***/
