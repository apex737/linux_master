#include "../def.h"

int main()
{
    pid_t pid;
    switch (pid = fork())
    {
    case -1:
        error_handler("fork() error");
        break;
    case 0:
        if(execlp("ls", "ls", "-a", "-l", NULL) == -1)
            error_handler("execlp");
        exit(0);
    default:
        printf("Parent Pid: %d\n", (int)getpid());
        break;
    }
    return 0;
}