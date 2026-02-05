#include "def.h"
#include <sys/signal.h>
#include <sys/wait.h>

/*  멀티 프로세싱 에코 서버
    1. 소켓을 만든다
    2. 매 통신 소켓 생성마다 fork를 통해 fd를 복사한다
    - 필요없는 fd는 삭제한다.
    3. 각 클라이언트 종료마다 자식 리소스를 해제한다.
    4. Pipe를 사용하여 파일에 로그를 남긴다. */

void sigchld_handler(int signo)
{
    pid_t pid;
    int status;
    while((pid = waitpid(-1, &status, WNOHANG)) > 0){
        printf("[REMOVED] PID: %d, EXIT STATUS: %d\n",
            pid, WEXITSTATUS(status));
    }
}

int main(int argc, char* argv[])
{
    if(argc != 2){
        printf("USAGE: %s <port> \n", argv[0]);
        exit(1);
    }

    // SIGCHLD 핸들러
    struct sigaction act;
    act.sa_handler = sigchld_handler;
    act.sa_flags = 0;
    sigemptyset(&act.sa_mask); // sigprocmask
    sigaction(SIGCHLD, &act, 0);

    int serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    if(serv_sock == -1) error_handler("socket");
    struct sockaddr_in serv_addr, clnt_addr;
    socklen_t sock_sz;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));

    int option = 1;
    setsockopt(serv_sock, SOL_SOCKET, SO_REUSEADDR, &option, sock_sz); 
    if(bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
        error_handler("bind");
    
    // SYNQ, ACCEPTQ 생성
    if(listen(serv_sock, 5) == -1) error_handler("listen");
    pid_t pid;
    int clnt_sock, len;
    char buf[BUF_SZ];
    
    int fds[2];
    FILE* fp;
    // 파이프에 값을 쓰는게 write(fds[1]), 읽는게 read(fds[0])
    pipe(fds);
    switch (pid = fork())
    {
        case -1: error_handler("fork");
        case 0:
            // 여기에서 로깅
            close(fds[1]); // 쓰기 필요 없음
            fp = fopen("msgLog.txt", "w");
            if(fp == NULL) error_handler("fopen");
            else puts("log process on service");
            while((len = read(fds[0], buf, BUF_SZ)) > 0) // 계속 읽기
            {
                // Stack(buf) 값을 Heap(FILE BUF)으로 보냄
                fwrite((void*)buf, 1, len, fp);
                // Heap(FILE BUF) 값을 file(Page Cache)로 보냄 
                fflush(fp); 
                // 또는 한번에 Stack => Page Cache
                // write(fileno(fp), buf, len);
            }

            fclose(fp);
            close(fds[0]);
            return 0;

        default:
            close(fds[0]); // 읽기 필요 없음.
            while(1)
            {
                sock_sz = sizeof(clnt_addr);
                clnt_sock = accept(serv_sock, 
                                    (struct sockaddr*)&clnt_addr, &sock_sz);
                if(clnt_sock == -1) continue;
                else puts("New Client Connected...");

                pid = fork();
                if(pid == 0)
                {
                    // 안쓰는 소켓 항상 먼저 닫아준다
                    close(serv_sock);
                    /*  Double Close Issue (매우 중요!!)
                        실수로 close(fds[0])를 호출하면 OS는 clnt_sock을 닫아버린다. 
                        이미 닫힌 fds[0]가 다른 fd를 위해 재사용 되기 때문이다. */
                    printf("clnt_sock: %d, fds[0]: %d\n", clnt_sock, fds[0]); 

                    /*  자식은 매 클라이언트 생성마다 에코 서비스를 제공하고 
                        연결이 끊기면 부모가 자원을 반납할수 있도록 함 */
                    while((len = read(clnt_sock, buf, BUF_SZ)) > 0){
                        write(clnt_sock, buf, len);
                        write(fds[1], buf, len);
                    }
                    // 루프 탈출은 곧 EOF; 즉 클라이언트의 연결이 끊긴 상황
                    close(clnt_sock);
                    close(fds[1]);
                    return 0;
                }

                close(clnt_sock);
            }            
    }
    close(serv_sock);
    return 0;
}