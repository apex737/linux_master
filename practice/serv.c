#include "def.h"

int main(int argc, char* argv[])
{
    if(argc != 2){
        printf("USAGE: %s <port>\n", argv[0]);
        exit(1);
    }
    int sock = socket(PF_INET, SOCK_DGRAM, 0);
    if(sock == -1) error_handler("socket");
    struct sockaddr_in serv_addr, clnt_addr;
    socklen_t sock_len = sizeof(clnt_addr);
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htons(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));
    if(bind(sock, (struct sockaddr*)&serv_addr, sock_len) == -1)
        error_handler("bind");
    
    char buf[64];
    int rxbyte;
    while(1)
    {
        rxbyte = recvfrom(sock, buf, sizeof(buf), 0, 
            (struct sockaddr *)&clnt_addr, &sock_len);
        sendto(sock, buf, rxbyte, 0, 
                (struct sockaddr *)&clnt_addr, sock_len);
    }
    return 0;
}