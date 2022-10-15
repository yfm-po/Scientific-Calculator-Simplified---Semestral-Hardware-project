#ifndef UTIL_H
#define UTIL_H

//#include <Arduino.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#define MAX_DIGITS 16

struct parsedCalculatorTokens
{
    int tokenCount;
    char tokens[MAX_DIGITS];
    double numbers[MAX_DIGITS];
    char operators[MAX_DIGITS];
    char parentheses[MAX_DIGITS];
    bool hasOpenParentheses;
    bool hasCloseParentheses;
    int openParenthesesCount;
    int closeParenthesesCount;
    bool hasSyntaxError;
};
  
struct parsedCalculatorTokens parseCalculatorTokens(char* input);

void printParsedCalculatorTokens(struct parsedCalculatorTokens CalculatorTokens);

void syntaxError();

#endif /* UTIL_H */