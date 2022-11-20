#include "Calculate.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

float parsePart(char **str)
{
	float out;
	float sum;
	out = cursorMover(str);

	if (**str == '(')
	{
		(*str)++;
		sum = parseSum(str);
		(*str)++;
		return out != 0 ? (float)(sum * out) : (float)sum;
	}
	return (float)out;
}

float parseFactor(char **str)
{
	float fac1;
	float fac2;
	char op;

	fac1 = parsePart(str);
	while (**str == '*' || **str == '/' || **str == '%')
	{
		op = **str;
		(*str)++;
		fac2 = parsePart(str);
		if (op == '*')	fac1 *= fac2;
		if (op == '/')	fac1 /= fac2;
		//if (op == '%')	fac1 = fac1 % fac2;
	}
	return fac1;
}

float parseSum(char **str)
{
	float pro1;
	float pro2;
	char op;

	pro1 = parseFactor(str);
	while (**str == '+' || **str == '-')
	{
		op = **str;
		(*str)++;
		pro2 = parseFactor(str);
		if (op == '+') (float)(pro1 += pro2);
		if (op == '-') (float)(pro1 -= pro2);
	}
	return (float)pro1;
}

float Evaulate(char *str)
{
	char *out;
	char *hold;

	if (!(out = (char *)malloc(sizeof(char) * strlen(str) + 1))) return 0;
	hold = out;
	removeSpaces(str, out);
	float o = parseSum(&out);
	free(hold);
	return o;
}

int	main(int argc, char **argv)
{
	if (argc > 1)
	{
		putNumber(Evaulate(argv[1]));
		putchar('\n');
	}
	return 0;
}
