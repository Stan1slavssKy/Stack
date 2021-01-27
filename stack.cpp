#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <math.h>

#define INITIAL_CAPACITY 10
#define X_CAPACITY 2

typedef double elem_t;

typedef struct Stack
{
    size_t  capacity;
    size_t  size;
    elem_t* data;
} Stack_t;

Stack_t* stack_construct (Stack_t* heap);

void   stack_destruct (Stack_t **stack); 
void   stack_resize   (Stack_t* stack); 
void   push           (Stack_t* stack, elem_t value);

elem_t pop            (Stack_t* stack);
elem_t peek           (const Stack_t* stack); 

int main()
{
    Stack_t stk = {};
    stack_construct (&stk);
    stack_destruct  (&stk); 
}

Stack_t* stack_construct (Stack_t* heap)
{   
    assert (heap != NULL);
    
    heap -> capacity = INITIAL_CAPACITY;
    heap -> data     = (elem_t*) calloc (heap -> capacity, sizeof (elem_t));
    
    assert ((heap -> data) != NULL);
    
    heap -> size = 0;
    return heap;
}

void stack_destruct (Stack_t **stack) 
{
    free ((*stack) -> data);
    free (*stack);
    *stack = NULL;
}

void stack_resize (Stack_t* stack) 
{
    stack -> capacity *= X_SIZE;
    stack -> data  = (elem_t*) realloc (stack -> data, stack -> capacity * sizeof(elem_t));
    
    assert ((stack -> data ) == NULL);
}

void push (Stack_t* stack, elem_t value) 
{
    if ((stack -> size) >= (stack -> capacity)) 
    {
        resize (stack);
    }
    stack -> data [stack -> size] = value;
    (stack -> size)++;
}

elem_t pop (Stack_t* stack) 
{
    assert ((stack -> size) == 0);
    (stack -> size)--;

    return stack -> data[stack -> size];
}

elem_t peek (const Stack_t* stack) 
{
    assert ((stack -> size) <= 0);
    
    return stack -> data[stack -> size - 1];
}
