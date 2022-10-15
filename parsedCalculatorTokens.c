#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
//#include <Arduino.h>

#include "parsedCalculatorTokens.h"

struct parsedCalculatorTokens parseCalculatorTokens(char* input)
{
    struct parsedCalculatorTokens CalculatorTokens;
    CalculatorTokens.tokenCount = 0;
    CalculatorTokens.openParenthesesCount = 0;
    CalculatorTokens.closeParenthesesCount = 0;
    CalculatorTokens.hasOpenParentheses = false;
    CalculatorTokens.hasCloseParentheses = false;
    CalculatorTokens.hasSyntaxError = false;

    for (int i = 0; i < strlen(input); i++)
    {
        char currentChar = input[i];
        if (isdigit(currentChar))
        {
            CalculatorTokens.tokens[CalculatorTokens.tokenCount] = 'n';
            CalculatorTokens.numbers[CalculatorTokens.tokenCount] = atof(&input[i]);
            CalculatorTokens.tokenCount++;
            while (isdigit(input[i + 1]) || input[i + 1] == '.')
            {
                i++;
            }
        }
        else if (currentChar == '+' || currentChar == '-' || currentChar == '*' || currentChar == '/')
        {
            CalculatorTokens.tokens[CalculatorTokens.tokenCount] = 'o';
            CalculatorTokens.operators[CalculatorTokens.tokenCount] = currentChar;
            CalculatorTokens.tokenCount++;
        }
        else if (currentChar == '(')
        {
            CalculatorTokens.tokens[CalculatorTokens.tokenCount] = 'p';
            CalculatorTokens.parentheses[CalculatorTokens.tokenCount] = currentChar;
            CalculatorTokens.tokenCount++;
            CalculatorTokens.hasOpenParentheses = true;
            CalculatorTokens.openParenthesesCount++;
        }
        else if (currentChar == ')')
        {
            CalculatorTokens.tokens[CalculatorTokens.tokenCount] = 'p';
            CalculatorTokens.parentheses[CalculatorTokens.tokenCount] = currentChar;
            CalculatorTokens.tokenCount++;
            CalculatorTokens.hasCloseParentheses = true;
            CalculatorTokens.closeParenthesesCount++;
        }
        else if (currentChar == ' ')
        {
            // 
        }
        else
        {
            syntaxError();
            CalculatorTokens.hasSyntaxError = true;
            break;
        }
    }

    if (CalculatorTokens.openParenthesesCount != CalculatorTokens.closeParenthesesCount)
    {
        syntaxError();
        CalculatorTokens.hasSyntaxError = true;
    }

    return CalculatorTokens;
}

void printParsedCalculatorTokens(struct parsedCalculatorTokens CalculatorTokens)
{
    printf("tokenCount: %d\n", CalculatorTokens.tokenCount);
    printf("tokens: ");
    for (int i = 0; i < CalculatorTokens.tokenCount; i++)
    {
        printf("%c", CalculatorTokens.tokens[i]);
    }
    printf("\n");
    printf("numbers: ");
    for (int i = 0; i < CalculatorTokens.tokenCount; i++)
    {
        printf("%f", CalculatorTokens.numbers[i]);
    }
    printf("\n");
    printf("operators: ");
    for (int i = 0; i < CalculatorTokens.tokenCount; i++)
    {
        printf("%c", CalculatorTokens.operators[i]);
    }
    printf("\n");
    printf("parentheses: ");
    for (int i = 0; i < CalculatorTokens.tokenCount; i++)
    {
        printf("%c", CalculatorTokens.parentheses[i]);
    }
    printf("\n");
    printf("hasOpenParentheses: %d\n", CalculatorTokens.hasOpenParentheses);
    printf("hasCloseParentheses: %d\n", CalculatorTokens.hasCloseParentheses);
    printf("openParenthesesCount: %d\n", CalculatorTokens.openParenthesesCount);
    printf("closeParenthesesCount: %d\n", CalculatorTokens.closeParenthesesCount);
    printf("hasSyntaxError: %d\n", CalculatorTokens.hasSyntaxError);
}

void syntaxError()
{
    printf("Syntax Error");
    exit(EXIT_FAILURE);
}