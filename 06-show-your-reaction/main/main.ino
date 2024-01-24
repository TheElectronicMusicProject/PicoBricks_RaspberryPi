/**
 * @file    main.ino
 * @author  Filippo Graziani
 * @date    24/01/2024
 * @brief   In this project you will learn about the randomness used in every
 *          programming language. With Picobricks, we will develop an electronic
 *          system using OLED display, Button-LED and Buzzer module. A timer
 *          starts running as soon as the Picobricks are turned on. With this
 *          timer, we can measure 1 thousandth of a second.
 *          https://docs.picobricks.com/en/latest/projects/show%20your%20reaction.html
 * @attention   The library "ACROBOTIC SSD1306" is needed.
 */

#include <stdint.h>
#include <stdlib.h>
#include <Wire.h>
#include "ACROBOTIC_SSD1306.h"

#define PICOBRICKS_BUZZER_PIN   20
#define PICOBRICKS_LED_PIN      7
#define PICOBRICKS_BTN_PIN      10

enum
{
    MELODY_DO = 262,
    MELODY_RE = 294,
    MELODY_MI = 330,
    MELODY_FA = 349,
    MELODY_SOL = 392,
    MELODY_LA = 440,
    MELODY_SI = 494,
};

void
setup ()
{
    Serial.begin(115200);

    while (!Serial)
    {
        /* Do nothing */
    }

    pinMode(PICOBRICKS_BUZZER_PIN, OUTPUT);
    pinMode(PICOBRICKS_LED_PIN, OUTPUT);
    pinMode(PICOBRICKS_BTN_PIN, INPUT);

    srand(analogRead(A1));

    Wire.begin();
    oled.init();
    oled.clearDisplay();

    Serial.println("STARTING!");
}   /* setup() */

void
loop ()
{
    uint32_t diff_time(0);
    uint32_t prev_time(0);
    oled.setTextXY(3, 0);
    oled.putString("Press the button");
    oled.setTextXY(5, 4);
    oled.putString("TO START");
    digitalWrite(PICOBRICKS_LED_PIN, LOW);

    if (1 == digitalRead(PICOBRICKS_BTN_PIN))
    {
        while (1 == digitalRead(PICOBRICKS_BTN_PIN))
        {
            delay(10);
        }

        for (int32_t cnt = 3; cnt > 0; --cnt)
        {
            oled.clearDisplay();
            oled.setTextXY(4, 8);
            oled.putString(String(cnt));
            delay(1000);
        }

        oled.clearDisplay();
        oled.setTextXY(4, 6);
        oled.putString("GO!!!");

        uint32_t random_pause = rand() % 8000 + 2000;
        delay(random_pause);
        digitalWrite(PICOBRICKS_LED_PIN, HIGH);

        prev_time = millis();

        while (0 == digitalRead(PICOBRICKS_BTN_PIN))
        {
            diff_time = millis() - prev_time;
        }

        tone(PICOBRICKS_BUZZER_PIN, MELODY_LA);
        delay(200);
        noTone(PICOBRICKS_BUZZER_PIN);

        oled.clearDisplay();
        oled.setTextXY(1, 4);
        oled.putString("Press the");

        oled.setTextXY(2, 3);
        oled.putString("RESET BUTTON");

        oled.setTextXY(3, 3);
        oled.putString("To Repeat!");

        oled.setTextXY(6, 3);
        oled.putString("Score: ");
        oled.setTextXY(6, 9);
        oled.putString(String(diff_time));
        oled.setTextXY(6, 13);
        oled.putString(" ms");

        Serial.println(diff_time);

        delay(10000);
        oled.clearDisplay();
    }
}   /* loop() */


/*** End of file ***/
