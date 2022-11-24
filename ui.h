#ifndef UI_H
#define UI_H

#include "lcd.h"
#include "SyntaxErrorHandler.h"
#include <Keypad.h>

#define BUZZER A3

void blinkWithLed(int *leds, int ledIndex);
void blinkWithLeds(int *leds, int ledsCount);
void waitForKeypress(char key, Keypad keypad);
void loadingScreen();
void animateLoadingDots(int row, int startIndex);
void showHistoryElementBasedOnIndex(char **expressionHistory, char **resultHistory, int index);

#endif