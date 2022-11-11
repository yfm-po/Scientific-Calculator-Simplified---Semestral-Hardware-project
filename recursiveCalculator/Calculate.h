#ifndef EVAL_EXPR_H
#define EVAL_EXPR_H
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

float cursorMover(char **str);
int	isBlank(char c);
void putNumber(int nb);
float parsePart(char **str);
float parseFactor(char **str);
float parseSum(char **str);
void removeSpaces(char *in, char *out);

#endif
