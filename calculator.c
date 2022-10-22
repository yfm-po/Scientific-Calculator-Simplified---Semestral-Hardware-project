#include "parsedInformations.h"
#include "calculator.h"
#include <math.h>

double factorial(double n)
{
    if (n == 0)
    {
        return 1;
    }
    else
    {
        return n * factorial(n - 1);
    }
}



double calculateRecursively(struct parsedInformations parsedCalculatorTokens)
{
    double result = 0;
    int i = 0;
    bool throughFirstIteration = false;
    
    while (i < parsedCalculatorTokens.tokenCount)
    {
        if (parsedCalculatorTokens.hasOpenParentheses && parsedCalculatorTokens.hasCloseParentheses)
        {
            while (parsedCalculatorTokens.tokens[i].type != LEFT_PAREN)
            {
                i++;
            }
            if (parsedCalculatorTokens.tokens[i].type == LEFT_PAREN)
            {
                double subresult = 0;
                int j = i + 1;
                while (j < parsedCalculatorTokens.tokenCount)
                {
                    if (parsedCalculatorTokens.tokens[j].type == RIGHT_PAREN)
                    {
                        break;
                    }
                    else
                    {
                        if (parsedCalculatorTokens.tokens[j].type == NUMBER)
                        {
                            subresult = parsedCalculatorTokens.tokens[j].value.number;
                        }
                        else if (parsedCalculatorTokens.tokens[j].type == OPERATOR)
                        {
                            if (parsedCalculatorTokens.tokens[j].value.op == '+')
                            {
                                subresult += parsedCalculatorTokens.tokens[j + 1].value.number;
                            }
                            else if (parsedCalculatorTokens.tokens[j].value.op == '-')
                            {
                                subresult -= parsedCalculatorTokens.tokens[j + 1].value.number;
                            }
                            else if (parsedCalculatorTokens.tokens[j].value.op == '*')
                            {
                                subresult *= parsedCalculatorTokens.tokens[j + 1].value.number;
                            }
                            else if (parsedCalculatorTokens.tokens[j].value.op == '/')
                            {
                                subresult /= parsedCalculatorTokens.tokens[j + 1].value.number;
                            }
                            else if (parsedCalculatorTokens.tokens[j].value.op == '^')
                            {
                                subresult = pow(subresult, parsedCalculatorTokens.tokens[j + 1].value.number);
                            }
                            else if (parsedCalculatorTokens.tokens[j].value.op == SQUARE_ROOT_ASCII)
                            {
                                subresult = sqrt(subresult);
                            }
                            else if (parsedCalculatorTokens.tokens[j].value.op == '!')
                            {
                                subresult = factorial(subresult);
                            }
                        }
                    }
                    j++;
                }
                result = subresult;
                //i = j;
            }
            i++;
        }
        else
        {
            if (parsedCalculatorTokens.tokens[i].type == NUMBER)
            {
                if (!throughFirstIteration)
                {
                    result = parsedCalculatorTokens.tokens[i].value.number;
                    throughFirstIteration = true;
                }
            }
            else if (parsedCalculatorTokens.tokens[i].type == OPERATOR)
            {
                if (parsedCalculatorTokens.tokens[i].value.op == '*')
                {
                    result *= parsedCalculatorTokens.tokens[i + 1].value.number;
                }
                else if (parsedCalculatorTokens.tokens[i].value.op == '/')
                {
                    result /= parsedCalculatorTokens.tokens[i + 1].value.number;
                }
                else if (parsedCalculatorTokens.tokens[i].value.op == '+')
                {
                    result += parsedCalculatorTokens.tokens[i + 1].value.number;
                }
                else if (parsedCalculatorTokens.tokens[i].value.op == '-')
                {
                    result -= parsedCalculatorTokens.tokens[i + 1].value.number;
                }
                else if (parsedCalculatorTokens.tokens[i].value.op == '^')
                {
                    result = pow(result, parsedCalculatorTokens.tokens[i + 1].value.number);
                }
                else if (parsedCalculatorTokens.tokens[i].value.op == SQUARE_ROOT_ASCII)
                {
                    result = sqrt(result);
                }
                else if (parsedCalculatorTokens.tokens[i].value.op == '!')
                {
                    result = factorial(result);
                }
            }
            i++;
        }
    }

    printf("%f\n", result);
    return result;
}


