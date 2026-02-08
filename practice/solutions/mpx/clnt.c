#include "../def.h"
#include <signal.h>
#include <wait.h>

void sigchld_handler(int signo);
void write_routine(int fd, char buf[]);
void read_routine(int fd, char buf[]);

int main(int argc, char* argv[])
{
    if(argc !=2){
        printf("USAGE: %s <port>\n", argv[0]);
        exit(1);
    }

    struct sigaction act;
    act.sa_flags = 0;
    act.sa_handler = sigchld_handler;
    sigemptyset(&act.sa_mask);
    sigaction(SIGCHLD, &act, 0);

    int sock = socket(PF_INET, SOCK_STREAM, 0);
    if(sock == -1) error_handler("sock");
    struct sockaddr_in serv_adr;
    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv_adr.sin_port = htons(atoi(argv[1]));
    if(connect(sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)
        error_handler("connect");
    
    char buf[BUF_SZ];
    int len;
    pid_t pid = fork();
    if(pid == 0)
    {
        write_routine(sock, buf);
        // q로 빠져나오면? 소켓을 close 
        shutdown(sock, SHUT_WR);
    }
    else 
    {
        read_routine(sock, buf);
    }

    close(sock);
    return 0;
}

void sigchld_handler(int signo)
{
    pid_t pid;
    int status;
    while((pid = waitpid(-1, &status, WNOHANG)) > 0){
        printf("[REMOVE] PID: %d, STATUS: %d\n", 
                pid, WEXITSTATUS(status));
    }
}

void write_routine(int fd, char buf[])
{
    int len;
    while(1)
    {
        fgets(buf, BUF_SZ, stdin);
        if(strcmp(buf, "q\n") == 0 || strcmp(buf, "Q\n") == 0) break;
        write(fd, buf, strlen(buf));
    }
}

void read_routine(int fd, char buf[])
{
    int len;
    while((len = read(fd, buf, BUF_SZ))>0)
    {
        buf[len] = 0;
        printf("[ECHO] %s\n", buf);
    }
    puts("read_routine got EOF");
}