#include "ui.h"

void blinkWithLed(int *leds, int ledIndex)
{
    digitalWrite(leds[ledIndex], HIGH);
    delay(500);
    digitalWrite(leds[ledIndex], LOW);
}

void blinkWithLeds(int *leds, int ledsCount)
{
    for (int i = 0; i < ledsCount; i++) digitalWrite(leds[i], HIGH);
    delay(500);
    for (int i = 0; i < ledsCount; i++) digitalWrite(leds[i], LOW);
}

void waitForKeypress(char key, Keypad keypad)
{
    key = keypad.getKey(); 
    while (key != 'C') key = keypad.getKey();
}

void loadingScreen()
{
    lcd_print_at(0, 0, "Turning on");
    animateLoadingDots(0, 9);
    
    delay(500);
    lcd_clear();

    lcd_print_at(1, 0, "Plase wait");        
    animateLoadingDots(1, 9);

    lcd_clear();
    lcd_print_at(0, 0, "Enter expression");
    delay(1500);
    lcd_clear();
}

void animateLoadingDots(int row, int startIndex)
{
    for (int i = 0; i < 3; i++)
    {
        for (int i = 1; i <= 3; i++)
        {
            lcd_print_at(row, startIndex + i, ".");
            tone(BUZZER, 5000, 100);
            delay(500);
        }

        for (int i = 1; i <= 3; i++)
        {
            lcd_print_at(row, startIndex + i, " ");
            tone(BUZZER, 5000, 100);
        }
    }
    lcd_print_at(row, startIndex + 1, "...");
}

void showHistoryElementBasedOnIndex(char **expressionHistory, char **resultHistory, int index)
{
    lcd_print_at(0, 0, expressionHistory[index]);
    if (!hasSyntaxError(expressionHistory[index]))
    {
        lcd_print_at(1, 0, "=");
        lcd_print_at(1, 1, resultHistory[index]);
    }
    else
    {
        lcd_print_at(1, 0, resultHistory[index]);
    }
}