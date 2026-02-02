#include <sys/wait.h>
#include "../def.h"

/*  Anon Pipe : Uni-Direction IPC */

int main(void)
{
    int pipePC[2], pipeCP[2];
    if(pipe(pipePC) == -1 || pipe(pipeCP) == -1) 
        error_handler("pipe");
    pid_t pid;
    char msgCh[] = "Child MSG";
    char msgPr[] = "Parent MSG";
    char buf[256];
    int len, status;
    switch (pid = fork())
    {
    case -1: error_handler("fork");
    
    case 0: 
        // 말이 안되는 파이프 닫기 
        close(pipePC[1]); close(pipeCP[0]);
        // child -> parent 메시지 전송하고 바로 닫기
        write(pipeCP[1], msgCh, strlen(msgCh)); 
        puts("CHILD STDOUT POSSIBLE??"); 
        close(pipeCP[1]); 
        if((len = read(pipePC[0], buf, sizeof(buf)-1)) == -1) 
            error_handler("read");
        buf[len] = 0;
        printf("Child Recieved: %s\n", buf);
        close(pipePC[0]); 
        
        break;
    default:
        // 말이 안되는 파이프 닫기
        close(pipePC[0]); close(pipeCP[1]);
        // parent -> child 메시지 전송하고 바로 닫기
        write(pipePC[1], msgPr, strlen(msgPr)); 
        puts("PARENT STDOUT POSSIBLE??");
        close(pipePC[1]); 
        // memset 필요할까? buf[len] = 0 처리하니까 필요없지 않나?
        // memset(buf, 0, sizeof(buf));
        if((len = read(pipeCP[0], buf, sizeof(buf)-1)) == -1) 
            error_handler("read");
        buf[len] = 0;
        close(pipeCP[0]); 
        printf("Parent Recieved: %s\n", buf);
        waitpid(pid, &status,0);
        break;
    }
    return 0;
}

/*  실행 결과
PARENT STDOUT POSSIBLE??
Parent Recieved: Child MSG
CHILD STDOUT POSSIBLE??
Child Recieved: Parent MSG

고찰: pipe()는 커맨드(|)와 달리, stdio를 점유하지 않는다
*/