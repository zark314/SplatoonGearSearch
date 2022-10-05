#include "utils.h"

void printfb(unsigned int a) {
    int i = 0;
    while (1)
    {
        printf("%d", a % 2);
        i++;
        a = a / 2;
        if (a == 0)
            break;
    }
    printf("\n");
    printf("%d\n", i);
}