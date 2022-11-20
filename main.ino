#include <Keypad.h>
#include "lcd.h"
#include "Calculate.h"
#define MAX_EXPR_LEN 16

#define CHANGE_MODE 3
#define CHANGE_MODE_LED 12
#define LEFT_PARENTHESES_LED 2
#define RIGHT_PARENTHESES_LED 13

const byte ROWS = 4;
const byte COLS = 4;

char keys[ROWS][COLS] = {
    {'1', '2', '3', '+'},
    {'4', '5', '6', '-'},
    {'7', '8', '9', '*'},
    {'=', '0', '/', 'C'}
};

byte rowPins[ROWS] = {11, 10, 9, 8};
byte colPins[COLS] = {7, 6, 5, 4};;

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

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
    char * expression = (char * ) calloc(MAX_EXPR_LEN, sizeof(char));
    char * result = (char * ) calloc(MAX_EXPR_LEN, sizeof(char));
    char key = keypad.getKey();

    int i = 0;
    while (key != '=' && i < MAX_EXPR_LEN) 
    {
        if (digitalRead(CHANGE_MODE) == HIGH) 
        {
            digitalWrite(CHANGE_MODE_LED, HIGH);
            Serial.println("entered into change mode");
            if (key == '+')
            {
                expression[i] = '(';
                i++;
                lcd_print_at(0, 0, expression);
                Serial.println(expression);
                digitalWrite(LEFT_PARENTHESES_LED, HIGH);
                delay(500);
                digitalWrite(LEFT_PARENTHESES_LED, LOW);
                key = NO_KEY;
            } 
            else if (key == '-') 
            {
                expression[i] = ')';
                i++;
                lcd_print_at(0, 0, expression);
                Serial.println(expression);
                digitalWrite(RIGHT_PARENTHESES_LED, HIGH);
                delay(500);
                digitalWrite(RIGHT_PARENTHESES_LED, LOW);
                key = NO_KEY;
            }
            digitalWrite(CHANGE_MODE_LED, LOW);
        }
        if (key == 'C') 
        {
            Serial.println("entered into C");
            if (i > 0) 
            {
                i--;
                expression[i] = '\0';
                lcd_print_at(0, i, " ");
                Serial.println(expression);
            }
        } 
        else if (key != NO_KEY && key != 'C') 
        {
            Serial.println("entered into else");
            expression[i] = key;
            i++;
            lcd_print_at(0, 0, expression);
            Serial.println(expression);
        }
        key = keypad.getKey();
    }
    lcd_print_at(1, 0, "=");
    float resultNumber = Evaulate(expression);
    dtostrf(resultNumber, 0, 2, result);
    lcd_print_at(1, 1, result);
    delay(2000);

    key = keypad.getKey();
    while (key != 'C') 
    {
        key = keypad.getKey();
    }
    lcd_clear();
}