#include "protection.h"

int main(/*int argc, char** argv*/)
{
    // STACK CONSTRUCT ON EXISTING STACK
    stack_constr (stk, 0);

    for (int i = 0; i < 11; i++)
    {
        stack_push (&stk, i);   
    }

    stack_dump (&stk);
    //stk.data = nullptr;
    stack_destruct  (&stk); 
}

/*TODO
1) защита от двойного конструктора/ деструктора 
2) убрать аборт
5) передавать режим защиты через аргументы командной строки
7) придумать нормальный хеш
8) что будет если передать &stk - 1 
*/