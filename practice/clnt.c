#include "../def.h"

int main(int argc, char* argv[])
{
    // socket

    int sock = socket(PF_INET, SOCK_STREAM, 0);
    struct sockaddr_in serv_addr;

    if(argc != 3) {
        printf("USAGE: %s <IP> <PORT>\n", argv[0]);
        exit(1);
    }
    
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));
    // connect
    connect(sock, (struct sockaddr *)&serv_addr
    , sizeof(struct sockaddr)); 

    char msg[30];
    // ssize_t cnt = read(sock, msg, sizeof(msg));
    // printf("MSG From Server: %s \n", msg);

    // 서버가 1바이트씩 다 보낼 때까지 잠시 대기
    usleep(100); 
    // Null 문자 제외한것까지만 읽는다
    ssize_t cnt = read(sock, msg, sizeof(msg) - 1);
    if(cnt > 0) {
        msg[cnt] = '\0';
        printf("Total bytes received: %zd\n", cnt);
        printf("Full Message: %s\n", msg);
    }
    close(sock);
    return 0;
}