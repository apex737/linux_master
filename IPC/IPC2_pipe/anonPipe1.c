#include <sys/wait.h>
#include "../def.h"

/*  Anon Pipe : Uni-Direction IPC
    OS가 임의로 생성한 Anon fd를 다른 Process에 전달할 방법은 fork (상속) 뿐이다.
    따라서, Child <-> Parent 사이의 IPC를 위해 사용된다. */

int main(void)
{
    int fd[2];
    pid_t pid;
    char buf[256];
    int len, status;
    // Everything is a file; 
    // pipe 생성도 open(), socket()과 유사
    if(pipe(fd) == -1) error_handler("pipe");

    char msg[] = "Child Proccess: ";
    // 프로세스 분기
    switch (pid = fork())
    {
    case -1: error_handler("fork");
    case 0:
        close(fd[1]); // close child pipe_o
        write(STDOUT_FILENO, msg, strlen(msg)); // msg 화면 출력
        // pipe_i에서 값 읽기
        if((len = read(fd[0], buf, 256-1)) == -1) error_handler("read"); 
        // 만약 child가 먼저 실행됐는데 파일(pipe)에 읽을 값이 없으면? 
        // read는 블로킹 API라서 부모에게 턴을 넘기고, 대기큐에서 기다림
        // 중요! 일단 읽기 시작하면 EOF (close(fd))가 올 때까지 계속 값을 받는다!
        write(STDOUT_FILENO, buf, len); // buf에서 꺼낸 값 화면 출력 
        close(fd[0]); // close child pipe_i; 
        break;
    default:
        close(fd[0]); // close parent pipe_i
        buf[0] = 0;
        // pipe_o에 쓰기
        write(fd[1], "Test msg\n", strlen("Test msg\n"));
        close(fd[1]); // close parent pipe_o; (Write EOF to pipe)
        // 자식 끝날때까지 블로킹 대기
        waitpid(pid, &status, 0);
        break;
    }
    return 0;
}