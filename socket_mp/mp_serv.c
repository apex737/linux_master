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
    socklen_t sock_sz, optlen;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));
    
    int option = 1; // true
    setsockopt(serv_sock, SOL_SOCKET, SO_REUSEADDR, (void*)&option, optlen);

    if(bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
        error_handler("bind");
    
    /*  여기에서 SYNQ와 ACCEPTQ가 생성되며,
        SYNQ에서 3W-HS가 완료된 객체만 소켓 형태로 ACCEPTQ에 Enqueue된다. */
    if(listen(serv_sock, 5) == -1)
        error_handler("listen");
    
    pid_t pid;
    int len;
    char buf[BUF_SZ];
    /*  부모는 fork를 하면서 자식에게 클라이언트 소켓을 넘긴다
        각 프로세스는 자식이 담당한다.   */
    while(1)
    {
        /*  accept는 소켓 사이즈를 바꾸는데 책임지진 않음!
            반드시 MAX_SZ로 매번 갱신해야함  */
        sock_sz = sizeof(clnt_addr);
        /*  ACCEPTQ에서 dequeue하면서 1:1 연결; 이제 소켓에 대고 읽거나 쓸 수 있다.
            ACCEPTQ에 꺼낼 소켓이 없으면 Blocking Wait.  */ 
        clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &sock_sz);
        if(clnt_sock == -1){
            continue;
        } else {
            puts("New Client Connected...");
        }

        switch (pid = fork())
        {
            case -1:
                puts("fork failed");
                continue;
                // error_handler("fork");
            case 0:
                close(serv_sock); // 자식은 clnt_sock만 사용
                                  // EOF를 보낼 때까지 계속 블로킹 read
                while((len = read(clnt_sock, buf, BUF_SZ)) > 0){
                    write(clnt_sock, buf, len);
                }

                close(clnt_sock);
                puts("client disconnected...");
                exit(0);
            default:
                // 부모는 serv_sock만 사용
                close(clnt_sock);
                break;
        }
        
    }
    close(serv_sock);
    return 0;
}