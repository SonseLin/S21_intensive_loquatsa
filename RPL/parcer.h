#ifndef PARCER_H
#define PARCER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "stack.h"

#define PLUS 43
#define MINUS 45
#define SIN 83
#define COS 67
#define TAN 84
#define CTG 71
#define SQRT 81
#define LN 76

int is_number(char c);
int is_number_or_x(char c);
int is_number_or_dot_or_x(char c);
char* input_string(void);
int op_weight(char op);
int is_lexema(const char* current, const char* input, int len, const char* lexema, int lex_len);
int is_op(char ch);

int is_func(const char* input, const char* current, int len);
char* convert(const char* input, int len, int* everything_is_fine);

void push_all_possible_op(int func, Stack* stack, const char* current, const char** input);
#endif