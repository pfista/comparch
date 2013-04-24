#include "stack.h"

void push (Stack *s, char c)
{
    if (!full(s))
        s->c[(s->top)++] = c; // Add c to the top and increment top
    else {
        fprintf(stderr, "Stack is full, cannot push\n");
    }
}

char pop (Stack *s)
{
    if (!empty(s))
        return (s->c[--(s->top)]);
    else {
        fprintf(stderr, "Stack is empty, cannot pop\n");
        return EOF;
    }
}

Boolean full (Stack *s)
{
    if (s->top >= STACK_SIZE-1)
        return TRUE;
    else return FALSE;
}

Boolean empty (Stack *s)
{
    if (s->top == 0) return TRUE;
    else return FALSE;
}

void init (Stack *s)
{
    s->top = 0;
}


void print(Stack *S)
{
    int i;
    if (S->top == 0)
       printf("Stack is empty.\n");
    else
    {
       printf("Stack contents: ");
       for (i=0;i<S->top;i++)
       {
          printf("%c  ",S->c[i]); 
       }
       printf("\n");
    }
}
