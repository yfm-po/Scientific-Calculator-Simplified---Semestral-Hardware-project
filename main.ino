#include "Calculate.h"
#include "ui.h"
#include "lcd.h"
#include "syntaxErrorHandler.h"

#define MAX_EXPR_LEN 16
#define HISTORY_SIZE 10

#define CHANGE_MODE 3
#define CHANGE_MODE_LED 12
#define LEFT_PARENTHESES_LED 2
#define RIGHT_PARENTHESES_LED 13

#define TOINT(char) (char - '0')
#define CURSOR_POINTER "^"

int historyIndex;
char** expressionHistory = (char **) calloc(HISTORY_SIZE, sizeof(char*));
char** resultHistory = (char **) calloc(HISTORY_SIZE, sizeof(char*));

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

void setup()
{
    lcd_init();

    pinMode(CHANGE_MODE, INPUT);
    pinMode(CHANGE_MODE_LED, OUTPUT);
    pinMode(LEFT_PARENTHESES_LED, OUTPUT);
    pinMode(RIGHT_PARENTHESES_LED, OUTPUT);
    pinMode(BUZZER, OUTPUT);

    historyIndex = 0;
    for (int i = 0; i < HISTORY_SIZE; i++)
    {
      expressionHistory[i] = (char *) calloc(MAX_EXPR_LEN, sizeof(char));
      resultHistory[i] = (char *) calloc(MAX_EXPR_LEN, sizeof(char));
    }
    
    loadingScreen();
}

void loop() 
{
    char *expression = (char *) calloc(MAX_EXPR_LEN, sizeof(char));
    char *result = (char *) calloc(MAX_EXPR_LEN, sizeof(char));
    char *historyIndexChar = (char *) calloc(1, sizeof(char));
    lcd_print_at(1, 0, CURSOR_POINTER);

    char key = keypad.getKey();
    int i = 0;
    while (key != '=' && i < MAX_EXPR_LEN)
    {
        if (digitalRead(CHANGE_MODE) == HIGH)
        {
            digitalWrite(CHANGE_MODE_LED, HIGH);
            if (key == '/')
            {
                tone(BUZZER, 5000, 100);
                lcd_print_at(0, 0, "Entered history");
                delay(850);
                lcd_clear();
                while (digitalRead(CHANGE_MODE) == HIGH && historyIndex > 0)
                {
                    tone(BUZZER, 5000, 100);
                    delay(500);
                    noTone(BUZZER);
                    if (historyIndex == 1)
                    {
                        lcd_clear();
                        showHistoryElementBasedOnIndex(expressionHistory, resultHistory, 0);
                        
                        key = NO_KEY;
                        while (key != 'C')
                        {
                            key = keypad.getKey();
                        }
                        tone(BUZZER, 5000, 100);
                        lcd_clear();
                    }
                    else
                    {
                        lcd_print_at(1, 0, "                ");
                        lcd_print_at(0, 0, "Enter index");
                        lcd_print_at(1, 0, "between 0 and");
                        lcd_print_at(1, 14, itoa(historyIndex - 1, historyIndexChar, 10));
    
                        key = NO_KEY;
                        while ((TOINT(key) < 0 || TOINT(key) > historyIndex - 1) || key == NO_KEY)
                        {
                            key = keypad.getKey();
                        }
                        tone(BUZZER, 5000, 100);
                        
                        lcd_clear();
                        showHistoryElementBasedOnIndex(expressionHistory, resultHistory, TOINT(key));
                        
                        key = NO_KEY;
                        while (key != 'C')
                        {
                            key = keypad.getKey();
                        }
                        tone(BUZZER, 5000, 100);
                        lcd_clear();
                    }
                }
                return;
            }
            else if (key == '+')
            {
                expression[i] = '(';
                i++;
                lcd_print_at(1, 0, "                ");
                lcd_print_at(1, i, CURSOR_POINTER);
                lcd_print_at(0, 0, expression);
                tone(BUZZER, 5000, 100);
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
                tone(BUZZER, 5000, 100);
                blinkWithLed(leds, 1);
                key = NO_KEY;
            }
            else if (key == 'C')
            {
                for (int j = 0; j < MAX_EXPR_LEN; j++) expression[j] = '\0';
                i = 0;
                lcd_clear();
                lcd_print_at(1, i, CURSOR_POINTER);
                tone(BUZZER, 5000, 100);
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
                tone(BUZZER, 5000, 100);
            }
        } 
        else if (key != NO_KEY && key != 'C')
        {
            expression[i] = key;
            i++;
            lcd_print_at(1, 0, "                ");
            lcd_print_at(1, i, CURSOR_POINTER);
            lcd_print_at(0, 0, expression);
            tone(BUZZER, 5000, 100);
        }
        key = keypad.getKey();
    }

    float resultNumber = Evaulate(expression);
    
    dtostrf(resultNumber, 0, 2, result);

    if (expression != NULL && strcmp(expression, "") && result != NULL && strcmp(result, ""))
    {
        if (!hasSyntaxError(expression))
        {
            strcpy(expressionHistory[historyIndex], expression);
            strcpy(resultHistory[historyIndex], result);
        }
        else
        {
            strcpy(expressionHistory[historyIndex], expression);
            strcpy(resultHistory[historyIndex], "Syntax Error!");
        }
        historyIndex++;
    }
    
    if (hasSyntaxError(expression))
    {
        syntaxError();
        waitForKeypress(key, keypad);
        lcd_clear();
        return;
    }
    else if (expression != NULL && strcmp(expression, ""))
    {
        lcd_print_at(1, 0, "                ");
        lcd_print_at(1, 0, "=");
        lcd_print_at(1, 1, result);
        waitForKeypress(key, keypad);
        lcd_clear();
    }
    
    free(expression);
    free(result);
    lcd_clear();
}