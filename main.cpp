#include <protection.h>

int main()
{
    Stack_t stk = {};

    stack_construct (&stk, 1);

    for (int i = 0; i < 10; i++)
        stack_push (&stk, 10);

    stack_dump (&stk);

    stack_destruct  (&stk); 
}
