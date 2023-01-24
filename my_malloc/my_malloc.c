#ifndef __MALLOC_C
#define __MALLOC_C

// First Fit malloc/free
#include "my_malloc.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
size_t heap_size = 0;
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
        return (void *)requestedSpace + Meta_size;
    }

    // enough space to split : size > meta data and allocation size
    if (curr->size > size + Meta_size) {
        Node *allocatedSpace = (Node *)((void *)curr + (curr->size) - size);
        curr->size -= Meta_size + size;
        allocatedSpace->size = size;
        allocatedSpace->prev = NULL;
        allocatedSpace->next = NULL;
        return (void *)allocatedSpace + Meta_size;
    }
    // space enough to allocate, but not enough to hold meta data, no split needed, remove node from the linkedlist
    else if (curr->size >= size && curr->size - size < Meta_size) {
        removeNode(curr);
        return (void *)curr + Meta_size;
    }
}

void ff_free(void *ptr) {
    if (!ptr) {
        return;
    }
    // minus the offset of a meta_size, now pointer point at Node
    Node *pointer = ptr - Meta_size;
    heap_size += pointer->size;
    // add Node
    Node *curr = head;
    while (curr) {
        // if curr smaller than the new freed block, we move to next
        curr = curr->next;
        if (curr > pointer) {
            break;
        }
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
            mergeFront(toAdd);
        }
    }
    // add Node right before curr
    if (curr) {
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
void merge(Node *toAdd) {
    mergeBack(toAdd);
    mergeFront(toAdd);
}
void mergeBack(Node *toAdd) {
    if (toAdd->next && (void *)toAdd + Meta_size + toAdd->size == (void *)toAdd->next) {
        // modify size of the Node
        toAdd->size += Meta_size + toAdd->next->size;
        // if nextNode is not tail, need modify next next
        if (toAdd->next->next) {
            toAdd->next->next->prev = toAdd;
        }
        toAdd->next = toAdd->next->next;
    }
}
void mergeFront(Node *toAdd) {
    // if pointer is adjecent to prev Node
    if (toAdd->prev && (void *)toAdd->prev + toAdd->prev->size + Meta_size == (void *)toAdd) {
        toAdd->prev->size += toAdd->size + Meta_size;
        toAdd->prev->next = toAdd->next;
        if (toAdd->next) {
            toAdd->next->prev = toAdd->prev;
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

size_t get_data_segment_size() {
    return heap_size;
}
size_t get_data_segment_free_space_size() {
    Node *curr = head;
    size_t size = head->size;
    while (curr) {
        size += curr->size + Meta_size;
        curr = curr->next;
    }
    return size;
}
#endif
