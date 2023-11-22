#include "paint.h"

#include "pl_math.h"

void output_graph(char* notation, int* no_err) {
    double answer[WIDTH];
    int value = 0;
    for (double x = 0.0; x < 4.0 * M_PI || !*no_err; x += 4.0 * M_PI / WIDTH) {
        answer[value++] = notation_answer(notation, x, no_err);
    }
    if (*no_err) {
        for (double y = 1; y >= -1; y -= RANGE) {
            for (int x = 0; x < WIDTH; x++) {
                if (answer[x] <= y + POSSIBLE_RANGE && answer[x] >= y - POSSIBLE_RANGE) {
                    printf("*");
                } else {
                    printf(".");
                }
            }
            printf("\n");
        }
    }
}