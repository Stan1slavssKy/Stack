#ifndef PROTECTION_H_INCLUDED
#define PROTECTION_H_INCLUDED

#include "stack.h"

#define DATA_POISON -1
#define DOUBLE_TYPE

#ifdef DOUBLE_TYPE
    #define POISON_XXX NAN
#endif

#ifdef INT_TYPE
    #define POISON_XXX 0xBAADEDAA
#endif
//-----------------------------------------------------------------------------------------

#define assert_ok(stack) if (stack_ok(stack))     \
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

void stack_dump       (Stack_t* stack);
void stack_null       (Stack_t* stack);
void poison_fill_in   (Stack_t* stack, size_t beg, size_t end);
void placing_canaries (Stack_t* stack, void* memory);

const char* error_detect  (Stack_t* stack);

//-----------------------------------------------------------------------------------------

#endif
