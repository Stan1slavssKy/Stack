#include "protection.h"

int main(/*int argc, char** argv*/)
{
    // STACK CONSTRUCT ON EXISTING STACK
    stack_constr (stk, 0);

    for (int i = 0; i < 15; i++)
        stack_push (&stk, i);

    stack_dump (&stk);
   // stk.data = nullptr;
    stack_destruct  (&stk); 
}
