#include "def.h"
#include <sys/wait.h>

void sigchld_handler(int signo)
{
    pid_t pid;
    int status;
    // 자식 프로세스 pid를 반환하는 non-blocking wait
    while((pid = waitpid(-1, &status, WNOHANG)) > 0)
    {
        printf("REMOVE PID: %d, EXIT STATUS: %d\n", 
            pid, WEXITSTATUS(status));
    }
}


int main(int argc, char* argv[])
{
    if(argc != 2){
        printf("USAGE: %s <port>\n", argv[0]);
        exit(1);
    }

    struct sigaction act;
    act.sa_flags = 0;
    act.sa_handler = sigchld_handler;
    sigemptyset(&act.sa_mask);
    sigaction(SIGCHLD, &act, 0);

    int serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    int clnt_sock;
    if(serv_sock==-1) error_handler("socket");
    struct sockaddr_in serv_addr, clnt_addr;
    socklen_t sock_sz;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));
    
    if(bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
        error_handler("bind");
    
    if(listen(serv_sock, 5) == -1)
        error_handler("listen");
    int fds[2];
    pipe(fds);
    char buf[BUF_SZ];
    int len;
    pid_t pid = fork();
    if(pid==0)
    {
        FILE *fp = fopen("echomsg.txt", "w");
        char msgbuf[BUF_SZ];
        int i,len;
        for(i=0;i<10;i++)
        {
            len=read(fds[0], msgbuf, BUF_SZ);
            fwrite((void*)msgbuf, 1, len, fp);
        }
        fclose(fp);
        return 0;
    }

    while(1)
    {
        sock_sz = sizeof(clnt_addr);
        clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &sock_sz);
        if(clnt_sock==-1) continue;
        else puts("new client connected...");

        pid = fork();
        if(pid==0)
        {
            close(serv_sock);
            while((len = read(clnt_sock, buf, BUF_SZ)) != 0)
            {
                write(clnt_sock, buf, len);
                write(fds[1], buf, len);
            }

            close(clnt_sock);
            puts("client disconnected...");
            return 0;
        } 
        else {
            close(clnt_sock);
        }
        
    }
    close(serv_sock);
    return 0;
}