#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdio.h>


int main() {
    uint32_t T[64];
    printf("T array: \n");
    for (int i=1; i <= 64; i++){
        T[i] = (uint32_t) (4294967296.0 * fabs(sin(i)));
        printf("0x%08x\n", T[i]);
    }
    return 0;
}
