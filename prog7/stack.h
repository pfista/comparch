#include "global.h"

#define STACK_SIZE 5

typedef struct Stack
{
    char c[STACK_SIZE];
    int top;

}Stack;

void push (Stack *s, char c);
char pop (Stack *s);
void init (Stack *s);
Boolean full (Stack *s);
Boolean empty (Stack *s);
void print(Stack *S);
