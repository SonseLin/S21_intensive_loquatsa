#ifndef STACK_H
#define STACK_H

#include <stdio.h>
#define STACK_SIZE 50

typedef struct {
    char data[STACK_SIZE];
    int top;
} Stack;

void push(Stack* stack, char item);
char pop(Stack* stack);

// char top_stack(Stack* stack);
int is_empty(Stack* stack);

void push_num(double* stack, int* top, double x);

double pop_num(const double* stack, int* top);

#endif