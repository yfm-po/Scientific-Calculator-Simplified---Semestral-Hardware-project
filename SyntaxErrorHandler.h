#ifndef SYNTEXERRORHANDLER_H
#define SYNTEXERRORHANDLER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "lcd.h"

bool hasSyntaxError(char *input);

void syntaxError();

#endif