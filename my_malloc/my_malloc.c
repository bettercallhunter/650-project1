#ifndef __MALLOC_C
#define __MALLOC_C
#include "my_malloc.h"

#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
unsigned long heap_size = 0;
void *ff_malloc(size_t size) {
    Node *curr = head;
    while (curr != NULL) {
        // find the next Node that is at least larger than size
        if (curr->size >= size) {
            break;
        } else {
            // traversal through the linkedlist
            curr = curr->next;
        }
    }
    // if no Node satisfied found, allocate new space
    if (curr == NULL) {
        void *ptr = sbrk(Meta_size + size);
        heap_size += Meta_size + size;
        if (ptr == (void *)-1) {
            return NULL;
        }
        Node *requestedSpace = (Node *)ptr;
        requestedSpace->prev = NULL;
        requestedSpace->next = NULL;
        requestedSpace->size = size;
        // // add a offset of size of Meta data
        return (void *)((char *)requestedSpace + Meta_size);
    }
    // enough space to split : size > meta data and allocation size
    if (curr->size > size + Meta_size) {
        Node *allocatedSpace = (Node *)((char *)curr + (curr->size) - size);
        curr->size -= Meta_size + size;
        allocatedSpace->size = size;
        allocatedSpace->prev = NULL;
        allocatedSpace->next = NULL;
        return (void *)((char *)allocatedSpace + Meta_size);
    }
    // space enough to allocate, but not enough to hold meta data, no split needed, remove node from the linkedlist
    else if (curr->size >= size && curr->size - size <= Meta_size) {
        removeNode(curr);
        return (char *)curr + Meta_size;
    }
    return NULL;
}
void *bf_malloc(size_t size) {
    Node *curr = head;
    Node *best = NULL;
    size_t overHead = SIZE_MAX;
    while (curr != NULL) {
        // find the next Node that is at least larger than size
        // update best if find a better fit
        if (curr->size >= size && curr->size - size <= overHead) {
            best = curr;
            overHead = curr->size - size;
            // traversal through the linkedlist
            if (overHead == 0) {
                removeNode(curr);
                return (char *)curr + Meta_size;
            }
        }
        curr = curr->next;
    }
    curr = best;
    // if no Node satisfied found, allocate new space
    if (curr == NULL) {
        void *ptr = sbrk(Meta_size + size);
        heap_size += Meta_size + size;
        if (ptr == (void *)-1) {
            return NULL;
        }
        Node *requestedSpace = (Node *)ptr;
        requestedSpace->prev = NULL;
        requestedSpace->next = NULL;
        requestedSpace->size = size;
        // add a offset of size of Meta data
        return (void *)((char *)requestedSpace + Meta_size);
    }
    // enough space to split : size > meta data and allocation size
    if (curr->size > size + Meta_size) {
        Node *allocatedSpace = (Node *)((char *)curr + (curr->size) - size);
        curr->size -= Meta_size + size;
        allocatedSpace->size = size;
        allocatedSpace->prev = NULL;
        allocatedSpace->next = NULL;
        return (char *)allocatedSpace + Meta_size;
    }
    // space enough to allocate, but not enough to hold meta data, no split needed, remove node from the linkedlist
    else if (curr->size >= size && curr->size - size <= Meta_size) {
        removeNode(curr);
        return (void *)((char *)curr + Meta_size);
    }
    return NULL;
}

void ff_free(void *ptr) {
    my_free(ptr);
}
void bf_free(void *ptr) {
    my_free(ptr);
}
void merge(Node *toAdd) {
    mergeBack(toAdd);
    mergeFront(toAdd);
}
void mergeBack(Node *toAdd) {
    if (toAdd->next && (char *)toAdd + Meta_size + toAdd->size == (char *)toAdd->next) {
        // modify size of the Node
        toAdd->size += Meta_size + toAdd->next->size;
        // if nextNode is not tail, need modify next next

        if (toAdd->next->next) {
            toAdd->next->next->prev = toAdd;
        } else {
            tail = toAdd;
        }
        toAdd->next = toAdd->next->next;
    }
}
void mergeFront(Node *toAdd) {
    // if pointer is adjecent to prev Node
    if (toAdd->prev && (char *)toAdd->prev + toAdd->prev->size + Meta_size == (char *)toAdd) {
        toAdd->prev->size += toAdd->size + Meta_size;
        toAdd->prev->next = toAdd->next;
        if (toAdd->next) {
            toAdd->next->prev = toAdd->prev;
        } else {
            tail = toAdd;
        }
    }
}
void removeNode(Node *curr) {
    if (curr->prev) {
        (curr)->prev->next = curr->next;
    } else {
        head = curr->next;
    }
    if (curr->next) {
        curr->next->prev = curr->prev;
    } else {
        tail = curr->prev;
    }
}
unsigned long get_data_segment_size() {
    return heap_size;
}
unsigned long get_data_segment_free_space_size() {
    Node *curr = head;
    unsigned long size = 0;
    while (curr) {
        size += curr->size + Meta_size;
        curr = curr->next;
    }
    return size;
}
void my_free(void *ptr) {
    if (!ptr) {
        return;
    }
    // minus the offset of a meta_size, now pointer point at Node
    Node *pointer = (Node *)((char *)ptr - Meta_size);
    if (!pointer) {
        return;
    }
    // add Node
    Node *curr = head;
    while (curr) {
        // if curr smaller than the new freed block, we move to next
        if (curr > pointer) {
            break;
        }
        curr = curr->next;
    }
    addNode(curr, pointer);
}
void addNode(Node *curr, Node *toAdd) {
    // if cannot find Node to add before
    if (!curr) {
        // if linkedlist is empty
        if (!head) {
            head = toAdd;
            tail = toAdd;
            toAdd->next = NULL;
            toAdd->prev = NULL;
        }
        // if add at tail, only merge with front Node
        else {
            tail->next = toAdd;
            toAdd->prev = tail;
            toAdd->next = NULL;
            tail = toAdd;
        }
    }
    // add Node right before curr
    else {
        // if curr is head
        if (!curr->prev) {
            head = toAdd;
            toAdd->next = curr;
            toAdd->prev = NULL;
            curr->prev = head;
        }
        // if curr not head
        else {
            toAdd->next = curr;
            toAdd->prev = curr->prev;
            toAdd->prev->next = toAdd;
            curr->prev = toAdd;
        }
        // check if to toAdd can merge with adjecent Node. Merge if needed
        merge(toAdd);
        return;
    }
}

#endif
