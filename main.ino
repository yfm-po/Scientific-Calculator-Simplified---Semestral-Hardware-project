#include <Keypad.h>
#include "lcd.h"
#include "Calculate.h"
#include "SyntaxErrorHandler.h"
#define MAX_EXPR_LEN 16
#define HISTORY_SIZE 10

#define CHANGE_MODE 3
#define CHANGE_MODE_LED 12
#define LEFT_PARENTHESES_LED 2
#define RIGHT_PARENTHESES_LED 13

#define TOINT(char) (char - '0')
#define CURSOR_POINTER "^"

int historyIndex;
char** expressionHistory = calloc(HISTORY_SIZE, sizeof(char*));
char** resultHistory = calloc(HISTORY_SIZE, sizeof(char*));

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

    historyIndex = 0;
    for (int i = 0; i < HISTORY_SIZE; i++)
    {
      expressionHistory[i] = calloc(MAX_EXPR_LEN, sizeof(char));
      resultHistory[i] = calloc(MAX_EXPR_LEN, sizeof(char));
    }
    
    lcd_print_at(0, 0, "Simple but cool");
    lcd_print_at(1, 0, "Calculator");
    delay(2000);
    lcd_clear();
    lcd_print_at(0, 0, "Enter expression");
    delay(2000);
    lcd_clear();
    lcd_print_at(1, 0, CURSOR_POINTER);
}

void loop() 
{
    char *expression = (char *) calloc(MAX_EXPR_LEN, sizeof(char));
    char *result = (char *) calloc(MAX_EXPR_LEN, sizeof(char));
    char* historyIndexChar = calloc(1, sizeof(char));
    
    char key = keypad.getKey();
    int i = 0;
    while (key != '=' && i < MAX_EXPR_LEN)
    {
        if (digitalRead(CHANGE_MODE) == HIGH) 
        {
            digitalWrite(CHANGE_MODE_LED, HIGH);
            if (key == '/')
            {
                while (digitalRead(CHANGE_MODE) == HIGH && historyIndex > 0)
                {
                    lcd_clear();
                    if (historyIndex == 1)
                    {
                        lcd_clear();
                        lcd_print_at(0, 0, expressionHistory[0]);
                        lcd_print_at(1, 0, "=");
                        lcd_print_at(1, 1, resultHistory[0]);
                        
                        key = NO_KEY;
                        while (key != 'C')
                        {
                            key = keypad.getKey();
                        }
                        lcd_clear();
                    }
                    else
                    {
                        lcd_print_at(0, 0, "Enter index");
                        lcd_print_at(1, 0, "between 0 and");
                        lcd_print_at(1, 14, itoa(historyIndex - 1, historyIndexChar, 10));
    
                        key = NO_KEY;
                        while ((atoi(key) < 0 || atoi(key) > historyIndex - 1) || key == NO_KEY)
                        {
                            key = keypad.getKey();
                        }
                        
                        lcd_clear();
                        lcd_print_at(0, 0, expressionHistory[TOINT(key)]);
                        lcd_print_at(1, 0, "=");
                        lcd_print_at(1, 1, resultHistory[TOINT(key)]);
                        
                        key = NO_KEY;
                        while (key != 'C')
                        {
                            key = keypad.getKey();
                        }
                        lcd_clear();
                    }
                }
            }
            else if (key == '+')
            {
                expression[i] = '(';
                i++;
                lcd_print_at(1, 0, "                ");
                lcd_print_at(1, i, CURSOR_POINTER);
                lcd_print_at(0, 0, expression);
                blinkWithLed(leds, 0);
                key = NO_KEY;
            } 
            else if (key == '-') 
            {
                expression[i] = ')';
                i++;
                lcd_print_at(1, 0, "                ");
                lcd_print_at(1, i, CURSOR_POINTER);
                lcd_print_at(0, 0, expression);
                blinkWithLed(leds, 1);
                key = NO_KEY;
            }
            else if (key == 'C')
            {
                for (int j = 0; j < MAX_EXPR_LEN; j++) expression[j] = '\0';
                i = 0;
                lcd_clear();
                lcd_print_at(1, i, CURSOR_POINTER);
                blinkWithLeds(leds, 2);
            }
            digitalWrite(CHANGE_MODE_LED, LOW);
        }
        else if (key == 'C') 
        {
            if (i > 0) 
            {
                i--;
                lcd_print_at(1, 0, "                ");
                lcd_print_at(1, i, CURSOR_POINTER);
                expression[i] = '\0';
                lcd_print_at(0, i, " ");
            }
        } 
        else if (key != NO_KEY && key != 'C')
        {
            expression[i] = key;
            i++;
            lcd_print_at(1, 0, "                ");
            lcd_print_at(1, i, CURSOR_POINTER);
            lcd_print_at(0, 0, expression);
        }
        key = keypad.getKey();
    }

    float resultNumber = Evaulate(expression);

    dtostrf(resultNumber, 0, 2, result);

    strcpy(expressionHistory[historyIndex], expression);
    strcpy(resultHistory[historyIndex], result);
    historyIndex++;
    
    if (hasSyntaxError(expression))
    {
        syntaxError();
        waitForKeypress(key);
        lcd_clear();
        return;
    }
    else if (strcmp(expression, "") || strcmp(expression, NULL))
    {
        lcd_print_at(1, 0, "=");
        lcd_print_at(1, 1, result);
        waitForKeypress(key);
        lcd_clear();
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

void waitForKeypress(char key)
{
    key = keypad.getKey(); 
    while (key != 'C') key = keypad.getKey();
}