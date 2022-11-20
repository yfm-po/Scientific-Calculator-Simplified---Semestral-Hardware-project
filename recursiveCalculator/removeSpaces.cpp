#include "Calculate.h"
#include <string.h>
#include <stdio.h>

void removeSpaces(char *in, char *out)
{
	char *cursor = out;

	while (*in)
	{
		if (!isBlank(*in))
		{
			*cursor = *in;
			cursor++;
		}
		in++;
	}
	*cursor = '\0';
}
