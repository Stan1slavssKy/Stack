#include <stdio.h>
#include <math.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>

#define INITIAL_CAPACITY 10
#define X_CAPACITY 2
#define DATA_POISON -1

//-----------------------------------------------------------------------------------------

#define DOUBLE_TYPE

#ifdef DOUBLE_TYPE
    typedef double elem_t;
    #define POISON_XXX NAN
#endif

//-----------------------------------------------------------------------------------------

typedef unsigned long long canary_t;

const canary_t Canary = 0xFADEBEEF;

typedef struct Stack
{
    canary_t left_struct_canary;

    size_t  capacity; // макс колво в стеке
    size_t  size;     // колво элементов 
    elem_t* data;
    
    size_t  struct_hash;
    size_t  stack_hash;

    canary_t right_struct_canary;
}  Stack_t;

//-----------------------------------------------------------------------------------------

Stack_t* stack_construct  (Stack_t* stack, long capacity);
elem_t   stack_pop        (Stack_t* stack);

void     stack_verificate (Stack_t* stack);
void     stack_destruct   (Stack_t* stack); 
void     stack_realloc    (Stack_t* stack); 

void     stack_push       (Stack_t* stack, elem_t value);
void     stack_damp       (Stack_t  stack);
void     poison_fill_in   (Stack_t* stack, size_t beg, size_t end);

void     placing_canaries (Stack_t* stack, void* memory);

//-----------------------------------------------------------------------------------------

int main()
{
    Stack_t stk = {};

    stack_construct (&stk, 3);
    
    stack_push (&stk, 10);
    stack_push (&stk, 11);
    stack_push (&stk, 12);
    stack_push (&stk, 13);
    stack_push (&stk, 14);
    stack_push (&stk, 15);
    stack_push (&stk, 16);
    stack_push (&stk, 17);
    stack_push (&stk, 18);
    stack_push (&stk, 19);
    printf ("You took out the element from the top - %f\n", stack_pop (&stk));

    stack_damp (stk);

    stack_destruct  (&stk); 
}

//-----------------------------------------------------------------------------------------

Stack_t* stack_construct (Stack_t* stack, long capacity)
{   
    assert (stack != NULL);
    assert (capacity >= 0);

    if (capacity == 0)
    {
        stack -> capacity = (size_t) capacity;
        stack -> size = 0;
        stack -> data = NULL;
        stack -> left_struct_canary  = Canary;
        stack -> right_struct_canary = Canary;

        stack_realloc (stack);
    }

    else 
    {
        stack -> capacity = (size_t) capacity;
        stack -> size = 0;
        stack -> data = NULL;
        stack -> left_struct_canary  = Canary;
        stack -> right_struct_canary = Canary;
        
        poison_fill_in (stack, 0, stack -> capacity);
        
        void* memory = calloc (1, (stack -> capacity) * sizeof (elem_t) + 2 * sizeof (canary_t));
        assert (memory != NULL);

        placing_canaries (stack, memory);

        stack -> size = 0;
    }
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
    
    if (stack -> capacity = 0)
    {
        stack -> capacity = INITIAL_CAPACITY;
        stack -> data     = (elem_t*) calloc (stack -> capacity, sizeof (elem_t));
    
        poison_fill_in (stack, 0, stack -> capacity);
    }

    size_t old_capacity = stack -> capacity;

    stack -> capacity *= X_CAPACITY;
    stack -> data      = (elem_t*) realloc (stack -> data, (stack -> capacity) * sizeof (elem_t));
    
    poison_fill_in (stack, old_capacity, stack -> capacity);

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

    *(stack -> data + stack -> size) = value;
    stack -> size++;
}

//-----------------------------------------------------------------------------------------

elem_t stack_pop (Stack_t* stack) 
{
    stack_verificate (stack);

    elem_t out = *(stack -> data + stack -> size - 1);

    poison_fill_in (stack, stack -> size - 1, stack -> size);

    stack -> size--;

    return out;
}

//-----------------------------------------------------------------------------------------

void stack_verificate (Stack_t* stack)
{
    assert (stack != NULL);
    assert ((stack -> size) != 0);
    assert ((stack -> data) != NULL);
}

//-----------------------------------------------------------------------------------------

void poison_fill_in (Stack_t* stack, size_t beg, size_t end)
{
    for (size_t i = beg; i < end; i++)
    {
        *(stack -> data + i) = POISON_XXX;
    }
}

//-----------------------------------------------------------------------------------------

void stack_damp (Stack_t stack)
{
    //печать в консоль
    printf ("===========STACK DUMP===========\n");
    printf ("Stack size     = %ld\n", stack.size);
    printf ("Stack capacity = %ld\n", stack.capacity);
    printf ("Stack data     = %p\n", stack.data);
    printf ("Stack right now:    \n");

    for (int i = 0; i < stack.capacity; i++) printf ("\t%f\n", *(stack.data + i));
    printf ("================================\n");

    //печать в файл 
    FILE* file = fopen ("Stack dump.txt", "w");

    fprintf (file, "===========STACK DUMP===========\n");
    fprintf (file, "Stack capacity = %ld\n", stack.capacity);
    fprintf (file, "Stack size     = %ld\n", stack.size);
    fprintf (file, "Stack data     = %p\n", stack.data);
    fprintf (file, "Stack right now:    \n");

    for (int i = 0; i < stack.capacity; i++) fprintf (file, "\t%f\n", *(stack.data + i));
    fprintf (file, "================================\n");
}

//-----------------------------------------------------------------------------------------

void placing_canaries (Stack_t* stack, void* memory)
{
    assert (stack != NULL);
    
    canary_t* canary_array_left = (canary_t*) memory;
    *canary_array_left = Canary;
        
    stack -> data = (elem_t*) (canary_array_left + 1);
                        
    canary_t* canary_array_right = (canary_t*) (stack -> data + stack -> capacity);
    *canary_array_right = Canary;  
}
