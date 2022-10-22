#include "parsedInformations.h"
#include "calculator.h"

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        printf("Usage: %s <input>", argv[0]);
    }

    struct parsedInformations calculatorTokens = parseCalculatorTokens(argv[1]);
    double calc = calculateRecursively(calculatorTokens);
}