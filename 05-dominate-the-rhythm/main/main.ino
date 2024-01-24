/**
 * @file    main.ino
 * @author  Filippo Graziani
 * @date    24/01/2024
 * @brief   We will use the button-LED module to start the song, the
 *          potentiometer module to adjust the speed of the song, and the buzzer
 *          module to play the notes. Potentiometer is analog input module.
 *          https://docs.picobricks.com/en/latest/projects/dominate%20the%20rhythm.html
 * @attention   The library "ACROBOTIC SSD1306" is needed.
 */

#include <Arduino.h>
#include <stdint.h>
#include <Wire.h>
#include "ACROBOTIC_SSD1306.h"

#define PICOBRICKS_BUZZER_PIN   20
#define PICOBRICKS_POT_PIN      26
#define PICOBRICKS_BTN_PIN      10

typedef enum
{
    MELODY_DO = 262,
    MELODY_RE = 294,
    MELODY_MI = 330,
    MELODY_FA = 349,
    MELODY_SOL = 392,
    MELODY_LA = 440,
    MELODY_SI = 494,
} melody_t;

static void irq_btn();

static volatile bool gb_start_melody(false);

void
setup ()
{
    Serial.begin(115200);

    while (!Serial)
    {
        /* Do nothing */
    }

    pinMode(PICOBRICKS_BUZZER_PIN, OUTPUT);
    pinMode(PICOBRICKS_POT_PIN, INPUT);
    pinMode(PICOBRICKS_BTN_PIN, INPUT);
    attachInterrupt(digitalPinToInterrupt(PICOBRICKS_BTN_PIN), irq_btn,
                                          FALLING);

    Wire.begin();
    oled.init();
    oled.clearDisplay();
    oled.setTextXY(1, 0);
    oled.putString("Melody stopped!");

    Serial.println("STARTING!");
}   /* setup() */

void
loop ()
{
    static bool b_prev_start(gb_start_melody);
    static bool b_debounce(false);
    static uint32_t time_irq(0);
    static uint32_t status = 0;
    int32_t pot_val = analogRead(PICOBRICKS_POT_PIN);
    int32_t speed = pot_val / 128;

    if (b_prev_start != gb_start_melody)
    {
        detachInterrupt(digitalPinToInterrupt(PICOBRICKS_BTN_PIN));
        time_irq = millis();
        oled.clearDisplay();
        oled.setTextXY(1, 0);
        b_prev_start = gb_start_melody;
        b_debounce = true;
        Serial.println("Status changed in " + String(gb_start_melody));

        if (true == gb_start_melody)
        {
            oled.putString("Playing ...");
        }
        else
        {
            oled.putString("Melody stopped!");
            status = 0;
            noTone(PICOBRICKS_BUZZER_PIN);
        }
    }

    if (true == gb_start_melody)
    {
        oled.setTextXY(2, 0);
        oled.putString("Speed " + String(speed));

        switch (status)
        {
            case 0:
                tone(PICOBRICKS_BUZZER_PIN, MELODY_LA);
                delay(1000 / (speed + 1));
                status = 1;
            break;

            case 1:
                tone(PICOBRICKS_BUZZER_PIN, MELODY_MI);
                delay(500 / (speed + 1));
                status = 2;
            break;

            case 2:
                tone(PICOBRICKS_BUZZER_PIN, MELODY_MI);
                delay(500 / (speed + 1));
                status = 3;
            break;

            case 3:
                tone(PICOBRICKS_BUZZER_PIN, MELODY_MI);
                delay(500 / (speed + 1));
                status = 4;
            break;

            case 4:
                tone(PICOBRICKS_BUZZER_PIN, MELODY_MI);
                delay(500 / (speed + 1));
                status = 5;
            break;

            case 5:
                tone(PICOBRICKS_BUZZER_PIN, MELODY_MI);
                delay(500 / (speed + 1));
                status = 6;
            break;

            case 6:
                tone(PICOBRICKS_BUZZER_PIN, MELODY_MI);
                delay(500 / (speed + 1));
                status = 7;
            break;

            case 7:
                tone(PICOBRICKS_BUZZER_PIN, MELODY_FA);
                delay(500 / (speed + 1));
                status = 8;
            break;

            case 8:
                tone(PICOBRICKS_BUZZER_PIN, MELODY_MI);
                delay(500 / (speed + 1));
                status = 9;
            break;

            case 9:
                tone(PICOBRICKS_BUZZER_PIN, MELODY_RE);
                delay(500 / (speed + 1));
                status = 10;
            break;

            case 10:
                tone(PICOBRICKS_BUZZER_PIN, MELODY_FA);
                delay(500 / (speed + 1));
                status = 11;
            break;

            case 11:
                tone(PICOBRICKS_BUZZER_PIN, MELODY_MI);
                delay(1000 / (speed + 1));
                status = 0;
            break;

            default:
                status = 0;
            break;
        }
    }

    // Wait 500ms before enabling again the interrupt (debouncing).
    //
    if (((millis() - time_irq) > 500) && (true == b_debounce))
    {
        b_debounce = false;
        Serial.println("Interrupt enabled");
        attachInterrupt(digitalPinToInterrupt(PICOBRICKS_BTN_PIN), irq_btn,
                                              FALLING);
    }
}   /* loop() */

static void
irq_btn ()
{
    gb_start_melody = !gb_start_melody;
}   /* irq_btn() */


/*** End of file ***/
