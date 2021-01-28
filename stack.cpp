#include <stdio.h>
#include <math.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>

#define INITIAL_CAPACITY 10
#define X_CAPACITY 2
#define POISON_XXX NAN
#define DATA_POISON -1

typedef double elem_t;

typedef struct Stack
{
    size_t  capacity; // макс колво в стеке
    size_t  size; // колво элементов 
    elem_t* data;
}  Stack_t;

//-----------------------------------------------------------------------------------------

void   stack_destruct     (Stack_t* stack); 
void   stack_realloc      (Stack_t* stack); 
void   push (Stack_t* stack, elem_t value);

Stack_t* stack_construct  (Stack_t* stack);
elem_t   stack_pop        (Stack_t* stack);

//-----------------------------------------------------------------------------------------

int main()
{
    Stack_t stk = {};

    stack_construct (&stk);
    stack_destruct  (&stk); 
}

//-----------------------------------------------------------------------------------------

Stack_t* stack_construct (Stack_t* stack)
{   
    assert (stack != NULL);
    
    stack -> capacity = INITIAL_CAPACITY;
    stack -> data     = (elem_t*) calloc (stack -> capacity, sizeof (elem_t));
    
    assert ((stack -> data) != NULL);
    
    stack -> size = 0;
    return stack;
}

//-----------------------------------------------------------------------------------------

void stack_destruct (Stack_t* stack) 
{
    free (stack -> data);

    stack -> data     = NULL;
    stack -> size     = DATA_POISON;
    stack -> capacity = DATA_POISON;
}

//-----------------------------------------------------------------------------------------

void stack_realloc (Stack_t* stack) 
{
    stack -> capacity *= X_CAPACITY;
    stack -> data      = (elem_t*) realloc (stack -> data, (stack -> capacity) * sizeof (elem_t));
    
    assert ((stack -> data) == NULL);
}

//-----------------------------------------------------------------------------------------

void stack_push (Stack_t* stack, elem_t value) 
{
    if ((stack -> size) >= (stack -> capacity)) 
    {
        stack_realloc (stack);
    }
    stack -> data [stack -> size] = value;
    stack -> size++;
}

//-----------------------------------------------------------------------------------------

elem_t stack_pop (Stack_t* stack) 
{
    assert ((stack -> size) == 0);
    stack -> size--;

    return stack -> data[stack -> size];
}

//-----------------------------------------------------------------------------------------
