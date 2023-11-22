#include "stack.h"

void push(Stack* stack, char item) {
    if (stack->top == STACK_SIZE - 1) {
        printf("Stack overflow!\n");
    }
    stack->data[++(stack->top)] = item;
}

char pop(Stack* stack) {
    if (stack->top == -1) {
        printf("Stack underflow!\n");
    }
    return stack->data[(stack->top)--];
}

// char top_stack(Stack* stack) {
//     if (stack->top == -1) {
//         printf("Stack is empty!\n");
//         return '0';
//     }
//     return stack->data[stack->top];
// }

int is_empty(Stack* stack) { return stack->top == -1; }

void push_num(double* stack, int* top, double x) {
    *top = *top + 1;
    stack[*top] = x;
}

double pop_num(const double* stack, int* top) {
    double x = stack[*top];
    *top = *top - 1;
    return x;
}