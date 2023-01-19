#ifndef __MALLOC_H
#define __MALLOC_H
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
struct memory_blck_t {
    size_t size;
    int used;
    struct memory_blck_t *next;
    struct memory_blck_t *prev;

    /* data */
};
typedef struct memory_blck_t memory_blck;
void *ff_malloc(size_t size);
void ff_free(void *ptr);
void mergeBack(memory_blck *curr);
void mergeFront(memory_blck *curr);

#endif