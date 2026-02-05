#include "../def.h"

int main(int argc, char* argv[])
{
    if(argc!=2){
        printf("USAGE: %s <port>\n", argv[0]);
        exit(1);
    }
    int sock = socket(PF_INET, SOCK_STREAM, 0);
    if(sock == -1) error_handler("socket");
    struct sockaddr_in serv_addr, clnt_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv_addr.sin_port = htons(atoi(argv[1]));
    if(connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
        error_handler("connect");

    // 루프백 환경(127.0.0.1)이므로 의도적 지연 추가
    write(sock, "123", 3); usleep(1);
    send(sock, "4", 1, MSG_OOB); usleep(1);
    write(sock, "567", 3); usleep(1);
    // OOB는 1byte만 전송 가능함
    send(sock, "890", 3, MSG_OOB); // 0
                                   // 89
    close(sock);
    
    return 0;
}