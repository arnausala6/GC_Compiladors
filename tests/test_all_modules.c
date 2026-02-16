#include <stdio.h>
#include "main.h"
FILE *ofile = NULL;

int main() {
    printf("Hello from sample input");
    int x = 10;
    int y = 20;
    int z = x + y;
    int i = y-1;
    printf("Result: %d", z);

    return 0;
}