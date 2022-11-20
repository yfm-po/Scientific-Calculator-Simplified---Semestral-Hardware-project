#include "parsedInformations.h"
#include "calculator.h"
#include <math.h>
#include "lstack.h"

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

double calculateResult(struct parsedInformations parsedCalculatorTokens)
{
    LStack stack = CreateStack();
    double result = 0;

    for (int i = 0; i < parsedCalculatorTokens.tokenCount; i++)
    {
        if (parsedCalculatorTokens.tokens[i].type == NUMBER)
        {
            Push(parsedCalculatorTokens.tokens[i].value.number, stack);
            printf("found first number: ");
            PrintStack(stack);
            putchar('\n');
        }
        if (parsedCalculatorTokens.tokens[i].type == OPERATOR)
        {
            double operand1 = TopAndPop(stack);
            printf("found first operand: %c\n", operand1);
            PrintStack(stack);
            double operand2;
            for (int j = i + 1; j < parsedCalculatorTokens.tokenCount; j++)
            {
                if (parsedCalculatorTokens.tokens[j].type == NUMBER)
                {
                    operand2 = parsedCalculatorTokens.tokens[j].value.number;
                    printf("found second operand: %d\n", operand2);
                    PrintStack(stack);
                    operand2 = TopAndPop(stack);
                    break;
                }
            }
            switch (parsedCalculatorTokens.tokens[i].value.op)
            {
                case '+':
                    result = operand1 + operand2;
                    break;
                case '-':
                    result = operand2 - operand1;
                    break;
                case '*':
                    result = operand1 * operand2;
                    break;
                case '/':
                    result = operand2 / operand1;
                    break;
                case '^':
                    result = pow(operand2, operand1);
                    break;
                case SQUARE_ROOT_ASCII:
                    result = sqrt(operand1);
                    break;
                case '!':
                    result = factorial(operand1);
                    break;
                default:
                    break;
            }
            Push(result, stack);
        }
    }

    result = TopAndPop(stack);

    return result;
}


