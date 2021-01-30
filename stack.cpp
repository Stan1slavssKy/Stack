#include <stdio.h>
#include <math.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>

#define INITIAL_CAPACITY 10
#define X_CAPACITY 2
#define DATA_POISON -1

#define DOUBLE_TYPE

#ifdef DOUBLE_TYPE
    typedef double elem_t;
    #define POISON_XXX NAN
#endif

typedef struct Stack
{
    size_t  capacity; // макс колво в стеке
    size_t  size;     // колво элементов 
    elem_t* data;
    size_t  hash;
}  Stack_t;

//-----------------------------------------------------------------------------------------

Stack_t* stack_construct  (Stack_t* stack);
elem_t   stack_pop        (Stack_t* stack);

void     stack_verificate (Stack_t* stack);
void     stack_destruct   (Stack_t* stack); 
void     stack_realloc    (Stack_t* stack); 

void  stack_push (Stack_t* stack, elem_t value);

//-----------------------------------------------------------------------------------------

int main()
{
    Stack_t stk = {};

    stack_construct (&stk);
    
    stack_push (&stk, 10);
    stack_push (&stk, 11);
  /*  stack_push (&stk, 12);
    stack_push (&stk, 13);
    stack_push (&stk, 14);
    stack_push (&stk, 15);
    stack_push (&stk, 16);
    stack_push (&stk, 17);
    stack_push (&stk, 18);
    stack_push (&stk, 19);
    stack_push (&stk, 20);
    stack_push (&stk, 21);
    printf ("%f", stack_pop  (&stk));*/

    for (int i = 0; i < stk.capacity; i++) printf ("%f\n", *(stk.data + i));

    stack_destruct  (&stk); 
}

//-----------------------------------------------------------------------------------------

Stack_t* stack_construct (Stack_t* stack)
{   
    assert (stack != NULL);
    
    stack -> capacity = INITIAL_CAPACITY;
    stack -> data     = (elem_t*) calloc (stack -> capacity, sizeof (elem_t));
    
    assert ((stack -> data) != NULL);
    
    for (int i = 0; i < stack -> capacity; i++) *(stack -> data + i) = POISON_XXX;

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
    stack_verificate (stack);
    
    size_t old_capacity = stack -> capacity;

    stack -> capacity *= X_CAPACITY;
    stack -> data      = (elem_t*) realloc (stack -> data, (stack -> capacity) * sizeof (elem_t));
    
    for (int i = old_capacity; i < stack -> capacity; i++) *(stack -> data + i) = POISON_XXX;

    assert ((stack -> data) != NULL);
}

//-----------------------------------------------------------------------------------------

void stack_push (Stack_t* stack, elem_t value) 
{
    assert (stack != NULL);
    assert ((stack -> data) != 0);

    if ((stack -> size + 1) >= (stack -> capacity)) 
    {
        stack_realloc (stack);
    }

    stack -> data [stack -> size] = value;
    stack -> size++;
}

//-----------------------------------------------------------------------------------------

elem_t stack_pop (Stack_t* stack) 
{
    stack_verificate (stack);

    elem_t out = *(stack -> data + stack -> size);
    *(stack -> data + stack -> size) = POISON_XXX; 

    stack -> size--;

    return out;
}

//-----------------------------------------------------------------------------------------

void stack_verificate (Stack_t* stack)
{
    assert (stack != NULL);
    assert ((stack -> size) != 0);
    assert ((stack -> data) != 0);
}
