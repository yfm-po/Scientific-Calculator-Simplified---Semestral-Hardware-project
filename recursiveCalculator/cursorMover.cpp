#include <stdio.h>

int	isBlank(char c)
{
	return (c == '\t' || c == '\n' || c == '\v' || c == '\f' || c == '\r' || c == ' ');
}

float cursorMover(char **str)
{
	float result = 0, sign = 1;
	while (isBlank(**str)) (*str)++;

	if (**str == '-')
	{
		sign = -1;
		(*str)++;
		if (**str == '(') return -1;
	}
	else if (**str == '+')
	{
		(*str)++;
	}
	
	while (**str >= '0' && **str <= '9')
	{
		result = (10 * result) + **str - '0';
		(*str)++;
	}

	printf("%f\n", result * sign);
	return (float)(result * sign);
}
