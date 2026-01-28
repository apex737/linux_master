#include "../../def.h"

int main(int argc, char* argv[])
{
    if(argc != 2){
        printf("USAGE: %s\n", argv[0]);
        exit(1);
    }
    int sock = socket(PF_INET, SOCK_DGRAM, 0);
    if(sock == -1) error_handler("socket() error");
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(atoi(argv[1]));
    if(bind(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1) 
        error_handler("bind() error");

    char msg[1024];
    struct sockaddr from;
    socklen_t pkt_len = sizeof(from);
    ssize_t rxSize;
    while(1)
    {
        rxSize = recvfrom(sock, msg, sizeof(msg), 0, &from, &pkt_len);
        sendto(sock, msg, rxSize, 0, &from, pkt_len);
    }

    return 0;
}