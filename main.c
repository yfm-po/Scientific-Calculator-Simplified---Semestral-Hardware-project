#include "parsedCalculatorTokens.h"

int main(int argc, char* argv[])
{
    struct parsedCalculatorTokens calculatorTokens = parseCalculatorTokens(argv[1]);
    printParsedCalculatorTokens(calculatorTokens);
}