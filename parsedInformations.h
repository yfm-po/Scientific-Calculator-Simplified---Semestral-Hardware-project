#ifndef UTIL_H
#define UTIL_H

//#include <Arduino.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#define MAX_DIGITS 16


struct token 
{
    enum type {NUMBER, OPERATOR, LEFT_PAREN, RIGHT_PAREN} type;
    union 
    {
        double number;
        char op;
    } value;
};

struct parsedInformations
{
    struct token tokens[MAX_DIGITS];
    int tokenCount;
    double numbers[MAX_DIGITS];
    char operators[MAX_DIGITS];
    char parentheses[MAX_DIGITS];
    bool hasOpenParentheses;
    bool hasCloseParentheses;
    int openParenthesesCount;
    int closeParenthesesCount;
    bool hasSyntaxError;
};
  
struct parsedInformations parseCalculatorTokens(char* input);

void printParsedInformations(struct parsedInformations CalculatorTokens);

void syntaxError();

#endif /* UTIL_H */