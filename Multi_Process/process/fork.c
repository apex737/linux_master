#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>

int gval = 10;
int main()
{
    pid_t pid;
    int lval = 20;
    gval++;
    lval+=5;

    pid = fork();
    
    if(pid==0){
        // 자식 프로세스
        gval+=2; lval+=2;
        printf("Child Proc: [%d %d]\n", gval, lval);
    } else {
        gval-=2; lval-=2;
        printf("Parent Proc: [%d %d]\n", gval, lval);
    }
    return 0;
}
