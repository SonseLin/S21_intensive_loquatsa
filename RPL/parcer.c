#include "parcer.h"

int is_number(char c) { return c >= 48 && c <= 57; }
int is_number_or_x(char c) { return (c >= 48 && c <= 57) || c == 120; }
int is_number_or_dot_or_x(char c) { return (c >= 48 && c <= 57) || c == 46 || c == 120; }

char* input_string(void) {
    char* input = malloc(sizeof(char) * 256);
    scanf("%255[^\n]s", input);
    return input;
}

int op_weight(char op) {
    int result = -1;
    switch (op) {
        case '+':
        case '-':
            result = 1;
            break;
        case '*':
        case '/':
            result = 2;
            break;
        case SIN:
        case COS:
        case TAN:
        case CTG:
        case SQRT:
        case LN:
            result = 3;
            break;
    }
    return result;
}

int is_op(char ch) { return (ch == '+' || ch == '-' || ch == '*' || ch == '/'); }

int is_func(const char* input, const char* current, int len) {
    int result = 0;
    if (is_lexema(current, input, len, "sin", 3)) result = SIN;
    if (is_lexema(current, input, len, "cos", 3)) result = COS;
    if (is_lexema(current, input, len, "tan", 3)) result = TAN;
    if (is_lexema(current, input, len, "ctg", 3)) result = CTG;
    if (is_lexema(current, input, len, "sqrt", 4)) result = SQRT;
    if (is_lexema(current, input, len, "ln", 2)) result = LN;
    return result;
}

int is_lexema(const char* current, const char* input, int len, const char* lexema, int lex_len) {
    int res = 1;
    if (current + lex_len - 1 < input + len) {
        for (int i = 0; i < lex_len; i++) {
            if (lexema[i] != current[i]) {
                res = 0;
            }
        }
    } else {
        res = 0;
    }
    return res;
}

char* convert(const char* input, int len, int* no_err) {
    char* result = malloc((len * 2) * sizeof(char));
    Stack stack;
    stack.top = -1;
    int j = 0;
    int check = 0;

    const char* current = input;

    while (*current != '\0' && *no_err) {
        if (is_number_or_x(*current)) {
            while (is_number_or_dot_or_x(*current)) {
                result[j++] = *current++;
                check = 1;
            }
            current--;
        } else if (*current == '(') {
            push(&stack, *current);
        } else if (*current == ')') {
            while (!is_empty(&stack) && pop(&stack) != '(') {
                result[j++] = pop(&stack);
                result[j++] = ' ';
            }
            if (!is_empty(&stack) && pop(&stack) != '(') {
                *no_err = 0;
            } else {
                pop(&stack);
            }
        } else if (is_op(*current) || is_func(input, current, len)) {
            int func = is_func(input, current, len);
            while (!func && !is_empty(&stack) && op_weight(pop(&stack)) >= op_weight(*current)) {
                result[j++] = pop(&stack);
                result[j++] = ' ';
            }
            push_all_possible_op(func, &stack, current, &current);
        } else {
            *no_err = 0;
        }
        if (check) {
            result[j++] = ' ';
            check = 0;
        }
        current++;
    }

    while (!is_empty(&stack)) {
        result[j++] = pop(&stack);
        result[j++] = ' ';
    }
    result[j] = '\0';

    return result;
}

void push_all_possible_op(int func, Stack* stack, const char* current, const char** input) {
    if (!func) {
        push(stack, *current);
    } else if (func == SIN) {
        push(stack, 's');
        *input += 2;
    } else if (func == COS) {
        push(stack, 'c');
        *input += 2;
    } else if (func == TAN) {
        push(stack, 't');
        *input += 2;
    } else if (func == CTG) {
        push(stack, 'g');
        *input += 2;
    } else if (func == SQRT) {
        push(stack, 'q');
        *input += 3;
    } else if (func == LN) {
        push(stack, 'l');
        *input += 1;
    }
}