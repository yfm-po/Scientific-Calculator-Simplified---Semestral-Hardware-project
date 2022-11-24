#ifndef CALCULATE_H
#define CALCULATE_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

float cursorMover(char **str);
int  isBlank(char c);
float parsePart(char **str);
float parseFactor(char **str);
float parseSum(char **str);
void removeSpaces(char *in, char *out);
float Evaulate(char *str);

#endif