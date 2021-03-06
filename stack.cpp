#include <stdio.h>
#include <math.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>

//-----------------------------------------------------------------------------------------

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

#define assert_ok(stack) if (stack_ok(stack))            \
                                {                        \
                                    stack_dump (stack);  \
                                    abort ();            \
                                }                        \

//-----------------------------------------------------------------------------------------

#define error_case(enum_particle) case enum_particle:        \
                                      return #enum_particle; \

//-----------------------------------------------------------------------------------------

enum error
{
    MEMORY_OUT = 1,
    NULL_DATA,
    NEGATIVE_SIZE,
    NEGATIVE_CAPACITY,
    WRONG_CANARY_STRUCT_LEFT,
    WRONG_CANARY_STRUCT_RIGHT,
    WRONG_CANARY_ARRAY_LEFT,
    WRONG_CANARY_ARRAY_RIGHT,
};

//-----------------------------------------------------------------------------------------

typedef unsigned long long canary_t;

const canary_t Canary = 0xFADEBEEF;

typedef struct Stack
{
    canary_t left_struct_canary;

    size_t  capacity; // макс колво в стеке
    size_t  size; // колво элементов 

    elem_t* data;
    
    int error;

    size_t  struct_hash;
    size_t  stack_hash;

    canary_t right_struct_canary;
}  Stack_t;

//-----------------------------------------------------------------------------------------

Stack_t* stack_construct  (Stack_t* stack, long capacity);

elem_t   stack_pop        (Stack_t* stack);
int      stack_ok         (Stack_t* stack);
void     stack_destruct   (Stack_t* stack); 
void     stack_realloc    (Stack_t* stack); 
void     stack_null       (Stack_t* stack);
void     stack_dump       (Stack_t* stack);

void     stack_push       (Stack_t* stack, elem_t value);
void     poison_fill_in   (Stack_t* stack, size_t beg, size_t end);
void     placing_canaries (Stack_t* stack, void* memory);

const char* error_detect  (Stack_t* stack);

//-----------------------------------------------------------------------------------------

int main()
{
   
    Stack_t stk = {};

    stack_construct (&stk, 1);

    for (int i = 0; i < 10; i++)
        stack_push (&stk, 10);

    stack_dump (&stk);

    stack_destruct  (&stk); 
}

//-----------------------------------------------------------------------------------------

Stack_t* stack_construct (Stack_t* stack, long capacity)
{   
    stack_null (stack);

    if (capacity == 0)
    {
        stack -> capacity = (size_t) capacity;
        stack -> size = 0;
        stack -> data = NULL;
        stack -> left_struct_canary  = Canary;
        stack -> right_struct_canary = Canary;

        stack_realloc (stack);
    }

    else if (capacity > 0)
    {
        stack -> capacity = (size_t) capacity;
        stack -> size = 0;
        stack -> data = NULL;
        stack -> left_struct_canary  = Canary;
        stack -> right_struct_canary = Canary;
        
        void* memory = calloc (1, (stack -> capacity) * sizeof (elem_t) + 2 * sizeof (canary_t));
        
        if (memory == NULL)
        {
            stack -> error = MEMORY_OUT;
            assert_ok (stack);
        }

        placing_canaries (stack, memory);
        poison_fill_in   (stack, stack -> size, stack -> capacity);
    }

    else if (capacity < 0)
    {
        stack -> error = NEGATIVE_CAPACITY;
        assert_ok (stack);
    }

    return stack;
}

//-----------------------------------------------------------------------------------------

void stack_destruct (Stack_t* stack) 
{
    free ((canary_t*) (stack -> data) - 1);

    stack -> data                = NULL;       
    stack -> size                = DATA_POISON; 
    stack -> error               = 0;           
    stack -> capacity            = DATA_POISON;             
    stack -> left_struct_canary  = -Canary;  
    stack -> right_struct_canary = -Canary;
}

//-----------------------------------------------------------------------------------------

void stack_null (Stack_t* stack)
{
    if (stack == NULL)
    {
        printf ("STACK IS NULL!\n");
        abort ();
    }
}

//-----------------------------------------------------------------------------------------

void stack_realloc (Stack_t* stack) 
{
    stack_null (stack);

    if (stack -> capacity == 0)
    {
        stack -> capacity = INITIAL_CAPACITY;
        void* memory      = calloc (1, stack -> capacity * sizeof (elem_t) + 2 * sizeof (canary_t));

        if (memory == NULL)
        {
            stack -> error = MEMORY_OUT;
            assert_ok (stack);
        }

        placing_canaries (stack, memory);
        poison_fill_in   (stack, stack -> size, stack -> capacity);
    }

    else if (stack -> capacity == stack -> size)
    {
        size_t old_capacity = stack -> capacity;
        void* memory = realloc ((canary_t*)(stack -> data) - 1, (stack -> capacity *= X_CAPACITY) * sizeof (elem_t) + 2 * sizeof (canary_t));

        if (memory == NULL)
        {
            stack -> error = MEMORY_OUT;
            assert_ok (stack);
        }

        placing_canaries (stack, memory);
        poison_fill_in (stack, old_capacity, stack -> capacity);

        assert_ok (stack);
        
    }
}

