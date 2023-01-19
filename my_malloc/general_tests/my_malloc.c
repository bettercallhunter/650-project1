#ifndef __MALLOC_C
#define __MALLOC_C



// First Fit malloc/free
#include "my_malloc.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct memory_blck_t memory_blck;
memory_blck *head;
memory_blck *tail;

void *ff_malloc(size_t size) {
    /*find the apporarite location*/
    memory_blck **curr = &head;
    memory_blck *ptr;

    while (*curr != NULL) {
        if ((*curr)->size >= size && (*curr)->used == 0) {
            break;
        } else {
            curr = &((*curr)->next);
        }
    }
    if (*curr != NULL) {
        (*curr)->used = 1;
    } else {
        *curr = sbrk(size + sizeof(memory_blck));
        (*curr)->size = size;
        (*curr)->next = NULL;
        (*curr)->prev = tail;
        (*curr)->used = 1;
        tail = *curr;
    }
    return *curr + 1;
};
void ff_free(void *ptr) {
    memory_blck *curr = (memory_blck *)(ptr - sizeof(memory_blck));
    curr->used = 0;
    if (curr->prev && curr->prev->used == 0) {
        mergeFront(curr);
        curr = curr->prev;
    }
    if (curr->next && curr->next->used == 0) {
        mergeBack(curr);
    }
}
void mergeFront(memory_blck *curr) {
    memory_blck *temp = curr->prev;
    temp->size += curr->size;
    curr->prev->next = curr->next;
    if(curr->next){
      curr->next->prev = temp;
    }
}
void mergeBack(memory_blck *curr) {
    memory_blck *temp = curr->prev;
    curr->next->size += curr->size;
    if (curr->prev){
      curr->prev->next = curr->next;
    }
    curr->next->prev = temp;
}

#endif
