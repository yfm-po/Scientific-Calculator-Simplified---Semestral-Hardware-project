#include "ui.h"

//funkcia na blinknutie LED
void blinkWithLed(int *leds, int ledIndex)
{
    digitalWrite(leds[ledIndex], HIGH);
    delay(500);
    digitalWrite(leds[ledIndex], LOW);
}

//funkcia na blinknutie s viacerimi LEDmi
void blinkWithLeds(int *leds, int ledsCount)
{
    for (int i = 0; i < ledsCount; i++) digitalWrite(leds[i], HIGH);
    delay(500);
    for (int i = 0; i < ledsCount; i++) digitalWrite(leds[i], LOW);
}

//funkcia ktora caka na key 'C'[Clear]
void waitForKeypress(char key, Keypad keypad)
{
    key = keypad.getKey(); 
    while (key != 'C') key = keypad.getKey();
}

//funkcia na vypis loadingu
void loadingScreen()
{
    lcd_print_at(0, 0, "Turning on");
    animateLoadingDots(0, 9);
    
    delay(200);
    lcd_clear();

    lcd_print_at(1, 0, "Plase wait");        
    animateLoadingDots(1, 9);

    lcd_clear();
    lcd_print_at(0, 0, "Enter expression");
    delay(500);
    lcd_clear();
}

//funkcia na animaciu bodiek po loadingu
void animateLoadingDots(int row, int startIndex)
{
    for (int i = 0; i < 3; i++)
    {
        for (int i = 1; i <= 3; i++)
        {
            lcd_print_at(row, startIndex + i, ".");
            tone(BUZZER, 5000, 100);
            delay(200);
        }

        for (int i = 1; i <= 3; i++)
        {
            lcd_print_at(row, startIndex + i, " ");
            tone(BUZZER, 5000, 100);
        }
    }
    lcd_print_at(row, startIndex + 1, "...");
}

//funkcia na vypis vyrazu a vysledku z historie podla indexu
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

//funkcia na blinknutie LED pri vypise vysledku
void FinisherBlinking(int *leds, int ledsCount)
{
    for (int i = 0; i < ledsCount; i++)
    {
        digitalWrite(leds[i], HIGH);
        tone(BUZZER, 5000, 100);
        delay(100);
        digitalWrite(leds[i], LOW);
        delay(100);
    }

    for (int i = ledsCount - 1; i >= 0; i--)
    {
        digitalWrite(leds[i], HIGH);
        tone(BUZZER, 5000, 100);
        delay(100);
        digitalWrite(leds[i], LOW);
        delay(100);
    }
}

//funkcia na blinknutie LED v pripade syntax erroru
void syntaxErrorBlinking(int *leds, int ledsCount)
{
    for (int i = 0; i < ledsCount; i++)
    {
        digitalWrite(leds[i], HIGH);
        tone(BUZZER, 5000, 100);
        delay(500);
        digitalWrite(leds[i], LOW);
        delay(500);
    }

    for (int i = ledsCount - 1; i >= 0; i--)
    {
        digitalWrite(leds[i], HIGH);
        tone(BUZZER, 5000, 100);
        delay(500);
        digitalWrite(leds[i], LOW);
        delay(500);
    }
}