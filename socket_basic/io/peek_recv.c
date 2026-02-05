#include "../def.h"

int main(int argc, char* argv[])
{
     if(argc!=2){
        printf("USAGE: %s <port>\n", argv[0]);
        exit(1);
    }
    int acpt_sock = socket(PF_INET, SOCK_STREAM, 0);
    if(acpt_sock == -1) error_handler("socket");
    struct sockaddr_in serv_addr, clnt_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv_addr.sin_port = htons(atoi(argv[1]));
    if(bind(acpt_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
        error_handler("bind");
    
    listen(acpt_sock, 5);
    socklen_t serv_adr_sz = sizeof(serv_addr);
    int recv_sock = accept(acpt_sock, (struct sockaddr*)&serv_addr, &serv_adr_sz);
    int len;
    char buf[BUF_SZ];
    // 버퍼를 비우지 않고 Non-Block Peeking만 하기 때문에
    // while((len = ...) > 0) 방식 사용 불가
    while(1)
    {
        len = recv(recv_sock, buf, sizeof(buf)-1, MSG_PEEK | MSG_DONTWAIT);
        if(len > 0) break;
    }
    buf[len] = 0;
    printf("Buffering %d bytes: %s\n", len, buf);
    // 버퍼를 진짜로 읽어서 비워냄
    len = recv(recv_sock, buf, sizeof(buf)-1, 0);
    buf[len] = 0;
    printf("Read again: %s\n", buf);
    close(acpt_sock);
    return 0;
}