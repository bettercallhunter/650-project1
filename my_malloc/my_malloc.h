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
void *bf_malloc(size_t size);
void ff_free(void *ptr);
void my_free(void *ptr);
void bf_free(void *ptr);
void merge(Node *toAdd);
void mergeBack(Node *toAdd);
void mergeFront(Node *toAdd);
void removeNode(Node *curr);
size_t get_data_segment_size();
void addNode(Node *curr, Node *toAdd);
size_t get_data_segment_free_space_size();

#endif