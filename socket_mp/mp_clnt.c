#include "def.h"
#include <sys/signal.h>
#include <sys/wait.h>

/* 입출력 루틴 분할 (생산자-소비자 패턴) */
void write_routine(int sock, char msg[]);
void read_routine(int sock, char msg[]);
void sigchld_handler(int signo);

int main(int argc, char* argv[])
{
    if(argc != 2){
        printf("USAGE: %s <port>\n", argv[0]);
        exit(1);
    }

    // SIGCHLD 핸들러
    struct sigaction act;
    act.sa_flags = 0;
    sigemptyset(&act.sa_mask);
    act.sa_handler = sigchld_handler;
    sigaction(SIGCHLD, &act, 0);

    int sock = socket(PF_INET, SOCK_STREAM, 0);
    if(sock == -1) error_handler("socket");
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv_addr.sin_port = htons(atoi(argv[1]));

    if(connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
        error_handler("connect");
    
    pid_t pid;
    char msg[BUF_SZ];
    int len;
    switch (pid = fork())
    {
    case -1:
        error_handler("fork");
    case 0:
        // child is a producer
        write_routine(sock, msg);
        break;
    default:
        // parent is a consumer
        read_routine(sock, msg);
        // 이하의 처리가 없으면 자식 프로세스가 남게된다
        kill(pid, SIGKILL);
        usleep(1000*100); // 1ms sleep
        break;
    }
    close(sock);
    return 0;
}

void sigchld_handler(int signo)
{
    int status;
    pid_t pid;
    while((pid = waitpid(-1, &status, WNOHANG)) > 0)
    {
        printf("[REMOVE] PID: %d, STATUS: %d\n",
            pid, WEXITSTATUS(status));
    }
}

void write_routine(int sock, char msg[])
{
    while(1)
    {
        fgets(msg, BUF_SZ, stdin);
        if(strcmp(msg, "q\n") == 0 || strcmp(msg, "Q\n") == 0) 
        {
            /*  자식 프로세스가 close(fd)로 RC-=1을 해도 여전히 부모가 소켓을 잡고 있어서
                (RC > 0) 서버가 종료 시퀀스에 돌입하지 않는다.
                반면, shutdown은 서버에게 EOF를 직접 전달하기 때문에
                close(clnt_sock)에 의해 부모 프로세스도 종료된다.  */
            shutdown(sock, SHUT_WR); 
            return;
        }
        write(sock, msg, strlen(msg));
    }
}

void read_routine(int sock, char msg[])
{
    int len;
    while((len = read(sock, msg, BUF_SZ-1)) > 0) 
    {
        msg[len] = 0;
        printf("ECHO: %s\n", msg);
    }
}