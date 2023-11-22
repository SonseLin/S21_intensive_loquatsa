#ifndef CALCULATE_H
#define CALCULATE_H

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "parcer.h"
#include "stack.h"

double notation_answer(char* notation, double x, int* no_err);
int procede(char* notation, double num1, double num2, double* result, double* nums, int* top);

#endif