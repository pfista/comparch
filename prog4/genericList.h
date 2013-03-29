#include <stdio.h>
#include <stdlib.h>

typedef struct List
{
    node *first, *last;
    int size;
}List;

typedef struct Node 
{
    struct Node *next;
    void *data;
}Node;

List* list_alloc(void);
void add(void *data, List *list);
void removeData(void *data, List *list);
Node* list_get_node(void *data, List *list);
