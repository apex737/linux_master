#include "../def.h"
#include <sys/wait.h>
int main()
{
    pid_t pid;
    int status;
    switch (pid = fork())
    {
    case -1:
        error_handler("fork()");
        break;
    
    case 0:
        puts("Child Proccess");
        exit(2);
        break;
    default:
        while(wait(&status) != pid) continue;
        puts("Parent Process");
        printf("Status: %d, %x\n", status, status);
        printf("Child Process Exit Status %d\n", WEXITSTATUS(status) );
        break;
    }
    return 0;
}