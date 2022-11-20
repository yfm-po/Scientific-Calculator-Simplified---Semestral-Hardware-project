#include "Calculate.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int isBlank(char c) 
{
    return (c == '\t' || c == '\n' || c == '\v' || c == '\f' || c == '\r' || c == ' ');
}

float cursorMover(char ** str) 
{
    float result = 0, sign = 1;
    while (isBlank( ** str)) ( * str) ++;

    if ( ** str == '-') 
    {
        sign = -1;
        ( * str) ++;
        if ( ** str == '(') return -1;
    } 
    else if ( ** str == '+') 
    {
        ( * str) ++;
    }

    while ( ** str >= '0' && ** str <= '9') 
    {
        result = (10 * result) + ** str - '0';
        ( * str) ++;
    }

    return (float)(result * sign);
}

void removeSpaces(char * in , char * out) 
{
    char * cursor = out;

    while ( * in ) 
    {
        if (!isBlank( * in )) 
        {
            * cursor = * in ;
            cursor++;
        } in ++;
    }
    * cursor = '\0';
}

float parsePart(char ** str) 
{
    float out;
    float sum;
    out = cursorMover(str);

    if ( ** str == '(') 
    {
        ( * str) ++;
        sum = parseSum(str);
        ( * str) ++;
        return out != 0 ? (float)(sum * out) : (float) sum;
    }
    return (float) out;
}

float parseFactor(char ** str) 
{
    float fac1;
    float fac2;
    char op;

    fac1 = parsePart(str);
    while ( ** str == '*' || ** str == '/') 
    {
        op = ** str;
        ( * str) ++;
        fac2 = parsePart(str);
        if (op == '*') fac1 *= fac2;
        if (op == '/') fac1 /= fac2;
    }
    return fac1;
}

float parseSum(char ** str) 
{
    float pro1;
    float pro2;
    char op;

    pro1 = parseFactor(str);
    while ( ** str == '+' || ** str == '-') 
    {
        op = ** str;
        ( * str) ++;
        pro2 = parseFactor(str);
        if (op == '+')(float)(pro1 += pro2);
        if (op == '-')(float)(pro1 -= pro2);
    }
    return (float) pro1;
}

float Evaulate(char * str) 
{
    char * out;
    char * hold;

    if (!(out = (char * ) malloc(sizeof(char) * strlen(str) + 1))) return 0;
    hold = out;
    removeSpaces(str, out);
    float o = parseSum( & out);
    free(hold);
    return o;
}