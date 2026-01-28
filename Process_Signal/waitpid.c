#include <sys/wait.h>
#include "../def.h"

int main()
{
    pid_t pid = fork();
    int status;
    if(pid==0)
    {
        puts("Child Process");
        sleep(3);
        exit(3);
    } 
    else 
    {
        puts("Parent Process");
        while(waitpid(pid, &status, WNOHANG) == 0){
            puts("Parent still wait...");
            sleep(1);
        }
        printf("Child Exit Status: %d\n", WEXITSTATUS(status));
    }

    return 0;
}