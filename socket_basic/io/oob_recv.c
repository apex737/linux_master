#include "../def.h"
#include <signal.h>

// 전역 선언에 이유라도?
int acpt_sock, recv_sock;
void sigurg_handler(int signo);

int main(int argc, char* argv[])
{
    struct sockaddr_in recv_adr, serv_adr;
    int len, state;
    socklen_t serv_adr_sz;
    char buf[BUF_SZ];
    if(argc!=2){
        printf("USAGE: %s <port>\n", argv[0]);
        exit(1);
    }
    // SIGURG 핸들러
    struct sigaction act;
    act.sa_handler = sigurg_handler;
    act.sa_flags = 0;
    sigemptyset(&act.sa_mask);
    
    // 리스닝 소켓
    acpt_sock = socket(PF_INET, SOCK_STREAM, 0);
    memset(&recv_adr, 0, sizeof(recv_adr));
    recv_adr.sin_family = AF_INET;
    recv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    recv_adr.sin_port = htons(atoi(argv[1]));
    if(bind(acpt_sock, (struct sockaddr*)&recv_adr, sizeof(recv_adr)) == -1)
        error_handler("bind");
    listen(acpt_sock, 5);
    serv_adr_sz = sizeof(serv_adr);
    recv_sock = accept(acpt_sock, (struct sockaddr*)&serv_adr, &serv_adr_sz);

    // fcntl: recv_sock에서 발생한 SIGURG의 목적지를 현재 프로세스로 고정
    // fork 등에 의해 recv_sock이 복사될 수 있기 때문 
    fcntl(recv_sock, F_SETOWN, getpid());
    state = sigaction(SIGURG, &act, 0);

    while((len = recv(recv_sock, buf, sizeof(buf), 0)) != 0)
    {
        if(len == -1) continue;
        buf[len] = 0;
        puts(buf);
    }

    close(recv_sock);
    close(acpt_sock);
    return 0;
}

void sigurg_handler(int signo)
{
    int len;
    char buf[BUF_SZ];
    len = recv(recv_sock, buf, sizeof(buf), MSG_OOB);
    buf[len] = 0;
    printf("URG MSG: %s\n", buf);
}