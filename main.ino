#include "Calculate.h"
#include "ui.h"
#include "lcd.h"
#include "syntaxErrorHandler.h"

//makra vyuzite v programe
#define MAX_EXPR_LEN 16
#define HISTORY_SIZE 10

//makra vyuzite na piny
#define CHANGE_MODE 3
#define CHANGE_MODE_LED 12
#define LEFT_PARENTHESES_LED 2
#define RIGHT_PARENTHESES_LED 13

//makra vyuzite na zobrazenie na LCD
#define TOINT(char) (char - '0')
#define CURSOR_POINTER "^"

//deklaracia historyIndexu a pole na ulozenie vyrazov a vysledkov
int historyIndex;
char** expressionHistory = (char **) calloc(HISTORY_SIZE, sizeof(char*));
char** resultHistory = (char **) calloc(HISTORY_SIZE, sizeof(char*));

//pole na makra lediek
int leds[] = { RIGHT_PARENTHESES_LED, LEFT_PARENTHESES_LED, CHANGE_MODE_LED };


//deklaracie na vyuzitie keypadu
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

//funkcia setup na inicializacie pinov a poli, zobrazenie uvodnej spravy na LCD
void setup()
{
    lcd_init();

    pinMode(CHANGE_MODE, INPUT);
    pinMode(CHANGE_MODE_LED, OUTPUT);
    pinMode(LEFT_PARENTHESES_LED, OUTPUT);
    pinMode(RIGHT_PARENTHESES_LED, OUTPUT);
    pinMode(BUZZER, OUTPUT);
    randomSeed(analogRead(0));

    historyIndex = 0;
    for (int i = 0; i < HISTORY_SIZE; i++)
    {
      expressionHistory[i] = (char *) calloc(MAX_EXPR_LEN, sizeof(char));
      resultHistory[i] = (char *) calloc(MAX_EXPR_LEN, sizeof(char));
    }
    
    loadingScreen();
}

//funkcia loop na hlavnu slucku kalkulacky
void loop() 
{
    //deklaracia stringov na elementy vyrazov, a na index vyrazu aby sme vedeli zobrazit index na LCD
    char *expression = (char *) calloc(MAX_EXPR_LEN, sizeof(char));
    char *result = (char *) calloc(MAX_EXPR_LEN, sizeof(char));
    char *historyIndexChar = (char *) calloc(1, sizeof(char));

    //vypisanie cursoru na LCD
    lcd_print_at(1, 0, CURSOR_POINTER);

    //hlavna slucka na kalkulacie
    char key = keypad.getKey();
    int i = 0;
    while (key != '=' && i < MAX_EXPR_LEN)
    {
        //kombinacie s CHANGE_MODE/shift button
        if (digitalRead(CHANGE_MODE) == HIGH)
        {
            digitalWrite(CHANGE_MODE_LED, HIGH);
            
            if (key == '3')
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
        //vymazanie posledneho karakteru a posun naspat o 1
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
        //pridavanie cisel alebo operacii
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

    //vykonanie kalkulacie
    float resultNumber = Evaulate(expression);
    
    //konverzia vysledku na string aby sme ho mohli vypisat na LCD
    dtostrf(resultNumber, 0, 2, result);

    //kopirovanie vysledku, pripadne Syntax erroru do historie
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
    
    //Vypis syntax erroru alebo vysledku na LCD
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
    
    //uvolnenie pamate
    free(expression);
    free(result);
    lcd_clear();
}


//funkcia na minihru, ktora sa vola Bomb Defuse Game
void bombDefuseGame()
{
    //Vypis uvodnej spravy
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

    //alokovanie pamate pre nahodne vygenerovany kod, vstup, a string na cas
    char* code = calloc(4, sizeof(char));
    for (int i = 0; i < 4; i++) code[i] = '\0';

    char* input = calloc(4, sizeof(char));
    for (int i = 0; i < 4; i++) input[i] = '\0';

    char* timeLeft = calloc(2, sizeof(char));
    for (int i = 0; i < 2; i++) timeLeft[i] = '\0';

    char defKey;

    //generovanie nahodneho kodu
    for (int i = 0; i < 4; i++) code[i] = (char)(random(0, 9) + '0');
    code[4] = '\0';
    
    Serial.println(code);

    //inicializacia inputu
    strcpy(input, "****");
    Serial.println(input);

    int guessIndex = 0;
    int countDelay = 1000;

    //hlavna slucka minihry
    int i;
    for (i = 60; i >= 0; i--)
    {
        //v pripade nezadania kodu v case, vypis spravy o prehre
        if (i == 1)
        {
            lcd_clear();
            lcd_print_at(0, 0, "Time's up!");
            lcd_print_at(1, 0, "You lose!");
            delay(1000);
            lcd_clear();
            lcd_print_at(0, 0, "The code was");
            lcd_print_at(1, 0, code);
            delay(1000);
            lcd_clear();
            break;
        }

        lcd_print_at(0, 0, "guess: ");
        lcd_print_at(0, 8, input);
        lcd_print_at(1, 0, "Time left: ");

        //vypis casu
        if (i >= 10) lcd_print_at(1, 11, itoa(i, timeLeft, 10));
        else
        {
            lcd_print_at(1, 11, "0");
            lcd_print_at(1, 12, itoa(i, timeLeft, 10));
        }

        digitalWrite(CHANGE_MODE_LED, HIGH);
        tone(BUZZER, 5000, 100);
        delay(countDelay);
        countDelay -= 16.66;
        digitalWrite(CHANGE_MODE_LED, LOW);


        //nacitanie stlacenej klavesy
        defKey = keypad.getKey();
        if (defKey != NO_KEY && (TOINT(defKey) >= 0 && TOINT(defKey) <= 9))
        {
            input[guessIndex] = defKey;
            guessIndex++;
            Serial.println(input);
            Serial.println(guessIndex);
            Serial.println(code);
            tone(BUZZER, 5000, 100);
        }

        //v pripade, ze je kod zadany, vypis spravy o vyhre, pripadne o prehre
        if (guessIndex == strlen(code))
        {
            lcd_clear();
            lcd_print_at(0, 0, "guess: ");
            lcd_print_at(0, 8, input);
            if (!strcmp(input, code))
            {
                lcd_clear();
                digitalWrite(CHANGE_MODE_LED, LOW);
                lcd_print_at(0, 0, "Bomb has been");
                lcd_print_at(1, 0, "defused!");
                delay(1000);
                lcd_clear();
                lcd_print_at(0, 0, "You are");
                lcd_print_at(1, 0, "awesome!");
                digitalWrite(LEFT_PARENTHESES_LED, HIGH);
                digitalWrite(RIGHT_PARENTHESES_LED, HIGH);
                tone(BUZZER, 2500, 2000);
                digitalWrite(LEFT_PARENTHESES_LED, LOW);
                digitalWrite(RIGHT_PARENTHESES_LED, LOW);
                delay(1000);
                lcd_clear();
                return;
            }
            else
            {
                lcd_clear();
                lcd_print_at(0, 0, "Wrong code!");
                lcd_print_at(1, 0, "You lose!");
                delay(1000);
                lcd_clear();
                lcd_print_at(0, 0, "The code was");
                lcd_print_at(1, 0, code);
                delay(1000);
                lcd_clear();
                break;
            }
        }
    }
    tone(BUZZER, 5000, 2000);
    delay(1000);
    lcd_clear();

    //uvolnenie pamate
    free(code);
    free(input);
    free(timeLeft);
}