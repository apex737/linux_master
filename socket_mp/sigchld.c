#include "def.h"
#include <stdbool.h>
#include <signal.h>
#include <sys/wait.h>

// 자식 처리는 SIGCHLD 핸들러에서 수행하는게 정석
void sigchld_handler(int signo)
{
    int status;
    pid_t pid;
    // 종료된 모든 자식을 한 번에 수거하기 위해 while 루프 사용
    while((pid = waitpid(-1, &status, WNOHANG)) > 0) {
        printf("REMOVE PID: %d, STATUS: %d\n", pid, WEXITSTATUS(status));
    }
}

int main()
{
    pid_t pid;
    struct sigaction act;
    act.sa_handler = sigchld_handler;
    act.sa_flags = 0;
    sigemptyset(&act.sa_mask);
    sigaction(SIGCHLD, &act, 0);

    switch (pid = fork())
    {
        case -1:
            error_handler("fork");
        case 0:
            puts("Child Process 1"); 
            sleep(10);
            exit(1);
        default:
            printf("Child1 PID: %d\n", pid); 
            pid = fork();
            if(pid == 0)
            {
                puts("Child Process 2");  
                sleep(10);
                exit(2);        
            }
            else
            {
                printf("Child2 PID: %d\n", pid); 
                for(int i=0; i <5; i++){
                    puts("wait...");
                    sleep(5);
                }
            }
    }
    return 0;
}

/*  실행 결과: vTaskDelay(1000) ~= sleep(1)
    [0초]
    Child1 PID: 2275130     // 부모 프로세스 => fork()
    Child Process 1         // 자식1 프로세스 => sleep(10)
    Child2 PID: 2275131     // 부모 프로세스 => sleep(5)
    wait...
    Child Process 2         // 자식2 프로세스 => sleep(10)

    [5초]
    wait...                 // 부모 프로세스 => sleep(5)

    [10초]                  
    wait...                         // 부모 프로세스 => sleep(5)
    REMOVE PID: 2287353, STATUS: 1  // 시그널 핸들러 => sleep에서 즉시 깨어남 
    REMOVE PID: 2287354, STATUS: 2
    
    [15초]
    wait...                 // 부모 프로세스 => sleep(5)

    [20초]
    wait...                 // 부모 프로세스 => sleep(5)
*/