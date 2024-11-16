/**
 * @file    main.ino
 * @author  Filippo Graziani
 * @date    16/11/2024
 * @brief   In this project, we will electronically prepare the attention and
 *          concentration developer Buzz Wire Game with the help of a conductor
 *          wire using the buzzer and LED module with Picobricks. While
 *          preparing this project, you will have learned an input technique
 *          that is not a button but will be used like a button.
 *          https://docs.picobricks.com/en/latest/projects/buzz%20wire%20game.html
 * @attention   The library "ACROBOTIC SSD1306" is needed.
 */

#include <Arduino.h>
#include <stdint.h>
#include <stdlib.h>
#include <Wire.h>
#include <ACROBOTIC_SSD1306.h>
#include "pico/stdlib.h"

#define PICOBRICKS_BTN_PIN          10
#define PICOBRICKS_BUZZER_PIN       20
#define PICOBRICKS_TEST_POINT_PIN   1

#define CHECK_MSEC                  5

static struct repeating_timer g_debounce_timer{0};
static volatile bool gb_btn_press{false};
static volatile bool gb_btn_changed{false};

static bool irq_timer_btn(repeating_timer_t * p_rt);


void
setup ()
{
    Serial.begin(115200);

    while (!Serial)
    {
        /* Do nothing */
    }

    pinMode(PICOBRICKS_BUZZER_PIN, OUTPUT);
    pinMode(PICOBRICKS_BTN_PIN, INPUT_PULLUP);
    pinMode(PICOBRICKS_TEST_POINT_PIN, INPUT_PULLUP);

    add_repeating_timer_ms(CHECK_MSEC, irq_timer_btn, NULL, &g_debounce_timer);

    Wire.begin();
    oled.init();
    oled.clearDisplay();
    oled.setTextXY(0, 0);
    oled.putString("BUZZ WIRE GAME");
    oled.setTextXY(4, 0);
    oled.putString("Press the button");
    oled.setTextXY(5, 0);
    oled.putString("to start!");

    Serial.println("STARTING!");
}   /* setup() */

void
loop ()
{
    if (true == gb_btn_press)
    {
        Serial.println("Button pressed: " + String(gb_btn_press));

        oled.clearDisplay();
        oled.setTextXY(2, 0);
        oled.putString("GAME");
        oled.setTextXY(3, 0);
        oled.putString("STARTED");

        int32_t start_time(millis());
        int32_t current_time{0};

        while (HIGH == digitalRead(PICOBRICKS_TEST_POINT_PIN))
        {
            current_time = millis() - start_time;

            if (current_time >= 5000)
            {
                break;
            }
        }

        oled.clearDisplay();
        oled.setTextXY(2, 0);
        oled.putString("GAME");
        oled.setTextXY(3, 0);
        oled.putString("OVER!");
        oled.setTextXY(4, 0);
        oled.putString("Time: " + String(current_time) + "ms");

        tone(PICOBRICKS_BUZZER_PIN, 500, 300);
        delay(5000);

        oled.clearDisplay();
        oled.setTextXY(0, 0);
        oled.putString("BUZZ WIRE GAME");
        oled.setTextXY(4, 0);
        oled.putString("Press the button");
        oled.setTextXY(5, 0);
        oled.putString("to start!");
    }
}   /* loop() */

static bool
irq_timer_btn (repeating_timer_t * p_rt)
{
    // Stable time before registering pressed.
    //
    const int32_t press_msec(10);

    // Stable time before registering released.
    //
    const int32_t release_msec(100);

    // Counter of interrupt calls.
    //
    static uint8_t count(release_msec / CHECK_MSEC);

    // Debounced state of the key.
    //
    static bool debounced_key_press(false);
    bool key_changed(false);
    bool key_pressed(debounced_key_press);

    bool raw_state = digitalRead(PICOBRICKS_BTN_PIN);

    // If the key has not changed its status.
    //
    if (raw_state == debounced_key_press)
    {
        // Key released (HIGH state).
        //
        if (true == debounced_key_press)
        {
            count = release_msec / CHECK_MSEC;
        }
        // Key pressed (LOW state).
        //
        else
        {
            count = press_msec / CHECK_MSEC;
        }
    }
    // If the key has changed, wait for new state to become stable.
    //
    else
    {
        --count;

        // The wait before elaborating the key is terminated.
        //
        if (0 == count)
        {
            // Timer expired, accept the change.
            //
            debounced_key_press = raw_state;
            key_changed = true;
            key_pressed = debounced_key_press;

            if (true == debounced_key_press)
            {
                // Key released (HIGH state).
                //
                count = release_msec / CHECK_MSEC;
            }
            else
            {
                // Key pressed (LOW state).
                //
                count = press_msec / CHECK_MSEC;
            }
        }
    }

    gb_btn_press = key_pressed;
    gb_btn_changed = key_changed;

    return true;
}   /* irq_timer_btn() */


/*** End of file ***/
