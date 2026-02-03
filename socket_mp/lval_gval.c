#include "def.h"

// 지역변수든, 전역변수든 Copy on Write에 의해 각 프로세스만의 값으로 변한다.
int gval  = 30;
int main()
{
    int lval  = 10;
    pid_t pid;
    switch (pid = fork())
    {
    case -1:
        error_handler("fork");
    case 0:
        puts("child process");
        lval += 5;
        gval -= 5;
        printf("lval: %d, gval: %d\n", lval, gval);
        break;
    
    default:
    puts("parent process");
        lval += 15;
        gval -= 15;
        printf("lval: %d, gval: %d\n", lval, gval);
        break;
    }
    return 0;
}