#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define FLOAT_MIN -2147483648
#define FLOAT_MAX 2147483647

void putNumber(float nb)
{
	if (nb < 10 && nb >= 0)
	{
		putchar(nb + '0');
	}
	else if (nb >= 10)
	{
		putNumber(nb / 10);
		putchar((int)nb % 10 + '0');
	}
	else if (nb < 0)
	{
		if (nb == FLOAT_MIN)
		{
			putchar('-');
			putchar('2');
			putNumber(FLOAT_MAX);
		}
		else
		{
			putchar('-');
			putNumber(nb * -1);
		}
	}
}

