#include "genericList.h"

List* list_alloc(void)
{
    List theList = malloc(sizeof(List));
    theList->first= NULL;
    theList->last = NULL;
    theList->size = 0;
    return theList;
}

void add(void *data, List *list)
{
    Node *newNode = malloc(sizeof(Node));
    newNode->next = NULL;
    newNode->data = data;
}

Node* list_get_node(void *data, List *list);

void main (int argc, char*argv[]) 
{

    List symbolTable = list_alloc();
    list_get_node("hello", symbolTable)->data->
    add("hello", symbolTable);

}
