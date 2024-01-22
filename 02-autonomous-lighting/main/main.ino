/**
 * @file    main.ino
 * @author  Filippo Graziani
 * @date    21/01/2024
 * @brief   In this project, with PicoBricks, we will enable the LED to turn on
 *          when the amount of light decreases in order to understand the
 *          working systems of the systems where the lighting is turned on
 *          automatically when it gets dark.
 *          https://docs.picobricks.com/en/latest/projects/autonomous-lighting.html
 * @attention   The library "Adafruit NeoPixel" is needed.
 */

#include <Arduino.h>
#include <stdint.h>
#include <string>
#include <Adafruit_NeoPixel.h>

#ifdef __AVR__
#   include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

#define PICOBRICKS_NEOPIXEL_PIN     6
#define PICOBRICKS_LDR_PIN          27
#define NUMLEDS                     1
#define DELAY_PIXEL                 500

static class Adafruit_NeoPixel g_leds(NUMLEDS, PICOBRICKS_NEOPIXEL_PIN,
                                      NEO_GRB + NEO_KHZ800);
static const int32_t g_threshold(200);

void
setup ()
{
#if 0
    Serial.begin(115200);

    while (!Serial)
    {
        /* Do nothing */
    }
#endif
    pinMode(PICOBRICKS_NEOPIXEL_PIN, OUTPUT);
    pinMode(PICOBRICKS_LDR_PIN, INPUT);
    analogReadResolution(12);

    g_leds.begin();
}   /* setup() */

void
loop ()
{
    int32_t read_ldr = analogRead(PICOBRICKS_LDR_PIN);
    g_leds.clear();
#if 0
    String print_string = "Value of LDR is " + read_ldr;

    Serial.println(print_string);
#endif

    for (uint32_t idx = 0; idx < NUMLEDS; ++idx)
    {
        if (g_threshold < read_ldr)
        {
            g_leds.setPixelColor(idx, g_leds.Color(0, 255, 0));
            g_leds.show();
            delay(DELAY_PIXEL);
        }
        else if ((g_threshold > read_ldr) && ((g_threshold / 2) < read_ldr))
        {
            g_leds.setPixelColor(idx, g_leds.Color(0, 0, 255));
            g_leds.show();
            delay(DELAY_PIXEL);
        }
        else
        {
            g_leds.setPixelColor(idx, g_leds.Color(255, 0, 0));
            g_leds.show();
            delay(DELAY_PIXEL);
        }
    }
}   /* loop() */


/*** End of file ***/
