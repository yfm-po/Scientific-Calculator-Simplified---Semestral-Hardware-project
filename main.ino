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

int leds[] = { RIGHT_PARENTHESES_LED, LEFT_PARENTHESES_LED, CHANGE_MODE_LED };

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

//TODO: add bomb defuse game mode (with timer) to CHANGE_MODE + 123 combination where you get random 4 digit number and you have to guess it in 30 seconds,
//if you guess it, you have defused the bomb, else it blows up with blinking very fast leds and buzzer
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
            
            if (key == '1' || key == '2' || key == '3')
            {
                bombDefuseGame();
            }

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
            else if (key == '*')
            {
                expression[i] = ' ';
                i++;
                lcd_print_at(1, 0, "                ");
                lcd_print_at(1, i, CURSOR_POINTER);
                lcd_print_at(0, 0, expression);
                tone(BUZZER, 5000, 100);
                blinkWithLed(leds, 0);
                key = NO_KEY;
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
        syntaxErrorBlinking(leds, 3);
        waitForKeypress(key, keypad);
        lcd_clear();
        return;
    }
    else if (expression != NULL && strcmp(expression, ""))
    {
        lcd_print_at(1, 0, "                ");
        lcd_print_at(1, 0, "=");
        lcd_print_at(1, 1, result);
        FinisherBlinking(leds, 3);
        waitForKeypress(key, keypad);
        lcd_clear();
    }
    
    free(expression);
    free(result);
    lcd_clear();
}

void bombDefuseGame()
{
    lcd_clear();
    lcd_print_at(0, 0, "Well, this is");
    lcd_print_at(1, 0, "not just a");
    delay(1000);
    lcd_clear();

    lcd_print_at(0, 0, "calculator!");
    lcd_print_at(1, 0, "It's a bomb!");
    delay(1000);
    lcd_clear();

    lcd_print_at(0, 0, "You have to");
    lcd_print_at(1, 0, "defuse it!");
    delay(1000);
    lcd_clear();

    lcd_print_at(0, 0, "You have 1");
    lcd_print_at(1, 0, "chance!");
    delay(1000);
    lcd_clear();

    char* code = calloc(4, sizeof(char));
    for (int i = 0; i < 4; i++) code[i] = '\0';

    char* input = calloc(4, sizeof(char));
    for (int i = 0; i < 4; i++) input[i] = '\0';

    char* timeLeft = calloc(2, sizeof(char));
    for (int i = 0; i < 2; i++) timeLeft[i] = '\0';

    char defKey;

    for (int i = 0; i < strlen(code); i++) code[i] = TOINT(random(0, 9));
    Serial.println(code);

    for (int i = 0; i < strlen(input); i++) input[i] = '*';
    Serial.println(input);

    int guessIndex = 0;
    int countDelay = 1000;
    for (int i = 30; i >= 0; i--)
    {
        lcd_print_at(0, 0, "guess: ");
        lcd_print_at(0, 8, input);
        lcd_print_at(1, 0, "Time left: ");
        lcd_print_at(1, 11, itoa(i, timeLeft, 10));
        digitalWrite(CHANGE_MODE_LED, HIGH);
        tone(BUZZER, 5000, 100);
        delay(countDelay);
        countDelay -= 33.33;
        digitalWrite(CHANGE_MODE_LED, LOW);
        //get input from user to guess the code
        defKey = keypad.getKey();
        if (defKey != NO_KEY && defKey != 'C')
        {
            input[guessIndex] = defKey;
            guessIndex++;
            tone(BUZZER, 5000, 100);
        }
        else if (defKey == 'C')
        {
            if (guessIndex > 0)
            {
                input[guessIndex] = '*';
                guessIndex--;
                tone(BUZZER, 5000, 100);
            }
        }

        if (guessIndex == strlen(code))
        {
            if (!strcmp(input, code))
            {
                lcd_clear();
                lcd_print_at(0, 0, "Bomb has been");
                lcd_print_at(1, 0, "defused!");
                delay(1000);
                lcd_clear();
                lcd_print_at(0, 0, "You are");
                lcd_print_at(1, 0, "awesome!");
                delay(1000);
                lcd_clear();
                digitalWrite(CHANGE_MODE_LED, LOW);
                return;
            }
            else
            {
                lcd_clear();
                lcd_print_at(0, 0, "Wrong code!");
                lcd_print_at(1, 0, "You lose!");
                delay(1000);
                lcd_clear();
                lcd_print_at(0, 0, "Bomb has");
                lcd_print_at(1, 0, "exploded!");
                delay(1000);
                lcd_clear();
                digitalWrite(CHANGE_MODE_LED, LOW);
                return;
            }
        }
    }

    digitalWrite(CHANGE_MODE_LED, LOW);
    return;
}