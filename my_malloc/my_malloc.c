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
        if (curr->size > size) {
            break;
        } else {
            // traversal through the linkedlist
            curr = curr->next;
        }
    }
    // if no Node satisfied found, allocate new space
    if (curr == NULL) {
        heap_size -= size;
        void *ptr = sbrk(Meta_size + size);
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
        heap_size -= size + Meta_size;
        Node *allocatedSpace = (Node *)((void *)curr + (curr->size) - size);
        curr->size -= Meta_size + size;
        allocatedSpace->size = size;
        allocatedSpace->prev = NULL;
        allocatedSpace->next = NULL;
        return (void *)allocatedSpace + Meta_size;
    }
    // space enough to allocate, but not enough to hold meta data, no split needed, remove node from the linkedlist
    else if (curr->size >= size && curr->size - size <= Meta_size) {
        heap_size -= curr->size;
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
    while (curr && curr < pointer) {
        // if curr smaller than the new freed block, we move to next

        curr = curr->next;

        // we find the right location, break
    }

    // now add node
    // if curr is null, add at tail
    if (!curr) {
        pointer->prev = tail;
        curr = pointer;
        pointer->next = NULL;
        tail = pointer;
    } else {
        pointer->next = curr;
        pointer->prev = (curr)->prev;
        (curr)->prev = pointer;
        curr = pointer;
    }
    merge(pointer);
    return;
}

void merge(Node *pointer) {
    // if pointer is adjecent to prev Node
    if (pointer->prev && (pointer->prev + Meta_size + pointer->prev->size == pointer)) {
        heap_size += Meta_size;
        pointer->prev->size += pointer->size;
        removeNode(pointer);
        pointer = pointer->prev;
    }

    if (pointer + Meta_size + pointer->size == pointer->next) {
        heap_size += Meta_size;
        pointer->size += pointer->next->size;
        removeNode(pointer->next);
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
    Node **curr = &head;
    size_t size = 0;
    while (*curr) {
        size += (*curr)->size;
        curr = &((*curr)->next);
    }
    printf("%ld", size);
    return size;
}
#endif