//-----------------------------------------------------------------------------------------

void stack_push (Stack_t* stack, elem_t value) 
{
    assert_ok  (stack);
    stack_null (stack);

    if ((stack -> size + 1) > (stack -> capacity)) 
    {
        stack_realloc (stack);
    }


    *(stack -> data + stack -> size) = value;
    stack -> size++;
}

//-----------------------------------------------------------------------------------------

elem_t stack_pop (Stack_t* stack) 
{
    assert_ok  (stack);
    stack_null (stack);

    elem_t out = *(stack -> data + stack -> size - 1);

    poison_fill_in (stack, stack -> size - 1, stack -> size);

    stack -> size--;

    return out;
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

void stack_dump (Stack_t* stack)
{
    const char* text_of_error = error_detect (stack);

    canary_t* left_arr_can  = (canary_t*)(stack -> data) - 1;
    canary_t* right_arr_can = (canary_t*)(stack -> data + stack -> capacity);

    printf ("===================STACK DUMP===================\n");
    if (stack -> error != 0)
        printf ("Error №%d found in the stack: %s\n", stack -> error, text_of_error);
    else
        printf ("\t\t  Stack is OK!\n");
   
    printf ("Struct left  canary[%p] = %lld\n", &stack -> left_struct_canary, stack -> left_struct_canary);
    printf ("Stack  size                         = %ld\n", stack -> size) ;
    printf ("Stack  capacity                     = %ld\n", stack -> capacity);
    printf ("Array  left  canary[%p] = %lld\n", left_arr_can, *left_arr_can);
    printf ("Stack  data                     = %p\n", stack -> data);
    printf ("Array  right canary[%p] = %lld\n", right_arr_can, *right_arr_can);
    printf ("Struct right canary[%p] = %lld\n", &stack -> right_struct_canary, stack -> right_struct_canary);

    for (int i = 0; i < stack -> capacity; i++) printf ("\t\t[%d]: %f\n", i, *(stack -> data + i));
    printf ("================================================\n");
  /*  ЗЗЗЗЗЗЗЗЗЗЗЗЗЗЗЗЗАААААААААААААМММММММММММММЕЕЕЕЕЕЕЕЕЕЕЕЕННННННННННИИИИИИИИИИИИТТТТТТТТТТТТТТЬЬЬЬЬЬЬЬЬЬЬЬ
заменить %lld на define
*/
}

//-----------------------------------------------------------------------------------------

void placing_canaries (Stack_t* stack, void* memory)
{
    stack_null (stack);
    
    canary_t* canary_array_left = (canary_t*) memory;
    *canary_array_left = Canary;
        
    stack -> data = (elem_t*) (canary_array_left + 1); //кладёт указатель в структуру 
                        
    canary_t* canary_array_right = (canary_t*) (stack -> data + stack -> capacity);
    *canary_array_right = Canary;  
}

//-----------------------------------------------------------------------------------------

int stack_ok (Stack_t* stack)
{
    if (stack -> error != 0)
    {
        return stack -> error;
    }

    else
    {
        if (stack -> capacity != 0 && stack -> data == NULL)
        {
            stack -> error = NULL_DATA;
            return NULL_DATA;
        }
        
        if (stack -> size < 0)
        {
            stack -> error = NEGATIVE_SIZE;
        }

        if (stack -> left_struct_canary != Canary)
        {
            stack -> error = WRONG_CANARY_STRUCT_LEFT;
            return WRONG_CANARY_STRUCT_LEFT;
        }

        if (stack -> right_struct_canary != Canary)
        {
            stack -> error = WRONG_CANARY_STRUCT_RIGHT;
            return WRONG_CANARY_STRUCT_RIGHT;
        }

        if (*((canary_t*)(stack -> data) - 1) != Canary)
        {
            stack -> error = WRONG_CANARY_ARRAY_LEFT;
            return WRONG_CANARY_ARRAY_LEFT;
        }

        if (*((canary_t*)(stack -> data + stack -> capacity)) != Canary)
        {    
            stack -> error = WRONG_CANARY_ARRAY_RIGHT;
            return WRONG_CANARY_ARRAY_RIGHT;
        }
    }

    return 0;
}

//-----------------------------------------------------------------------------------------

const char* error_detect (Stack_t* stack)
{
    switch (stack -> error)
    {
        case 0:
            return "STACK_IS_OK";

        error_case (MEMORY_OUT);
        error_case (NEGATIVE_SIZE);
        error_case (NEGATIVE_CAPACITY);
        error_case (WRONG_CANARY_STRUCT_LEFT);
        error_case (WRONG_CANARY_STRUCT_RIGHT);
        error_case (WRONG_CANARY_ARRAY_LEFT);
        error_case (WRONG_CANARY_ARRAY_RIGHT);
        error_case (NULL_DATA);

        default:
            return "UNNOWN_ERROR!";
    }
}   
