#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
//#include <Arduino.h>

#include "parsedInformations.h"

/**
 * function is used to tokenizate the string input
 * 
 * @param {char*} input - The input string to parse
 * @returns {struct parsedInformations} 
 */
struct parsedInformations parseCalculatorTokens(char* input)
{
    struct parsedInformations CalculatorTokens;
    CalculatorTokens.tokenCount = 0;
    CalculatorTokens.hasOpenParentheses = false;
    CalculatorTokens.hasCloseParentheses = false;
    CalculatorTokens.openParenthesesCount = 0;
    CalculatorTokens.closeParenthesesCount = 0;
    CalculatorTokens.hasSyntaxError = false;

    for (int i = 0; i < strlen(input); i++)
    {
        if (isdigit(input[i]))
        {
            CalculatorTokens.tokens[CalculatorTokens.tokenCount].type = NUMBER;
            CalculatorTokens.tokens[CalculatorTokens.tokenCount].value.number = atof(&input[i]);
            CalculatorTokens.tokenCount++;
            while (isdigit(input[i]) || input[i] == '.')
            {
                i++;
            }
            i--;
        }
        else if (input[i] == '+' || input[i] == '-' || input[i] == '*' || input[i] == '/' || input[i] == '^' || input[i] == '√' || input[i] == '!')
        {
            CalculatorTokens.tokens[CalculatorTokens.tokenCount].type = OPERATOR;
            CalculatorTokens.tokens[CalculatorTokens.tokenCount].value.op = input[i];
            CalculatorTokens.tokenCount++;

            if ((!isdigit(input[i + 1]) || input[i + 1] != ' '))
            {
                syntaxError();
                CalculatorTokens.hasSyntaxError = true;
                break;
            }
        }
        else if (input[i] == '(')
        {
            CalculatorTokens.tokens[CalculatorTokens.tokenCount].type = LEFT_PAREN;
            CalculatorTokens.tokens[CalculatorTokens.tokenCount].value.op = input[i];
            CalculatorTokens.tokenCount++;
            CalculatorTokens.hasOpenParentheses = true;
            CalculatorTokens.openParenthesesCount++;

            if (!isdigit(input[i + 1]) || input[i + 1] != '(')
            {
                syntaxError();
                CalculatorTokens.hasSyntaxError = true;
                break;
            }
        }
        else if (input[i] == ')')
        {
            CalculatorTokens.tokens[CalculatorTokens.tokenCount].type = RIGHT_PAREN;
            CalculatorTokens.tokens[CalculatorTokens.tokenCount].value.op = input[i];
            CalculatorTokens.tokenCount++;
            CalculatorTokens.hasCloseParentheses = true;
            CalculatorTokens.closeParenthesesCount++;

            if (!isdigit(input[i + 1]) || input[i + 1] != ')')
            {
                syntaxError();
                CalculatorTokens.hasSyntaxError = true;
                break;
            }
        }
        else if (input[i] == ' ')
        {
            continue;
        }
        else
        {
            syntaxError();
            CalculatorTokens.hasSyntaxError = true;
            break;
        }
    }

    if (CalculatorTokens.hasOpenParentheses && !CalculatorTokens.hasCloseParentheses)
    {
        syntaxError();
        CalculatorTokens.hasSyntaxError = true;
    }
    else if (!CalculatorTokens.hasOpenParentheses && CalculatorTokens.hasCloseParentheses)
    {
        syntaxError();
        CalculatorTokens.hasSyntaxError = true;
    }
    else if (CalculatorTokens.openParenthesesCount != CalculatorTokens.closeParenthesesCount)
    {
        syntaxError();
        CalculatorTokens.hasSyntaxError = true;
    }

    return CalculatorTokens;
}


/**
 * function is used to print the parsed informations just for debugging purposes
 * 
 * @param {struct parsedInformations CalculatorTokens} CalculatorTokens - The parsed informations
 * @returns {void}
 */
void printParsedInformations(struct parsedInformations CalculatorTokens)
{
    for (int i = 0; i < CalculatorTokens.tokenCount; i++)
    {
        if (CalculatorTokens.tokens[i].type == NUMBER)
        {
            printf("NUMBER: %f \n", CalculatorTokens.tokens[i].value.number);
        }

        if (CalculatorTokens.tokens[i].type == OPERATOR)
        {
            printf("OPERATOR: %c \n", CalculatorTokens.tokens[i].value.op);
        }

        if (CalculatorTokens.tokens[i].type == LEFT_PAREN)
        {
            printf("LEFT_PAREN: %c \n", CalculatorTokens.tokens[i].value.op);
        }

        if (CalculatorTokens.tokens[i].type == RIGHT_PAREN)
        {
            printf("RIGHT_PAREN: %c \n", CalculatorTokens.tokens[i].value.op);
        }
    }
}

/**
 * function is used to print a syntax error message 
 * @returns {void}
 */
void syntaxError()
{
    printf("Syntax Error");
    exit(EXIT_FAILURE);
}