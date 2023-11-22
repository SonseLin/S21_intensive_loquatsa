#include "pl_math.h"

double notation_answer(char* notation, double x, int* no_err) {
    double nums[80];
    int top = -1, is_double = 0;
    double num1, num2, result;
    while (*notation != '\0' && *no_err) {
        if (*notation == 'x') {
            push_num(nums, &top, x);
        } else if (is_number(*notation)) {
            double current = 0;

            while (*notation != 'x' && *notation != ' ') {
                if (*notation != '.' && !is_double) {
                    current = current * 10 + (double)(*notation - 48);
                    notation++;
                } else if (*notation == '.' && !is_double) {
                    notation++;
                    is_double = 1;
                } else {
                    current = current + (double)(*notation - 48) / pow(10, is_double);
                    notation++;
                    is_double++;
                }
            }
            notation--;
            is_double = 0;
            push_num(nums, &top, current);
        } else if (*notation != ' ') {
            num1 = pop_num(nums, &top);
            num2 = pop_num(nums, &top);
            *no_err = procede(notation, num1, num2, &result, nums, &top);
            push_num(nums, &top, result);
        }
        notation++;
    }
    return pop_num(nums, &top);
}

int procede(char* notation, double num1, double num2, double* result, double* nums, int* top) {
    int res = 1;
    switch (*notation) {
        case '+':
            *result = num2 + num1;
            break;
        case '-':
            *result = num2 - num1;
            break;
        case '*':
            *result = num2 * num1;
            break;
        case '/':
            *result = num2 / num1;
            break;
        case 's':
            *result = sin(num1);
            push_num(nums, top, num2);
            break;
        case 'c':
            *result = cos(num1);
            push_num(nums, top, num2);
            break;
        case 't':
            *result = tan(num1);
            push_num(nums, top, num2);
            break;
        case 'g':
            *result = 1 / tan(num1);
            push_num(nums, top, num2);
            break;
        case 'q':
            *result = sqrt(num1);
            push_num(nums, top, num2);
            break;
        case 'l':
            *result = log(num1);
            push_num(nums, top, num2);
            break;
        default:
            res = 0;
    }
    return res;
}