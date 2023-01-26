

## Study Report
Implementation of Malloc and Free


## Creators

**Hunter**
<br>
Netid: ZS149

## What's included


```text
Mymalloc/
    |
    └── my_malloc.c
    └── my_malloc.h
    └── Makefile
    └── libmymalloc.so

```
## Implementation  for Malloc

## struct
to store the meta data, we firstly need to create a struct. Let's call it a Node.
<code>
struct Node_t {
    size_t size;
    struct Node_t *next;
    struct Node_t *prev;
};
typedef struct Node_t Node;
Node *head;
Node *tail;
</code>
To achieve the function of Malloc, the following function was used
<li>void removeNode(Node *curr);</li>

