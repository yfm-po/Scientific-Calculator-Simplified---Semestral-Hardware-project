#include <Keypad.h>
#include "lcd.h"
#include "Calculate.h"
#include "SyntaxErrorHandler.h"
#define MAX_EXPR_LEN 16

#define CHANGE_MODE 3
#define CHANGE_MODE_LED 12
#define LEFT_PARENTHESES_LED 2
#define RIGHT_PARENTHESES_LED 13

int leds[] = { LEFT_PARENTHESES_LED, RIGHT_PARENTHESES_LED };

const byte ROWS = 4;
const byte COLS = 4;

char keys[ROWS][COLS] = {
    {'1', '2', '3', '+'},
    {'4', '5', '6', '-'},
    {'7', '8', '9', '*'},
    {'=', '0', '/', 'C'}
};

byte rowPins[ROWS] = {11, 10, 9, 8};
byte colPins[COLS] = {7, 6, 5, 4};

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

//TODO: add buzzer, add easteregg, add more components
void setup() 
{
    lcd_init();

    pinMode(CHANGE_MODE, INPUT);
    pinMode(CHANGE_MODE_LED, OUTPUT);
    pinMode(LEFT_PARENTHESES_LED, OUTPUT);
    pinMode(RIGHT_PARENTHESES_LED, OUTPUT);

    lcd_print_at(0, 0, "Simple but cool");
    lcd_print_at(1, 0, "Calculator");
    delay(2000);
    lcd_clear();
    lcd_print_at(0, 0, "Enter expression");
    delay(2000);
    lcd_clear();
}

void loop() 
{
    char *expression = (char *) calloc(MAX_EXPR_LEN, sizeof(char));
    char *result = (char *) calloc(MAX_EXPR_LEN, sizeof(char));
    char key = keypad.getKey();

    int i = 0;
    while (key != '=' && i < MAX_EXPR_LEN) 
    {
        if (digitalRead(CHANGE_MODE) == HIGH) 
        {
            digitalWrite(CHANGE_MODE_LED, HIGH);
            if (key == '+')
            {
                expression[i] = '(';
                i++;
                lcd_print_at(0, 0, expression);
                blinkWithLed(leds, 0);
                key = NO_KEY;
            } 
            else if (key == '-') 
            {
                expression[i] = ')';
                i++;
                lcd_print_at(0, 0, expression);
                blinkWithLed(leds, 1);
                key = NO_KEY;
            }
            else if (key == 'C')
            {
                for (int j = 0; j < MAX_EXPR_LEN; j++)expression[j] = '\0';
                i = 0;
                lcd_clear();
                blinkWithLeds(leds, 2);
            }
            digitalWrite(CHANGE_MODE_LED, LOW);
        }
        if (key == 'C') 
        {
            if (i > 0) 
            {
                i--;
                expression[i] = '\0';
                lcd_print_at(0, i, " ");
            }
        } 
        else if (key != NO_KEY && key != 'C') 
        {
            expression[i] = key;
            i++;
            lcd_print_at(0, 0, expression);
        }
        key = keypad.getKey();
    }

    float resultNumber = Evaulate(expression);

    dtostrf(resultNumber, 0, 2, result);

    if (hasSyntaxError(expression))
    {
        syntaxError();
    }
    else if (strcmp(expression, "") || strcmp(expression, NULL))
    {
        lcd_print_at(1, 0, "=");
        lcd_print_at(1, 1, result);
        do{ key = keypad.getKey(); } while (key != 'C');
    }
    
    free(expression);
    free(result);

    lcd_clear();
}

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