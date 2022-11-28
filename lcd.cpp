#include "lcd.h"
#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>

//inicializacia LCD struktury
LiquidCrystal_I2C lcd(0x27, 16, 2);

//inicializacia LCD displeja
void lcd_init() 
{
    Serial.begin(9600);
    lcd.init();
    lcd.backlight();
    lcd.begin(16, 2);
}

// funckia na vymazanie znakov LCD displeja
void lcd_clear() 
{
    lcd.clear();
}

//funkcia na posunutie kurzora LCD displeja
void lcd_set_cursor(int y, int x) 
{
    lcd.setCursor(x, y);
}

//funkcia na vypis znaku na LCD displej
void lcd_print(char * text) 
{
    lcd.print(text);
}

//funkcia na nastavenie pozadia LCD displeja a nasledne vypisanie stringu na LCD displej
void lcd_print_at(int y, int x, char * text) 
{
    lcd_set_cursor(y, x);
    lcd_print(text);
}