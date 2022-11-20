#ifndef _LCD_H
#define _LCD_H

void lcd_init();
void lcd_clear();
void lcd_print(char* text);
void lcd_print_at(int y, int x, char* text);

#endif