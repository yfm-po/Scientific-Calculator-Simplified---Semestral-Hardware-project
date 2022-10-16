#include "functions.h"

#include <stdio.h>
#include <math.h>


double add(double number1, double number2){
    return number1 + number2;
}

double sub(double number1, double number2){
    return number1 - number2;
}

double mul(double number1, double number2){
    return number1 * number2;
}

double div(double number1, double number2){
    return number1 / number2;
}

double power(double number, double factor){
    return pow(number, factor);
}

double root(double number, double root){
    return pow(number, (double)(1 / root));
}

double factorial(int number){
    double smt = 1;

    if(number = 0){
        return smt;
    }
    
    if(number < 0){
        return 0;
    }
    //change to 0 if it returns 0 all the time
    for(int i = number; i != 0; i--){
        smt = smt * i;
    }

    return smt;
}