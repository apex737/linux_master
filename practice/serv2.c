#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
int main(int argc, char* argv[])
{
    if(argc != 2){
        printf("USAGE: %s <port>\n", argv[0]);
        exit(1);
    }

    int serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    int clnt_sock;
    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = NULL;
    serv_addr.sin_port = 
    bind(serv_sock, (struct sockaddr *)&serv_addr, sizeof(struct sockaddr));
    listen(serv_sock, 5);
    struct sockaddr_in clnt_addr;
    clnt_sock = accept(serv_sock, (struct sockaddr *)&clnt_addr, sizeof(struct sockaddr_in));
    write
    return 0;
}