#ifndef STACK_H_INCLUDED
#define STACK_H_INCLUDED    

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <math.h>

#define INITIAL_CAPACITY 10
#define X_CAPACITY 2

//----------------------------------------------------------

#define DOUBLE_TYPE

#ifdef DOUBLE_TYPE
    typedef double elem_t;
#endif

#ifdef INT_TYPE
    typedef int elem_t;
#endif

//----------------------------------------------------------

typedef unsigned long long canary_t;
const canary_t Canary = 0xFADEBEEF;

//----------------------------------------------------------

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

//----------------------------------------------------------

Stack_t* stack_construct  (Stack_t* stack, long capacity);

void     stack_realloc    (Stack_t* stack); 
void     stack_push       (Stack_t* stack, elem_t value);

elem_t   stack_pop        (Stack_t* stack);

void     stack_dump       (Stack_t* stack);
void     stack_destruct   (Stack_t* stack); 

//----------------------------------------------------------

#endif