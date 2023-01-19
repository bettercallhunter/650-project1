#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
int main() {
    int* x;
    int* y;
    int* z;
    int* o;
    o = sbrk(0);
    printf("initialization is %p \n", (o));
    x = sbrk(sizeof(int));
    y = sbrk(sizeof(int));
    z = sbrk(sizeof(int));
    *x = 1;
    *y = 2;
    *z = 3;
    printf("x = %p,y = %p,z = %p \n", x, y, z);
    printf("now freeing\n");
    void* j = sbrk(-4);
    printf("j = %p", j);
    printf("\n **************\n");
    void* k = sbrk(0);
    printf("k = %p \n", k);
    return 1;
}
