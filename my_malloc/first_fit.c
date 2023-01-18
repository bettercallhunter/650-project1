// First Fit malloc/free
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

memory_blck *head;
memory_blck *tail;

void *ff_malloc(size_t size) {
    /*find the apporarite location*/
    if (!head) {
        memory_blck Node;
        Node.next = NULL;
        Node.prev = NULL;
        Node.size = size;
        Node.used = 1;
        head = &Node;
        tail = &Node;
        void *ptr = (void *)sbrk(size + sizeof(memory_blck));
        return ptr + sizeof(memory_blck);
    }
    for
        head
};
void ff_free(void *ptr);
int main() {
    int *x = ff_malloc(sizeof(int));
    *x = 8;
    printf("%d", (*x));
    return 0;
}