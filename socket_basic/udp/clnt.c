#include "../../def.h"

int main(int argc, char* argv[])
{
    if(argc != 2){
        printf("USAGE: %s\n", argv[0]);
        exit(1);
    }
    int sock = socket(PF_INET, SOCK_DGRAM, 0);
    struct sockaddr_in sock_addr, clnt_addr;
    socklen_t sock_size = sizeof(sock_addr);
    sock_addr.sin_family = AF_INET;
    sock_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    sock_addr.sin_port = htons(atoi(argv[1]));

    char msg[1024];
    int rxSize;
    while(1)
    {
        printf("Enter Msg: ");
        fgets(msg, sizeof(msg), stdin);
        sendto(sock, msg, strlen(msg), 0, (struct sockaddr *)&sock_addr, sock_size);
        rxSize = recvfrom(sock, msg, sizeof(msg), 0, (struct sockaddr *)&clnt_addr, &sock_size);
        if(rxSize > 0)
        {
            msg[rxSize] = 0;
            printf("Echo : %s", msg);
        }
        
    }
    close(sock);
    return 0;
}