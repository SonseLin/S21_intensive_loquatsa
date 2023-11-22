#ifndef PAINT_H
#define PAINT_H

#include <stdio.h>
#define WIDTH 80
#define HEIGHT 25

#define M_PI 3.1415

#define RANGE (double)(2.0 / HEIGHT)
#define POSSIBLE_RANGE (RANGE / 2.0)

void output_graph(char* postfix, int* no_err);

#endif