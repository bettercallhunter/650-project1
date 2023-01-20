#ifndef __MALLOC_H
#define __MALLOC_H
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define Meta_size sizeof(Node)
struct Node_t {
    size_t size;
    struct Node_t *next;
    struct Node_t *prev;

    /* data */
};
typedef struct Node_t Node;
Node *head;
Node *tail;

void *ff_malloc(size_t size);
void ff_free(void *ptr);
void merge(Node *curr);
void removeNode(Node *curr);
Node *split(Node *curr, size_t size);
size_t get_data_segment_size();

size_t get_data_segment_free_space_size();
#endif