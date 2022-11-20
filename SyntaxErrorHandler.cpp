#include "SyntaxErrorHandler.h"
#include "SyntaxErrorHandler.h"

bool hasSyntaxError(char *input)
{
    int open = 0;
    int close = 0;
    for (int i = 0; i < strlen(input); i++)
    {
        if (input[i] == '(')
        {
            open++;
        }
        else if (input[i] == ')')
        {
            close++;
        }
        
        if (input[i] == '(')
        {
            if (input[i + 1] == ')')
            {
                return true;
            }
        }

        if (input[i] == ')')
        {
            if (input[i + 1] == '(')
            {
                return true;
            }
        }

        if (input[i] == '+' || input[i] == '-' || input[i] == '*' || input[i] == '/')
        {
            if (input[i + 1] == '+' || input[i + 1] == '-' || input[i + 1] == '*' || input[i + 1] == '/')
            {
                return true;
            }
        }
    }
    if (open != close)
    {
        return true;
    }
    return false;
}

void syntaxError()
{
    lcd_print_at(1, 0, "Syntax Error!");
}