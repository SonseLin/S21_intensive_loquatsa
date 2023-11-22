
#include <stdlib.h>
#include <string.h>

#include "paint.h"
#include "parcer.h"
#include "pl_math.h"

int main(void) {
    char* expr = input_string();
    if (expr != NULL) {
        int no_err = 1;
        char* notation = convert(expr, (int)strlen(expr), &no_err);
        if (notation != NULL && no_err) {
            printf("%s\n", notation);
            output_graph(notation, &no_err);
        } else {
            printf("n/a");
        }
        free(notation);
    } else {
        printf("n/a");
    }
    free(expr);
    return 0;
}