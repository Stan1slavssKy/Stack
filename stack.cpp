#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <math.h>

#define STACK_OVERFLOW  -100
#define STACK_UNDERFLOW -101
#define OUT_OF_MEMORY   -102

#define INITIAL_SIZE 10

typedef double elem_t;

struct stack
{
    elem_t *data;
    size_t  size;
    size_t   top;
};

struct stack* stack_create ();
void   stack_destruct ();
void   push ();
void   pop ();

struct stack* stack_create ()
{
    struct stack* out = calloc (1, sizeof (struct stack));
    
    assert (out != NULL);
    
    out -> size = INITIAL_SIZE;
    out -> data = calloc (out -> size, sizeof (elem_t));
    
    assert ((out -> data) != NULL);
    
    out -> top = 0;
    return out;
}

void stack_destruct (struct stack **stack) 
{
    free ((*stack) -> data);
    free (*stack);
    *stack = NULL;
}

int main()
{
    struct stack* heap = stack_create ();

    stack_destruct (&heap);
}
